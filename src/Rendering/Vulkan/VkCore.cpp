//
// Created by hindrik on 25-3-17.
//

#include <cstring>
#include <algorithm>
#include "VkCore.h"
#include <algorithm>
#include <shared_mutex>
#include "../../Core/Engine.h"
#include "../../Core/Logger.h"

VkCore::VkCore(vk_core_create_info createInfo, Engine& engine) : m_Engine(engine)
{
    checkLayersAndInstanceExtensionsSupport(createInfo);
    vkInit(createInfo);
    Logger::succes("Initialized Vulkan API succesfully.");

    m_IsDebugEnabled = createInfo.m_EnableDebugLayers;
    if(m_IsDebugEnabled)
        setupDebugFacilities();
}

VkCore::~VkCore()
{
    vkDeviceWaitIdle(m_Device);
    cleanUpDebugFacilities();
}

auto VkCore::vkInit(vk_core_create_info createInfo) -> void
{
    vkInitInstance(createInfo);

    //Enable debug if necessary
    if(m_IsDebugEnabled)
    {
        setupDebugFacilities();
    }
    vkInitPhysicalDevice(createInfo);
    vkInitLogicalDevice(createInfo);
    vkInitAssignQueues();
    vkInitSetupTransferFacilities();
}

auto VkCore::vkInitInstance(vk_core_create_info createInfo) -> void
{

    VkResult result;

    VkApplicationInfo applicationInfo                   = {};

    applicationInfo.sType                               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext                               = nullptr;
    applicationInfo.pApplicationName                    = createInfo.m_ApplicationName;
    applicationInfo.applicationVersion                  = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName                         = "Vulkano Engine";
    applicationInfo.engineVersion                       = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion                          = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo             = {};
    instanceCreateInfo.sType                            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                            = nullptr;
    instanceCreateInfo.flags                            = 0;
    instanceCreateInfo.pApplicationInfo                 = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount            = static_cast<uint32_t >(createInfo.m_EnabledInstanceExtensionNames.size());
    instanceCreateInfo.ppEnabledExtensionNames          = createInfo.m_EnabledInstanceExtensionNames.data();
    instanceCreateInfo.enabledLayerCount                = static_cast<uint32_t >(createInfo.m_EnabledInstanceValidationLayerNames.size());
    instanceCreateInfo.ppEnabledLayerNames              = createInfo.m_EnabledInstanceValidationLayerNames.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, m_Instance.reset());

    vkIfFailThrowMessage(result, "Error creating VkInstance.");
}

auto VkCore::vkInitPhysicalDevice(vk_core_create_info createInfo) -> void
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
        if(checkDevice(device, createInfo.m_EnabledDeviceExtentionNames))
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

    m_PhysicalDeviceInfo = getDeviceQueueFamilies(selectedDevice);



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

auto VkCore::vkInitLogicalDevice(vk_core_create_info createInfo) -> void
{

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

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t >(queueCreateInfos.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    //Device layers are deprecated, so we disable them
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t >(createInfo.m_EnabledDeviceExtentionNames.size());
    deviceCreateInfo.ppEnabledExtensionNames = createInfo.m_EnabledDeviceExtentionNames.data();


    result = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, m_Device.reset());
    vkIfFailThrowMessage(result, "Error creating device.");
}

auto VkCore::vkInitSetupQueueFamilies(const vector<VkQueueFamilyProperties> &queueFamilies) -> void
{

    for(uint32_t i = 0;  i < static_cast<uint32_t >(queueFamilies.size()); ++i)
    {
        const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

        vk_queue_family qf { queueFamily, i };

        for(uint32_t j = 0; j < queueFamily.queueCount; ++j)
            qf.m_Priorities.push_back(1.0f);

        m_QueueFamilies.push_back(qf);
    }
}

