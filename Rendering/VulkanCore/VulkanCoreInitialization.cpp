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
    VK_CHECK(result);

    //Enable debug if necessary
    if(m_IsDebugEnabled)
    {
        setupDebugFacilities();
    }

    result = vkInitPhysicalDevice();
    VK_CHECK(result);

    result = vkInitLogicalDevice();
    VK_CHECK(result);

    result = vkInitCreateSurface();
    VK_CHECK(result);

    vkInitAssignQqueues();


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
        if(vkInitCheckDevice(device, m_EnabledDeviceExtensions))
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

bool VulkanCore::vkInitCheckDevice(const VkPhysicalDevice deviceToCheck, const vector<const char *> &deviceExtentions) {

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

    if(!checkDeviceExtentions(deviceToCheck, deviceExtentions))
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