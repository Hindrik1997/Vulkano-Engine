//
// Created by hindrik on 27-1-17.
//

#include <iostream>
#include "VulkanCore.h"

VulkanCore::VulkanCore(string applicationName) : m_application_name(applicationName) {
    VkResult r = vk_init();
    if(r == VK_SUCCESS)
        std::cout << "Initialized Vulkan API" << std::endl;
}

VulkanCore::~VulkanCore() {

    vkDestroyCommandPool(m_device,m_selected_device_command_pool,nullptr);
    std::cout << "Destroyed default command pool" << std::endl;

    vkDestroyDevice(m_device, nullptr);
    std::cout << "Destroyed vulkan device" << std::endl;

    vkDestroyInstance(m_instance, nullptr);
    std::cout << "Destroyed vulkan instance" << std::endl;

}

VkResult VulkanCore::vk_init() {
    VkResult result;

    result = vk_init_instance();
    VK_CHECK(result);
    result = vk_init_enumerate_physical_devices_and_queues();
    VK_CHECK(result);
    result = vk_init_select_device_and_queues();
    VK_CHECK(result);
    result = vk_init_create_command_pool();
    VK_CHECK(result);
    result = vk_init_create_default_command_buffer();
    VK_CHECK(result);
    result = vk_init_setup_swapchain();
    VK_CHECK(result);

    return result;
}

VkResult VulkanCore::vk_init_instance() {

    VkResult result;
    VkApplicationInfo appInfo = {};
    VkInstanceCreateInfo instanceCreateInfo = {};
    vector<const char*> extensions =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
    };


    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext               = nullptr;
    appInfo.pApplicationName    = m_application_name.c_str();
    appInfo.applicationVersion  = 1;
    appInfo.pEngineName         = "Vulcan Engine";
    appInfo.engineVersion       = VK_MAKE_VERSION(0,0,1);
    appInfo.apiVersion          = VK_API_VERSION_1_0;

    instanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                    = nullptr;
    instanceCreateInfo.flags                    = 0;
    instanceCreateInfo.pApplicationInfo         = &appInfo;
    instanceCreateInfo.enabledExtensionCount    = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames  = &extensions[0];
    instanceCreateInfo.enabledLayerCount        = 0;
    instanceCreateInfo.ppEnabledLayerNames      = nullptr;

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);

    if(result == VK_ERROR_INCOMPATIBLE_DRIVER)
    {
        std::cout << "Cannot find a proper driver for Vulkan support." << std::endl;
        return result;
    }
    if(result != VK_SUCCESS)
    {
        std::cout << "Some error occurred during the creation of the instance." << std::endl;
        return result;
    }
    return result;
}

VkResult VulkanCore::vk_init_enumerate_physical_devices_and_queues() {

    VkResult result;
    vector<VkPhysicalDevice> devices;

    uint32_t physical_device_count = 0;
    result = vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);

    if(result != VK_SUCCESS)
    {
        std::cout << "Some error occurred during the fetching of the device count." << std::endl;
        return result;
    }

    devices.resize(physical_device_count);
    result = vkEnumeratePhysicalDevices(m_instance, &physical_device_count, &devices[0]);
    if(result != VK_SUCCESS)
    {
        std::cout << "Some error occurred during the fetching of the device list." << std::endl;
        return result;
    }

    m_physical_devices_queue_families.resize(physical_device_count);

    for(uint32_t i = 0; i < physical_device_count; ++i)
    {
        VkPhysicalDevice& device                = devices[i];
        vk_physical_device_info device_info     = {};
        uint32_t queue_family_count             = 0;
        device_info.m_physical_device           = device;


        vkGetPhysicalDeviceQueueFamilyProperties(device_info.m_physical_device, &queue_family_count, nullptr);

        device_info.m_queue_family_count        = queue_family_count;
        device_info.m_queue_family_properties.resize(queue_family_count);

        vkGetPhysicalDeviceQueueFamilyProperties(
                device_info.m_physical_device,
                &device_info.m_queue_family_count,
                &device_info.m_queue_family_properties[0]
        );

        m_physical_devices_queue_families[i]     = device_info;

        VkPhysicalDeviceProperties props = {};
        vkGetPhysicalDeviceProperties(device, &props);

        std::cout << props;

        for(uint32_t j = 0; j < device_info.m_queue_family_count; ++j)
        {
            std::cout << device_info.m_queue_family_properties[j];
        }
    }

    return result;
}

