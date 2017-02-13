//
// Created by hindrik on 27-1-17.
//

#include <iostream>
#include <algorithm>
#include "VulkanCore.h"
#include "../Core/Console.h"
#include "Renderer.h"


VulkanCore::VulkanCore(string applicationName, vector<const char*> enabledKHRInstanceExtensions, vector<const char*> enabledInstanceValidationLayers, bool enableValidationLayers)
        : m_application_name(applicationName) , m_layerAndExtensionManager(enabledKHRInstanceExtensions,enabledInstanceValidationLayers, enableValidationLayers)
{
    VkResult r = vk_init();
    if(r == VK_SUCCESS)
        Console::printLine("Initialized Vulkan API succesfully.");
}

VulkanCore::~VulkanCore() {
    m_layerAndExtensionManager.cleanupDebugFacilities();
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance,nullptr);
}

VkResult VulkanCore::vk_init() {
    VkResult result;

    result = vk_init_instance();
    VK_CHECK(result);

    //Enable debug if necessary
    if(m_layerAndExtensionManager.getDebugEnabled())
    {
        m_layerAndExtensionManager.setupDebugFacilities();
    }

    result = vk_init_physical_device();
    VK_CHECK(result);

    result = vk_init_logical_device();
    VK_CHECK(result);

    result = vk_init_create_surface();
    VK_CHECK(result);

    vk_init_assign_queues();


    return result;
}

bool VulkanCore::process_platform_API(float deltaTime) {
    return m_platform.processAPI(deltaTime);
}

VkResult VulkanCore::vk_init_instance() {

    VkResult result;

    VkApplicationInfo applicationInfo = {};

    vector<const char *> enabledExtensions = m_layerAndExtensionManager.getEnabledInstanceKHRExtensions();
    vector<const char *> enabledLayers = m_layerAndExtensionManager.getEnabledInstanceValidationLayers();

    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = m_application_name.c_str();
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "Vulkano Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t >(enabledExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t >(enabledLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);

    if (result == VK_SUCCESS)
    {
        m_layerAndExtensionManager.setVkInstanceHandle(m_instance);
    }

    return result;
}

VkResult VulkanCore::vk_init_physical_device() {
    VkResult result;

    uint32_t physical_device_count = 0;
    vector<VkPhysicalDevice> physical_devices;
    VkPhysicalDevice selected_device = VK_NULL_HANDLE;


    result = vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);
    VK_IF_FAIL_MSG(result, "Error when retrieving amount of physical devices.");

    if(physical_device_count <= 0) {
        throw std::runtime_error("Error, no physical devices supporting Vulkan found!.");
    }

    physical_devices.resize(physical_device_count);

    result = vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());
    VK_IF_FAIL_MSG(result, "Error when retrieving physical devices.");

    vector<VkPhysicalDevice> possibleGPUs;

    cout << "Possible GPU's found: " << std::endl;

    for(const auto& device : physical_devices)
    {
        if(vk_init_check_device(device))
        {
            possibleGPUs.push_back(device);
        }
    }

    if(possibleGPUs.size() == 0)
    {
        throw::std::runtime_error("Failed to find a suitable GPU. The GPU needs to be a discrete or integrated GPU, and needs to support geometry shaders and tesselation.");
    }

    vector<vk_device_rating> ratings;



    for(const auto& device : possibleGPUs)
    {
        ratings.push_back(vk_device_rating{device,vk_init_suitability_rating(device)});
    }

    std::sort(ratings.begin(), ratings.end(), [](const vk_device_rating& left, const vk_device_rating& right){ return right.m_rating < left.m_rating; });

    selected_device = ratings.front().m_device;

    if(selected_device == VK_NULL_HANDLE)
    {
        throw::std::runtime_error("Selected device is a VK_NULL_HANDLE! ");
    }

    m_physical_device = selected_device;


    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(selected_device, &props);


    std::cout << "Selected device: " << std::endl << props << std::endl;

    m_physical_device_info = vk_init_get_queue_families(selected_device);



    return result;
}

