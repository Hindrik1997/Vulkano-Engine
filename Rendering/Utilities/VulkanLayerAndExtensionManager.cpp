//
// Created by hindrik on 2-2-17.
//

#include <cstring>
#include "VulkanLayerAndExtensionManager.h"
#include "../../Core/Console.h"
#include "VulkanLayerAndExtensionManagerDebug.h"


VkResult VulkanLayerAndExtensionManager::vk_enumerate_extension_layers_and_extensions() {

    VkResult result;
    uint32_t instanceLayerCount;
    vector<VkLayerProperties> instanceLayerProperties;

    result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);

    VK_IF_FAIL_MSG(result, "Error occurred when fetching instance layer count")

    instanceLayerProperties.resize(instanceLayerCount);

    m_instanceLayersAndExtentions.resize(instanceLayerCount);

    result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());

    VK_IF_FAIL_MSG(result, "Error occurred when fetching the instance layer properties")

    for(uint32_t i = 0; i < instanceLayerProperties.size(); ++i)
    {
        VkLayerProperties& layerProperty = instanceLayerProperties[i];
        uint32_t extensionCount;
        vector<VkExtensionProperties> extensionProperties;

        result = vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &extensionCount, nullptr);

        VK_IF_FAIL_MSG(result, "Error occurred when fetching the instance layer extension count")

        extensionProperties.resize(extensionCount);

        result = vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &extensionCount, extensionProperties.data());

        VK_IF_FAIL_MSG(result, "Error occurred when fetching the instance layer extension properties")

        vk_layer_extension_properties layerProps;

        layerProps.m_layer_properties       = layerProperty;
        layerProps.m_extensions_properties  = extensionProperties;

        m_instanceLayersAndExtentions[i] = (layerProps);
    }

    std::cout << "Found " << m_instanceLayersAndExtentions.size() << " Extension Layers" << std::endl;

    for(vk_layer_extension_properties ep : m_instanceLayersAndExtentions)
    {
        std::cout << ep;
    }

    return result;
}

VkResult VulkanLayerAndExtensionManager::vk_enumerate_KHR_extensions() {
    VkResult result;
    uint32_t extensionCount;

    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    VK_IF_FAIL_MSG(result, "Error occurred when fetching the instance KHR extension count")

    m_instanceKHRExtensions.resize(extensionCount);

    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_instanceKHRExtensions.data());

    VK_IF_FAIL_MSG(result, "Error occurred when fetching the instance KHR extension properties")

    Console::printLine("Found " + std::to_string(m_instanceKHRExtensions.size()) + " KHR Extensions");

    for(VkExtensionProperties ep : m_instanceKHRExtensions)
    {
        cout << ep;
    }

    return result;
}

VulkanLayerAndExtensionManager::VulkanLayerAndExtensionManager(vector<const char*> enabledInstanceKHRExtensionNames, vector<const char*> enabledInstanceValidationLayerNames, bool enableValidationLayers)
        : m_enabledInstanceKHRExtensionNames(enabledInstanceKHRExtensionNames),m_enabledInstanceValidationLayerNames(enabledInstanceValidationLayerNames), m_validationLayersEnabled(enableValidationLayers)
{
    VkResult result;

    result = vk_enumerate_KHR_extensions();

    if(result != VK_SUCCESS)
        throw std::runtime_error("Error enumerating KHR extensions!");

    result = vk_enumerate_extension_layers_and_extensions();

    if(result != VK_SUCCESS)
        throw std::runtime_error("Error enumerating validation layers and extensions!");


    for(const char* name : m_enabledInstanceKHRExtensionNames)
    {
        if(!isKHRExtensionSupported(name))
        {
            throw std::runtime_error("KHR Extension not supported!");
        }
    }

    if(enableValidationLayers)
    {
        for(const char* name : m_enabledInstanceValidationLayerNames)
        {
            if(!isValidationLayerSupported(name))
            {
                throw std::runtime_error("Validation layer not supported!");
            }
        }

        bool debugExtensionFound = false;
        for(const char* name : m_enabledInstanceKHRExtensionNames)
        {
            if(strcmp(name, VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0)
            {
                debugExtensionFound = true;
                break;
            }
        }

        if(!debugExtensionFound)
        {
            throw std::runtime_error("KHR Debug Extension is required when using the validation layers. Please enable it.");
        }
    }
}

bool VulkanLayerAndExtensionManager::isValidationLayerSupported(const char* name) {
    for(vk_layer_extension_properties& prop : m_instanceLayersAndExtentions)
    {
        if(strcmp(prop.m_layer_properties.layerName, name) == 0)
        {
            return true;
        }
    }
    Console::print("Validation layer not supported: ");
    Console::printLine(name);
    return false;
}

bool VulkanLayerAndExtensionManager::isKHRExtensionSupported(const char* name) {
    for(VkExtensionProperties& prop : m_instanceKHRExtensions)
    {
        if(strcmp(prop.extensionName,name) == 0)
        {
            return true;
        }
    }
    Console::print("KHR Extension not supported: ");
    Console::printLine(name);
    return false;
}



VulkanLayerAndExtensionManager::~VulkanLayerAndExtensionManager() {


}

bool VulkanLayerAndExtensionManager::getDebugEnabled() {
    return m_validationLayersEnabled;
}



vector<const char*> VulkanLayerAndExtensionManager::getEnabledInstanceKHRExtensions() {
    return m_enabledInstanceKHRExtensionNames;
}

vector<const char*> VulkanLayerAndExtensionManager::getEnabledInstanceValidationLayers() {
    return m_enabledInstanceValidationLayerNames;
}

void VulkanLayerAndExtensionManager::setVkInstanceHandle(VkInstance& handle) {
    m_current_instance_handle = handle;
}