auto VkCore::vkInitAssignQueues() -> void
{
    m_GraphicsQueues.clear();
    m_ComputeQueues.clear();
    m_SparseBindingQueues.clear();
    m_TransferOnlyQueues.clear();

    for(uint32_t i = 0; i < static_cast<uint32_t >(m_QueueFamilies.size()); ++i)
    {
        vk_queue_family& qf = m_QueueFamilies[i];

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_GraphicsQueues.push_back(vk_queue{});
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_GraphicsQueues.back().m_Queue);
                m_GraphicsQueues.back().m_FamilyIndex = qf.m_Index;
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_ComputeQueues.push_back(vk_queue{});
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_ComputeQueues.back().m_Queue);
                m_ComputeQueues.back().m_FamilyIndex = qf.m_Index;
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_SparseBindingQueues.push_back(vk_queue{});
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_SparseBindingQueues.back().m_Queue);
                m_SparseBindingQueues.back().m_FamilyIndex = qf.m_Index;
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT && !(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT))
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_TransferOnlyQueues.push_back(vk_queue{});
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_TransferOnlyQueues.back().m_Queue);
                m_TransferOnlyQueues.back().m_FamilyIndex = qf.m_Index;
            }
        }
    }
}

auto VkCore::setupDebugFacilities() -> void
{
    if(!m_IsDebugEnabled)
    {
        Logger::failure("Enabling debug facilities while the debug layers are not enabled.");
    }

    VkDebugReportCallbackCreateInfoEXT createInfo = {};


    createInfo.sType                = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.pNext                = NULL;
    createInfo.flags                =
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT;
    createInfo.pfnCallback          = debugCallback;
    createInfo.pUserData            = NULL;

    if (createDebugReportCallbackEXT(m_Instance, &createInfo, nullptr, &m_DebugCallback) != VK_SUCCESS) {
        Logger::failure("Failed to set up debug callback!");
    }
    Logger::succes("Succesfully set up the debug facilities!");
}

auto VkCore::cleanUpDebugFacilities() -> void
{
    if(m_IsDebugEnabled)
    {
        VkResult result = destroyDebugReportCallbackEXT(m_Instance, m_DebugCallback, NULL);
        if(result  != VK_SUCCESS)
            Logger::error("Problem when destroying debug reporter callback function.");
        else
            Logger::log("Succefully cleaned up debug facilities!");
    }
}

VKAPI_ATTR auto VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj,
                                         size_t location, int32_t code, const char *layerPrefix, const char *msg,
                                                                                        void *userData) -> VkBool32
{
    std::stringstream str;
    str << "Validation layer: " << msg << std::endl;
    Logger::log(str.str());
    return VK_FALSE;

}

auto createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator,
                                                                VkDebugReportCallbackEXT *pCallback) -> VkResult
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
auto destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks *pAllocator) -> VkResult
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
        return VK_SUCCESS;
    } else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

