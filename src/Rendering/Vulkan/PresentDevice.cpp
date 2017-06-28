//
// Created by hindr on 23-6-2017.
//

#include "PresentDevice.h"
#include "../Utilities/VulkanUtilityFunctions.h"

PresentDevice::PresentDevice(vk_present_device_create_info createInfo) : m_Instance(createInfo.m_Instance)
{
    vkInitPhysicalDevice(createInfo);
    vkInitLogicalDevice(createInfo);
}

PresentDevice::~PresentDevice()
{
    vkDeviceWaitIdle(m_Device);
}

VkDevice PresentDevice::device() const
{
    return m_Device;
}

VkPhysicalDevice PresentDevice::physicalDevice() const
{
    return m_PhysicalDevice;
}

vk_queue PresentDevice::presentQueue() const
{
    return m_PresentQueue;
}

const vector<vk_queue_set>& PresentDevice::asyncComputeQueues() const
{
    return m_AsyncComputeQueues;
}

const vector<vk_queue_set>& PresentDevice::transferQueues() const
{
    return m_TransferQueues;
}

auto PresentDevice::vkInitPhysicalDevice(vk_present_device_create_info createInfo) -> void
{

    VkResult                    result;

    uint32_t                    physicalDeviceCount     = 0;
    vector<VkPhysicalDevice>    physicalDevices         = {};
    VkPhysicalDevice            selectedDevice          = VK_NULL_HANDLE;


    result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, nullptr);
    vkIfFailThrowMessage(result, "Error when retrieving amount of physical devices.");

    if(physicalDeviceCount == 0) {
        Logger::failure("Error, no physical devices supporting Vulkan found!.");
    }

    physicalDevices.resize(physicalDeviceCount);

    result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, physicalDevices.data());
    vkIfFailThrowMessage(result, "Error when retrieving physical devices.");

    vector<VkPhysicalDevice> possibleGPUs;

    if(createInfo.m_EnumeratePossibleDevicesInConsole)
    {
        Logger::log("Possible GPU's found: ");
    }
    for(const auto& device : physicalDevices)
    {
        if(checkDevice(device, createInfo))
        {
            possibleGPUs.push_back(device);

            if(!createInfo.m_EnumeratePossibleDevicesInConsole)
                continue;

            VkPhysicalDeviceProperties  deviceProperties;

            vkGetPhysicalDeviceProperties(device, &deviceProperties);

            std::stringstream str;
            str << std::endl << deviceProperties;
            Logger::log(str.str());

        }
    }

    if(possibleGPUs.size() == 0)
    {
        Logger::failure("Failed to find a suitable GPU. The GPU needs to be a discrete or integrated GPU, and needs to support geometry shaders and tesselation.");
    }

    vector<vk_device_rating> ratings;



    for(const auto& device : possibleGPUs)
    {
        ratings.push_back(vk_device_rating{device, rateDeviceSuitability(device)});
    }

    std::sort(ratings.begin(), ratings.end(), [](const vk_device_rating& left, const vk_device_rating& right){ return right.m_Rating < left.m_Rating; });

    selectedDevice = ratings.front().m_PhysicalDevice;

    if(selectedDevice == VK_NULL_HANDLE)
    {
        Logger::failure("Selected device is a VK_NULL_HANDLE! ");
    }

    m_PhysicalDevice = selectedDevice;


    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(selectedDevice, &props);

    std::stringstream str;
    str << "Selected device: " << std::endl << props;
    Logger::logNoEndl(str.str());


    if(createInfo.m_EnumerateLayersAndExtensionsInConsole)
    {
        vector<VkExtensionProperties> extensionProperties = enumerateDeviceExtensions(m_PhysicalDevice);

        Logger::log("Found " + std::to_string(extensionProperties.size()) + " device extensions:");

        for(uint32_t i = 0; i < static_cast<uint32_t >(extensionProperties.size()); ++i)
        {
            std::stringstream str1; str1 << std::endl << extensionProperties[i] << std::endl;
            Logger::logNoEndl(str1.str());
        }
    }
}

