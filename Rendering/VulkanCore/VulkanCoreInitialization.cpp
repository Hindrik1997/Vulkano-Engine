//
// Created by hindrik on 13-2-17.
//

#include <iostream>
#include "../VulkanCore.h"
#include "../../Core/Console.h"
#include <algorithm>

VkResult VulkanCore::vkInit() {
    VkResult result;

    result = vkInitInstance();
    VK_IF_FAIL_MSG(result, "Instance creation failed.");

    //Enable debug if necessary
    if(m_IsDebugEnabled)
    {
        setupDebugFacilities();
    }

    result = vkInitCreateSurface();
    VK_IF_FAIL_MSG(result, "Surface creation failed.")


    result = vkInitPhysicalDevice();
    VK_IF_FAIL_MSG(result, "Physical device selection failed.")

    result = vkInitLogicalDevice();
    VK_IF_FAIL_MSG(result, "Logical device creation failed.");

    vkInitAssignQqueues();

    result = vkInitCreateSwapchain();
    VK_IF_FAIL_MSG(result, "Swapchain creation failed.");

    result = vkInitCreateSwapchainImageViews();
    VK_IF_FAIL_MSG(result, "Error occured when creating the image views");

    return result;
}

VkResult VulkanCore::vkInitInstance() {

    VkResult result;

    VkApplicationInfo applicationInfo = {};


    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = m_ApplicationName.c_str();
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "Vulkano Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t >(m_EnabledInstanceKHRExtensionNames.size());
    instanceCreateInfo.ppEnabledExtensionNames = m_EnabledInstanceKHRExtensionNames.data();
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t >(m_EnabledInstanceValidationLayerNames.size());
    instanceCreateInfo.ppEnabledLayerNames = m_EnabledInstanceValidationLayerNames.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance);

    return result;
}

VkResult VulkanCore::vkInitPhysicalDevice() {

    VkResult                    result;

    uint32_t                    physicalDeviceCount = 0;
    vector<VkPhysicalDevice>    physicalDevices;
    VkPhysicalDevice            selectedDevice;


    result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, nullptr);
    VK_IF_FAIL_MSG(result, "Error when retrieving amount of physical devices.");

    if(physicalDeviceCount <= 0) {
        throw std::runtime_error("Error, no physical devices supporting Vulkan found!.");
    }

    physicalDevices.resize(physicalDeviceCount);

    result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, physicalDevices.data());
    VK_IF_FAIL_MSG(result, "Error when retrieving physical devices.");

    vector<VkPhysicalDevice> possibleGPUs;

    cout << "Possible GPU's found: " << std::endl;

    for(const auto& device : physicalDevices)
    {
        if(vkInitCheckDevice(device, m_EnabledDeviceExtensions, m_Surface))
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
        ratings.push_back(vk_device_rating{device, vkInitSuitabilityRating(device)});
    }

    std::sort(ratings.begin(), ratings.end(), [](const vk_device_rating& left, const vk_device_rating& right){ return right.m_Rating < left.m_Rating; });

    selectedDevice = ratings.front().m_PhysicalDevice;

    if(selectedDevice == VK_NULL_HANDLE)
    {
        throw::std::runtime_error("Selected device is a VK_NULL_HANDLE! ");
    }

    m_PhysicalDevice = selectedDevice;


    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(selectedDevice, &props);


    std::cout << "Selected device: " << std::endl << props << std::endl;

    m_PhysicalDeviceInfo = vkInitGetQueueFamilies(selectedDevice);

    vkEnumerateDeviceExtentions(m_PhysicalDevice);

    return result;
}