auto VkCore::checkLayersAndInstanceExtensionsSupport(vk_core_create_info createInfo) -> void {

    if(createInfo.m_EnumerateLayersAndExtensionsInConsole)
    {
        vector<VkExtensionProperties> instanceExtensions = enumerateInstanceExtensions();
        vector<vk_layer_extension_properties> layerProperties = enumerateValidationLayers();

        Logger::log("Found " + std::to_string(instanceExtensions.size()) + " KHR Extensions");

        for(VkExtensionProperties ep : instanceExtensions)
        {
            std::stringstream str;
            str << std::endl << ep;
            Logger::logNoEndl(str.str());
        }

        std::stringstream str;
        str << "Found " << layerProperties.size() << " Extension Layers" << std::endl;
        Logger::log(str.str());

        for(vk_layer_extension_properties ep : layerProperties)
        {
            std::stringstream str1;
            str1 << std::endl << ep;
            Logger::logNoEndl(str1.str());
        }
    }

    for(const char* name : createInfo.m_EnabledInstanceExtensionNames)
    {
        if(!isInstanceExtensionSupported(name))
        {
            Logger::failure("Instance Extension not supported!");
        }
    }

    for(const char* name : createInfo.m_EnabledInstanceValidationLayerNames)
    {
        if(!isValidationLayerSupported(name))
        {
            Logger::failure("Validation layer not supported!");
        }
    }

    if(createInfo.m_EnableDebugLayers)
    {
        bool debugExtensionFound = false;
        for(const char* name : createInfo.m_EnabledInstanceExtensionNames)
        {
            if(strcmp(name, VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0)
            {
                debugExtensionFound = true;
                break;
            }
        }

        if(!debugExtensionFound)
        {
            Logger::failure("KHR Debug Extension is required when using the validation layers. Please enable it.");
        }
    }
}

auto VkCore::isValidationLayerSupported(const char *name) -> bool
{
    vector<vk_layer_extension_properties> layers = enumerateValidationLayers();
    for(vk_layer_extension_properties& prop : layers)
    {
        if(strcmp(prop.m_layerProperties.layerName, name) == 0)
        {
            return true;
        }
    }
    Logger::error("Validation layer not supported: ");
    Logger::log(name);
    return false;
}

auto VkCore::isInstanceExtensionSupported(const char *name) -> bool
{
    vector<VkExtensionProperties> properties = enumerateInstanceExtensions();
    for(VkExtensionProperties& prop : properties)
    {
        if(strcmp(prop.extensionName, name) == 0)
        {
            return true;
        }
    }
    Logger::error("Instance extension not supported: ");
    Logger::log(name);
    return false;
}

auto VkCore::isDeviceExtensionSupported(const char *name, VkPhysicalDevice device) -> bool
{

    vector<VkExtensionProperties> props = enumerateDeviceExtensions(device);

    for(VkExtensionProperties& prop : props)
    {
        if(strcmp(prop.extensionName, name) == 0)
        {
            return true;
        }
    }
    Logger::error("Device extension not supported: ");
    Logger::log(name);
    return false;
}

auto VkCore::enumerateDeviceExtensions(VkPhysicalDevice device) -> vector<VkExtensionProperties>
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

auto VkCore::enumerateValidationLayers() -> vector<vk_layer_extension_properties>
{

    VkResult result;
    uint32_t instanceLayerCount;
    vector<VkLayerProperties> instanceLayerProperties;
    vector<vk_layer_extension_properties>   instanceLayersAndExtentions                   = {};

    result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);

    vkIfFailThrowMessage(result, "Error occurred when fetching instance layer count");

    instanceLayerProperties.resize(instanceLayerCount);

    instanceLayersAndExtentions.resize(instanceLayerCount);

    result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());

    vkIfFailThrowMessage(result, "Error occurred when fetching the instance layer properties");

    for(uint32_t i = 0; i < instanceLayerProperties.size(); ++i)
    {
        VkLayerProperties& layerProperty = instanceLayerProperties[i];
        uint32_t extensionCount;
        vector<VkExtensionProperties> extensionProperties;

        result = vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &extensionCount, nullptr);

        vkIfFailThrowMessage(result, "Error occurred when fetching the instance layer extension count");

        extensionProperties.resize(extensionCount);

        result = vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &extensionCount, extensionProperties.data());

        vkIfFailThrowMessage(result, "Error occurred when fetching the instance layer extension properties");

        vk_layer_extension_properties layerProps;

        layerProps.m_layerProperties        = layerProperty;
        layerProps.m_ExtensionProperties    = extensionProperties;

        instanceLayersAndExtentions[i]    = (layerProps);
    }

    return instanceLayersAndExtentions;
}

auto VkCore::enumerateInstanceExtensions() -> vector<VkExtensionProperties>
{
    VkResult result;
    uint32_t extensionCount;

    vector<VkExtensionProperties> instanceExtensions;

    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    vkIfFailThrowMessage(result, "Error occurred when fetching the instance KHR extension count");

    instanceExtensions.resize(extensionCount);

    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, instanceExtensions.data());

    vkIfFailThrowMessage(result, "Error occurred when fetching the instance KHR extension properties");

    return instanceExtensions;
}