auto PresentDevice::vkInitLogicalDevice(vk_present_device_create_info createInfo) -> void {

    VkResult result;

    vk_queue_family_selected_set selectedSet = vkInitSetupQueueFamilies(createInfo);

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    {

        VkDeviceQueueCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.pNext = nullptr;
        info.queueCount = static_cast<uint32_t >(selectedSet.presentQueue.m_Priorities.size());
        info.queueFamilyIndex = selectedSet.presentQueue.m_FamilyIndex;
        info.pQueuePriorities = selectedSet.presentQueue.m_Priorities.data();

        queueCreateInfos.push_back(info);
    }

    for(const auto& queueFamily : selectedSet.computeFamilies)
    {

        VkDeviceQueueCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.pNext = nullptr;
        info.queueCount = static_cast<uint32_t >(queueFamily.m_Priorities.size());
        info.queueFamilyIndex = queueFamily.m_FamilyIndex;
        info.pQueuePriorities = queueFamily.m_Priorities.data();

        queueCreateInfos.push_back(info);
    }

    for(const auto& queueFamily : selectedSet.transferFamlies)
    {

        VkDeviceQueueCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.pNext = nullptr;
        info.queueCount = static_cast<uint32_t >(queueFamily.m_Priorities.size());
        info.queueFamilyIndex = queueFamily.m_FamilyIndex;
        info.pQueuePriorities = queueFamily.m_Priorities.data();

        queueCreateInfos.push_back(info);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    deviceFeatures.geometryShader = VK_TRUE;
    deviceFeatures.tessellationShader = VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t >(queueCreateInfos.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    //PresentDevice layers are deprecated, so we disable them
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t >(createInfo.m_EnabledDeviceExtentionNames.size());
    deviceCreateInfo.ppEnabledExtensionNames = createInfo.m_EnabledDeviceExtentionNames.data();


    result = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, m_Device.reset());
    vkIfFailThrowMessage(result, "Error creating device.");

    vkInitSetupQueues(selectedSet);
}


auto PresentDevice::checkDeviceExtensions(VkPhysicalDevice device, vector<const char *> extensionNames) -> bool
{
    for(uint32_t i = 0; i < static_cast<uint32_t >(extensionNames.size()); ++i)
    {
        bool isSupported;
        isSupported = isDeviceExtensionSupported(extensionNames[i], device);
        if(!isSupported)
            return false;
    }
    return true;
}

auto PresentDevice::checkDevice(VkPhysicalDevice device, vk_present_device_create_info createInfo) -> bool
{

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    vk_physical_device_queue_family_info deviceInfo = getDeviceQueueFamilies(device);

    bool supportsGraphics = false;

    for(uint32_t index = 0; index < static_cast<uint32_t >(deviceInfo.m_QueueFamilyProperties.size()); ++index)
    {
        auto& queueFamily = deviceInfo.m_QueueFamilyProperties[index];
        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            supportsGraphics = true;
            break;
        }
    }

    uint32_t queueFamilyCount = 0;
    vector<VkQueueFamilyProperties> queueFamilies;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    queueFamilies.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount, queueFamilies.data());

    bool supportsSurface = false;

    for(uint32_t i = 0; i < static_cast<uint32_t >(queueFamilies.size()); ++i)
    {
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, createInfo.m_Surface, &presentSupport);
        if(presentSupport == VK_FALSE)
            continue;
        else
        {
            supportsSurface = true;
            break;
        }
    }

    if (!checkDeviceExtensions(device, createInfo.m_EnabledDeviceExtentionNames)) {
        return false;
    }
    return supportsGraphics && supportsSurface && (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) && deviceFeatures.geometryShader && deviceFeatures.tessellationShader;
}

auto PresentDevice::rateDeviceSuitability(VkPhysicalDevice device) -> int32_t
{

    int32_t rating = 0;

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        rating += 1000;

    rating += deviceProperties.limits.maxImageDimension2D;

    return rating;
}

auto PresentDevice::getDeviceQueueFamilies(VkPhysicalDevice device) -> vk_physical_device_queue_family_info
{

    uint32_t queueFamilyCount = 0;
    vector<VkQueueFamilyProperties> queueFamilies;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    if(queueFamilyCount == 0)
    {
        Logger::failure("No device queue families found.");
    }

    queueFamilies.resize(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    vk_physical_device_queue_family_info info;
    info.m_PhysicalDevice = device;
    info.m_QueueFamilyProperties = queueFamilies;
    return info;
}

auto PresentDevice::enumerateDeviceExtensions(VkPhysicalDevice device) -> vector<VkExtensionProperties>
{

    vector<VkExtensionProperties> extensionProperties;
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    if(extensionCount == 0)
        return extensionProperties;

    extensionProperties.resize(extensionCount);

    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensionProperties.data());

    return extensionProperties;
}

auto PresentDevice::isDeviceExtensionSupported(const char *name, VkPhysicalDevice device) -> bool
{

    vector<VkExtensionProperties> props = enumerateDeviceExtensions(device);

    for(VkExtensionProperties& prop : props)
    {
        if(strcmp(prop.extensionName, name) == 0)
        {
            return true;
        }
    }
    Logger::error("PresentDevice extension not supported: ");
    Logger::log(name);
    return false;
}