VkResult VulkanCore::vkInitLogicalDevice() {

    VkResult result;

    m_QueueFamilies.clear();

    vkInitSetupQueueFamilies(m_PhysicalDeviceInfo.m_QueueFamilyProperties);

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    if(m_QueueFamilies.size() > 0)
    {
        for(uint32_t i = 0; i < m_QueueFamilies.size(); ++i)
        {

            VkDeviceQueueCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            info.pNext = nullptr;
            info.queueCount = static_cast<uint32_t >(m_QueueFamilies[i].m_QueueFamilyProperties.queueCount);
            info.queueFamilyIndex = m_QueueFamilies[i].m_Index;
            info.pQueuePriorities = m_QueueFamilies[i].m_Priorities.data();

            queueCreateInfos.push_back(info);
        }
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    deviceFeatures.geometryShader = VK_TRUE;
    deviceFeatures.tessellationShader = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t >(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;

    //Device layers are deprecated, so we disable them
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;

    createInfo.enabledExtensionCount = static_cast<uint32_t >(m_EnabledDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_EnabledDeviceExtensions.data();


    result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device);
    if(result != VK_SUCCESS)
    {
        Console::printLine("Error when creating device.");
    }

    return result;
}

void VulkanCore::vkInitSetupQueueFamilies(const vector<VkQueueFamilyProperties> &queueFamilies) {

    for(uint32_t i = 0;  i < static_cast<uint32_t >(queueFamilies.size()); ++i)
    {
        const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

        vk_queue_family qf { queueFamily, i };

        for(uint32_t j = 0; j < queueFamily.queueCount; ++j)
            qf.m_Priorities.push_back(1.0f);

        m_QueueFamilies.push_back(qf);
    }
}

void VulkanCore::vkInitAssignQqueues() {

    m_GraphicsQueue = VK_NULL_HANDLE;
    m_AdditionalGraphicsQueues.clear();
    m_ComputeQueues.clear();
    m_SparseBindingQueues.clear();
    m_TransferOnlyQueues.clear();

    bool graphicsAssigned = false;

    for(uint32_t i = 0; i < static_cast<uint32_t >(m_QueueFamilies.size()); ++i)
    {
        vk_queue_family& qf = m_QueueFamilies[i];

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, qf.m_Index, m_Surface, &presentSupport);
            if(presentSupport == VK_FALSE)
                continue;

            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                if(!graphicsAssigned)
                {
                    //Assign as main graphics queue

                    vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_GraphicsQueue);

                    graphicsAssigned = true;
                } else
                {
                    m_AdditionalGraphicsQueues.push_back(VK_NULL_HANDLE);
                    vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_AdditionalGraphicsQueues.back());
                }
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_ComputeQueues.push_back(VK_NULL_HANDLE);
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_ComputeQueues.back());
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_SparseBindingQueues.push_back(VK_NULL_HANDLE);
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_SparseBindingQueues.back());
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT && !qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT && !qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_TransferOnlyQueues.push_back(VK_NULL_HANDLE);
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_TransferOnlyQueues.back());
            }
        }
    }
}

VkResult VulkanCore::vkInitCreateSurface() {

    VkResult result;

    result = m_Platform.createSurface(m_Instance, m_Surface);

    return result;
}

bool VulkanCore::vkInitCheckDevice(const VkPhysicalDevice deviceToCheck, const vector<const char *> &deviceExtentions, const VkSurfaceKHR surface) {

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(deviceToCheck, &deviceProperties);
    vkGetPhysicalDeviceFeatures(deviceToCheck, &deviceFeatures);

    cout << deviceProperties << std::endl;

    vk_physical_device_info deviceInfo = vkInitGetQueueFamilies(deviceToCheck);

    bool supportsGraphics = false;

    for(auto& queueFamily : deviceInfo.m_QueueFamilyProperties)
    {
        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            supportsGraphics = true;
            break;
        }
    }

    vk_swapchain_details details = fillSwapChainDetails(deviceToCheck, surface);
    if(!checkDeviceExtentions(deviceToCheck, deviceExtentions) || !checkSwapChainDetails(details))
        return false;


    return supportsGraphics && (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) && deviceFeatures.geometryShader && deviceFeatures.tessellationShader;
}

int32_t VulkanCore::vkInitSuitabilityRating(const VkPhysicalDevice deviceToRate) {

    int32_t rating = 0;

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(deviceToRate, &deviceProperties);
    vkGetPhysicalDeviceFeatures(deviceToRate, &deviceFeatures);

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        rating += 1000;

    rating += deviceProperties.limits.maxImageDimension2D;

    return rating;
}