auto VkCore::checkDeviceExtensions(VkPhysicalDevice device, vector<const char *> extensionNames) -> bool
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

auto VkCore::checkDevice(VkPhysicalDevice device, const vector<const char *> &deviceExtentions) -> bool
{

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    vk_physical_device_info deviceInfo = getDeviceQueueFamilies(device);

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

    if (!checkDeviceExtensions(device, deviceExtentions)) {
        return false;
    }
    return supportsGraphics && (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) && deviceFeatures.geometryShader && deviceFeatures.tessellationShader;
}

auto VkCore::rateDeviceSuitability(VkPhysicalDevice device) -> int32_t
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

auto VkCore::getDeviceQueueFamilies(VkPhysicalDevice device) -> vk_physical_device_info
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

    vk_physical_device_info info;
    info.m_PhysicalDevice = device;
    info.m_QueueFamilyProperties = queueFamilies;
    return info;
}

auto VkCore::physicalDeviceInfo() const -> const vk_physical_device_info
{
    return m_PhysicalDeviceInfo;
}

auto VkCore::physicalDevice() const -> const VkPhysicalDevice
{
    return m_PhysicalDevice;
}

auto VkCore::device() const -> const VkDevice
{
    return m_Device;
}

auto VkCore::instance() const -> const VkInstance
{
    return m_Instance;
}

auto VkCore::graphicsQueues() const -> const vector<vk_queue>&
{
    return m_GraphicsQueues;
}

auto VkCore::computeQueues() const -> const vector<vk_queue>&
{
    return m_ComputeQueues;
}

auto VkCore::transferOnlyQueues() const -> const vector<vk_queue>&
{
    return m_TransferOnlyQueues;
}

auto VkCore::sparseBindingQueues() const -> const vector<vk_queue>&
{
    return m_SparseBindingQueues;
}

auto VkCore::vkInitSetupTransferFacilities() -> void
{
    m_TransferQueueFamilies.clear();
    for(const auto& q : m_TransferOnlyQueues)
    {
        if(std::find(m_TransferQueueFamilies.begin(), m_TransferQueueFamilies.end(), q.m_FamilyIndex) == m_TransferQueueFamilies.end())
            m_TransferQueueFamilies.push_back(q.m_FamilyIndex);
    }

    for(const auto& q : m_TransferQueueFamilies)
    {
        m_TransferCommandPools.emplace_back(m_Device, q, true, false);
    }
}

auto VkCore::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset) -> void
{
    VkCommandBuffer buffer = m_TransferCommandPools[0].allocateCommandBuffer(CommandBufferLevel::Primary);

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = 0;

    VkUniqueHandle<VkFence> fence = {m_Device, vkDestroyFence};
    vkCreateFence(m_Device, &fenceCreateInfo, nullptr, fence.reset());

    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buffer, &info);

    VkBufferCopy copy = {};
    copy.srcOffset = srcOffset;
    copy.dstOffset = dstOffset;
    copy.size = size;

    vkCmdCopyBuffer(buffer, src, dst, 1, &copy);

    vkEndCommandBuffer(buffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;

    vkQueueSubmit(m_TransferOnlyQueues[0].m_Queue, 1, &submitInfo, fence);

    //Wait for transfer operation to be completed...
    vkWaitForFences(m_Device, 1, &fence, VK_TRUE, UINT64_MAX);

    m_TransferCommandPools[0].deallocateCommandBuffer(buffer);
}

auto VkCore::transferQueueFamilies() const -> const vector<uint32_t> &
{
    return m_TransferQueueFamilies;
}

auto VkCore::transitionImage(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer buffer = m_TransferCommandPools[0].allocateCommandBuffer(CommandBufferLevel::Primary);





}
