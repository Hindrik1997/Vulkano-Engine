//
// Created by hindrik on 25-3-17.
//

#include <cstring>
#include <algorithm>
#include "Instance.h"
#include <algorithm>
#include <shared_mutex>
#include "../../Core/Engine.h"
#include "../../Core/Logger.h"

Instance::Instance(vk_core_create_info createInfo, Engine* engine) : m_Engine(engine)
{
    vkSetupInit(createInfo);
}

Instance::~Instance()
{
    cleanUpDebugFacilities();
}


auto Instance::vkSetupInit(vk_core_create_info &createInfo) -> void
{
    checkLayersAndInstanceExtensionsSupport(createInfo);
    vkInit(createInfo);
    Logger::succes("Initialized Vulkan API succesfully.");

    m_IsDebugEnabled = createInfo.m_EnableDebugLayers;
    if(m_IsDebugEnabled)
        setupDebugFacilities();
}

auto Instance::vkInit(vk_core_create_info createInfo) -> void
{
    vkInitInstance(createInfo);

    //Enable debug if necessary
    if(m_IsDebugEnabled)
    {
        setupDebugFacilities();
    }
}

auto Instance::vkInitInstance(vk_core_create_info createInfo) -> void
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




auto Instance::setupDebugFacilities() -> void
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

auto Instance::cleanUpDebugFacilities() -> void
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

auto Instance::checkLayersAndInstanceExtensionsSupport(vk_core_create_info createInfo) -> void {

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

auto Instance::isValidationLayerSupported(const char *name) -> bool
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

auto Instance::isInstanceExtensionSupported(const char *name) -> bool
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

auto Instance::enumerateValidationLayers() -> vector<vk_layer_extension_properties>
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

auto Instance::enumerateInstanceExtensions() -> vector<VkExtensionProperties>
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

auto Instance::instance() const -> const VkInstance
{
    return m_Instance;
}