vk_physical_device_info VulkanCore::vkInitGetQueueFamilies(const VkPhysicalDevice device) {

    uint32_t queueFamilyCount = 0;
    vector<VkQueueFamilyProperties> queueFamilies;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    if(queueFamilyCount == 0)
    {
        throw std::runtime_error("No device queue families found!");
    }

    queueFamilies.resize(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for(const auto& queueFamily : queueFamilies)
    {
        cout << queueFamily << std::endl;
    }

    vk_physical_device_info info;
    info.m_PhysicalDevice = device;
    info.m_QueueFamilyProperties = queueFamilies;
    return info;
}

VkResult VulkanCore::vkInitCreateSwapchain() {

    m_SwapChainImages.clear();

    vk_swapchain_details swapchainDetails = fillSwapChainDetails(m_PhysicalDevice, m_Surface);

    VkSurfaceFormatKHR surfaceFormatKHR = pickSwapChainSurfaceFormat(swapchainDetails);
    VkPresentModeKHR presentModeKHR = pickSwapChainPresentMode(swapchainDetails);
    VkExtent2D extent2D = pickSwapChainExtent2D(swapchainDetails, WIDTH, HEIGHT);

    uint32_t imageCount = swapchainDetails.m_capabilities.minImageCount + 1;
    if(swapchainDetails.m_capabilities.maxImageCount > 0 && imageCount > swapchainDetails.m_capabilities.maxImageCount)
    {
        imageCount = swapchainDetails.m_capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfoKHR          = {};
    createInfoKHR.sType                             = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfoKHR.pNext                             = nullptr;
    createInfoKHR.surface                           = m_Surface;
    createInfoKHR.minImageCount                     = imageCount;
    createInfoKHR.imageFormat                       = surfaceFormatKHR.format;
    createInfoKHR.imageColorSpace                   = surfaceFormatKHR.colorSpace;
    createInfoKHR.imageExtent                       = extent2D;
    //Engine does not support stereoscopic 3d as of yet so 1 image array layer
    createInfoKHR.imageArrayLayers                  = 1;
    createInfoKHR.imageUsage                        = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    //Currently presentation queue and graphics queue are the same queues, so exclusive mode is okay
    createInfoKHR.imageSharingMode                  = VK_SHARING_MODE_EXCLUSIVE;
    createInfoKHR.queueFamilyIndexCount             = 0;
    createInfoKHR.pQueueFamilyIndices               = nullptr;

    //No pretransformation
    createInfoKHR.preTransform                      = swapchainDetails.m_capabilities.currentTransform;
    //Ignore alpha blending with the windowing system, well, for obvious reasons i guess :)
    createInfoKHR.compositeAlpha                    = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfoKHR.presentMode                       = presentModeKHR;
    createInfoKHR.clipped                           = VK_TRUE;

    //TODO: Set up swapchain stuff here in a proper way for later when resizing windows and whatnot
    if(m_Swapchain != VK_NULL_HANDLE)
        createInfoKHR.oldSwapchain = m_Swapchain;
    else
        createInfoKHR.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(m_Device, &createInfoKHR, nullptr, &m_Swapchain);
    if(result != VK_SUCCESS)
        throw std::runtime_error("Swapchain creation failed.");

    uint32_t swapImageCount = 0;
    vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapImageCount, nullptr);

    m_SwapChainImages.resize(swapImageCount);
    vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapImageCount, m_SwapChainImages.data());

    m_SwapChainExtent = extent2D;
    m_SwapChainFormat = surfaceFormatKHR.format;

    return result;
}

VkResult VulkanCore::vkInitCreateSwapchainImageViews() {

    VkResult result = VK_ERROR_INITIALIZATION_FAILED;

    cleanUpSwapchainImageViews();
    m_SwapChainImageViews.resize(m_SwapChainImages.size());

    //Default rgba color channel set
    VkComponentMapping componentMapping {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};

    //Default sub resource mapping, as color buffer
    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.baseMipLevel   = 0;
    subresourceRange.layerCount     = 1;
    subresourceRange.levelCount     = 1;



    for(uint32_t i = 0; i < static_cast<uint32_t >(m_SwapChainImages.size()); ++i)
    {

        VkImageViewCreateInfo createInfo    = {};
        createInfo.sType                    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext                    = nullptr;
        createInfo.image                    = m_SwapChainImages[i];
        createInfo.components               = componentMapping;
        createInfo.subresourceRange         = subresourceRange;
        createInfo.viewType                 = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                   = m_SwapChainFormat;

        result = vkCreateImageView(m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]);
        VK_IF_FAIL_MSG(result, "Error when creating image view.");
    }

    return result;
}


