VkResult VulkanCore::vk_init_logical_device() {

    VkResult result;

    m_queue_families.clear();

    vk_init_setup_queue_families(m_physical_device_info.m_queue_family_properties);

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    if(m_queue_families.size() > 0)
    {
        for(uint32_t i = 0; i < m_queue_families.size(); ++i)
        {

            VkDeviceQueueCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            info.pNext = nullptr;
            info.queueCount = static_cast<uint32_t >(m_queue_families[i].m_queue_family_properties.queueCount);
            info.queueFamilyIndex = m_queue_families[i].m_index;
            info.pQueuePriorities = m_queue_families[i].m_priorities.data();

            queueCreateInfos.push_back(info);
        }
    }

    //TODO: geomshaders and tesselationshaders?
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t >(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;

    //Device layers are deprecated, so we disable them
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;

    createInfo.enabledExtensionCount = static_cast<uint32_t >(enabledDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();


    result = vkCreateDevice(m_physical_device, &createInfo, nullptr, &m_device);
    if(result != VK_SUCCESS)
    {
        Console::printLine("Error when creating device.");
    }

    return result;
}

void VulkanCore::vk_init_setup_queue_families(const vector<VkQueueFamilyProperties>& queueFamilies) {

    for(uint32_t i = 0;  i < static_cast<uint32_t >(queueFamilies.size()); ++i)
    {
        const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

        vk_queue_family qf { queueFamily, i };

        for(uint32_t j = 0; j < queueFamily.queueCount; ++j)
            qf.m_priorities.push_back(1.0f);

        m_queue_families.push_back(qf);
    }
}

void VulkanCore::vk_init_assign_queues() {

    bool graphicsAssigned = false;

    for(uint32_t i = 0; i < static_cast<uint32_t >(m_queue_families.size()); ++i)
    {
        vk_queue_family& qf = m_queue_families[i];

        if(qf.m_queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, qf.m_index, m_surface, &presentSupport);
            if(presentSupport == VK_FALSE)
                continue;

            for(uint32_t j = 0; j < qf.m_queue_family_properties.queueCount; ++j)
            {
                if(!graphicsAssigned)
                {
                    //Assign as main graphics queue

                    vkGetDeviceQueue(m_device, qf.m_index, j, &m_graphics_queue);

                    graphicsAssigned = true;
                } else
                {
                    m_additional_graphics_queues.push_back(VK_NULL_HANDLE);
                    vkGetDeviceQueue(m_device, qf.m_index, j, &m_additional_graphics_queues.back());
                }
            }
        }

        if(qf.m_queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for(uint32_t j = 0; j < qf.m_queue_family_properties.queueCount; ++j)
            {
                m_compute_queues.push_back(VK_NULL_HANDLE);
                vkGetDeviceQueue(m_device, qf.m_index, j, &m_compute_queues.back());
            }
        }

        if(qf.m_queue_family_properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        {
            for(uint32_t j = 0; j < qf.m_queue_family_properties.queueCount; ++j)
            {
                m_sparse_binding_queues.push_back(VK_NULL_HANDLE);
                vkGetDeviceQueue(m_device, qf.m_index, j, &m_sparse_binding_queues.back());
            }
        }

        if(qf.m_queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT && !qf.m_queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && !qf.m_queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for(uint32_t j = 0; j < qf.m_queue_family_properties.queueCount; ++j)
            {
                m_transfer_only_queues.push_back(VK_NULL_HANDLE);
                vkGetDeviceQueue(m_device, qf.m_index, j, &m_transfer_only_queues.back());
            }
        }
    }
}

VkResult VulkanCore::vk_init_create_surface() {

    VkResult result;

    result = m_platform.createSurface(m_instance, m_surface);

    return result;
}