vk_queue_family_selected_set PresentDevice::vkInitSetupQueueFamilies(vk_present_device_create_info createInfo)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    vector<vk_queue_family_properties> graphicsAndCompute;
    vector<vk_queue_family_properties> computeOnly;
    vector<vk_queue_family_properties> transferOnly;

    //assign the queue families in the proper sets.
    uint32_t index = 0;
    for(auto& fam : queueFamilies)
    {
        if(fam.queueCount > 0 && fam.queueFlags & VK_QUEUE_GRAPHICS_BIT && fam.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            graphicsAndCompute.push_back(vk_queue_family_properties { fam, index });
            index++;
            continue;
        }
        if(fam.queueCount > 0 && fam.queueFlags & VK_QUEUE_COMPUTE_BIT && !(fam.queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            computeOnly.push_back(vk_queue_family_properties { fam, index });
            index++;
            continue;
        }
        if(fam.queueFlags > 0 && fam.queueFlags & VK_QUEUE_TRANSFER_BIT && !(fam.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(fam.queueFlags & VK_QUEUE_COMPUTE_BIT))
        {
            transferOnly.push_back(vk_queue_family_properties { fam, index });
            index++;
            continue;
        }
        Logger::warn("Did not find an appropriate type set for a queue family. ");
        index++;
    }

    vk_queue_family presentQueue            = {};
    vector<vk_queue_family> computeFamilies = {};
    vector<vk_queue_family> transferFamlies = {};


    //get present family and check present support.
    for(const auto& fam : graphicsAndCompute)
    {
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, fam.m_Index, createInfo.m_Surface, &presentSupport);
        if(presentSupport == VK_FALSE)
            continue;
        else
        {
            vk_queue_family qf;

            qf.m_FamilyIndex = fam.m_Index;
            qf.m_QueueFamilyProperties = fam.m_QueueFamilyProperties;

            qf.m_Priorities.push_back(1.0f);
            qf.m_QueueIndices.push_back(0);
            presentQueue = qf;
            break;
        }
    }

    //get the async compute families
    for(const auto& fam : computeOnly)
    {
        vk_queue_family qf;
        qf.m_FamilyIndex = fam.m_Index;
        qf.m_QueueFamilyProperties = fam.m_QueueFamilyProperties;

        for(uint32_t i = 0; i < fam.m_QueueFamilyProperties.queueCount; ++i)
        {
            qf.m_Priorities.push_back(1.0f);
            qf.m_QueueIndices.push_back(i);
        }
        computeFamilies.push_back(qf);
    }

    //get the transfer queue families
    for(const auto& fam : transferOnly)
    {
        vk_queue_family qf;
        qf.m_FamilyIndex = fam.m_Index;
        qf.m_QueueFamilyProperties = fam.m_QueueFamilyProperties;

        for(uint32_t i = 0; i < fam.m_QueueFamilyProperties.queueCount; ++i)
        {
            qf.m_Priorities.push_back(1.0f);
            qf.m_QueueIndices.push_back(i);
        }
        transferFamlies.push_back(qf);
    }

    return vk_queue_family_selected_set { presentQueue, computeFamilies, transferFamlies };

}



auto PresentDevice::vkInitSetupQueues(vk_queue_family_selected_set& selectedQueueSet) -> void
{
    m_PresentQueue.m_FamilyIndex = selectedQueueSet.presentQueue.m_FamilyIndex;
    vkGetDeviceQueue(m_Device, selectedQueueSet.presentQueue.m_FamilyIndex, 0, &m_PresentQueue.m_Queue);

    for(const auto& qf : selectedQueueSet.computeFamilies)
    {
        vk_queue_set set = {};
        set.m_FamilyIndex = qf.m_FamilyIndex;

        for(const auto& q : qf.m_QueueIndices)
        {
            VkQueue queue;
            vkGetDeviceQueue(m_Device, qf.m_FamilyIndex,q,&queue);
            set.m_Queues.push_back(queue);
        }
        m_AsyncComputeQueues.push_back(set);
    }

    for(const auto& qf : selectedQueueSet.transferFamlies)
    {
        vk_queue_set set = {};
        set.m_FamilyIndex = qf.m_FamilyIndex;

        for(const auto& q : qf.m_QueueIndices)
        {
            VkQueue queue;
            vkGetDeviceQueue(m_Device, qf.m_FamilyIndex,q,&queue);
            set.m_Queues.push_back(queue);
        }
        m_TransferQueues.push_back(set);
    }
}