VkResult VulkanCore::vk_init_select_device_and_queues() {
    VkResult result;

    bool found = false;

    //TODO: first device for now, fix later when understanding vulkan better
    vk_physical_device_info& device_info = m_physical_devices_queue_families[0];

    uint32_t queue_graphics_family_index = 0;

    for(uint32_t i = 0; i < device_info.m_queue_family_count; ++i)
    {
        if(device_info.m_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queue_graphics_family_index = i;
            found = true;
            break;
        }
    }

    if(!found)
    {
        std::cout << "Cannot find appropriate graphics queue for the selected device." << std::endl;
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    //TODO: zero priorities for now, fix later when understanding vulkan better
    float queue_priorities[1] = {0.0};

    VkDeviceQueueCreateInfo device_queue_create_info = {};

    vector<const char*> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    device_queue_create_info.sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    device_queue_create_info.pNext              = nullptr;
    device_queue_create_info.queueCount         = 1;
    device_queue_create_info.pQueuePriorities   = queue_priorities;
    device_queue_create_info.queueFamilyIndex   = queue_graphics_family_index;

    VkDeviceCreateInfo device_create_info = {};

    device_create_info.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext                    = nullptr;
    device_create_info.queueCreateInfoCount     = 1;
    device_create_info.pQueueCreateInfos        = &device_queue_create_info;
    device_create_info.enabledExtensionCount    = static_cast<uint32_t>(extensions.size());
    device_create_info.ppEnabledExtensionNames  = &extensions[0];
    device_create_info.enabledLayerCount        = 0;
    device_create_info.ppEnabledLayerNames      = nullptr;
    device_create_info.pEnabledFeatures         = nullptr;

    result = vkCreateDevice(device_info.m_physical_device, &device_create_info, nullptr, &m_device);
    if(result != VK_SUCCESS)
    {
        std::cout << "Device creation failed." << std::endl;
        return result;
    }

    m_selected_queue_family                     = queue_graphics_family_index;
    m_selected_device_queue_family_properties   = device_info.m_queue_family_properties;

    return result;
}

VkResult VulkanCore::vk_init_create_command_pool() {
    VkResult result;

    VkCommandPoolCreateInfo pool_info = {};

    pool_info.sType                 = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.pNext                 = nullptr;
    pool_info.queueFamilyIndex      = m_selected_queue_family;
    pool_info.flags                 = 0;

    result = vkCreateCommandPool(m_device, &pool_info,nullptr, &m_selected_device_command_pool);
    if(result != VK_SUCCESS)
    {
        std::cout << "There was an issue allocating the command pool." << std::endl;
        return result;
    }
    return result;
}

VkResult VulkanCore::vk_init_create_default_command_buffer() {

    VkResult result;

    VkCommandBufferAllocateInfo buffer_allocate_info = {};

    buffer_allocate_info.sType                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_allocate_info.pNext                  = nullptr;
    buffer_allocate_info.commandPool            = m_selected_device_command_pool;
    buffer_allocate_info.commandBufferCount     = 1;

    result = vkAllocateCommandBuffers(m_device,&buffer_allocate_info, &m_selected_device_default_command_buffer);

    if(result != VK_SUCCESS)
    {
        std::cout << "Something went wrong while allocating the default command buffer." << std::endl;
        return result;
    }
    return result;
}

VkResult VulkanCore::vk_init_setup_swapchain() {

    platform = new VulkanPlatform();

    VkResult result;

    VkXcbSurfaceCreateInfoKHR createInfoKHR = {};

    createInfoKHR.sType             = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfoKHR.pNext             = nullptr;
    createInfoKHR.window            = platform->getWindow();
    createInfoKHR.connection        = platform->getConnection();

    result = vkCreateXcbSurfaceKHR(m_instance, &createInfoKHR,nullptr, &m_surface);

    if(result != VK_SUCCESS)
    {
        std::cout << "Surface creation failed." << std::endl;
        return result;
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfoKHR = {};

    swapchainCreateInfoKHR.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfoKHR.pNext            = nullptr;
    swapchainCreateInfoKHR.surface          = m_surface;
    swapchainCreateInfoKHR.imageFormat      = VK_FORMAT_B8G8R8_UNORM;













}

bool VulkanCore::process_platform_API(float deltaTime) {
    return platform->processAPI(deltaTime);
}
