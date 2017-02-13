//
// Created by hindrik on 13-2-17.
//

#include <iostream>
#include <algorithm>
#include <cstring>
#include "../VulkanCore.h"
#include "../../Core/Console.h"

bool VulkanCore::is_validation_layer_supported(const char *name) {
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

bool VulkanCore::is_instance_KHR_extension_supported(const char *name) {
    for(VkExtensionProperties& prop : m_instanceKHRExtensions)
    {
        if(strcmp(prop.extensionName,name) == 0)
        {
            return true;
        }
    }
    Console::print("KHR extension not supported: ");
    Console::printLine(name);
    return false;
}

bool VulkanCore::is_device_extension_supported(const char *name, const VkPhysicalDevice deviceToCheck) {

    vector<VkExtensionProperties> props = vk_enumerate_device_extentions(deviceToCheck);

    for(VkExtensionProperties& prop : props)
    {
        if(strcmp(prop.extensionName, name) == 0)
        {
            return true;
        }
    }
    Console::print("Device extension not supported: ");
    Console::printLine(name);
    return false;
}

vector<VkExtensionProperties> VulkanCore::vk_enumerate_device_extentions(const VkPhysicalDevice deviceToCheck) {

    vector<VkExtensionProperties> extensionProperties;
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(deviceToCheck, nullptr, &extension_count, nullptr);

    if(extension_count == 0)
        return extensionProperties;

    extensionProperties.resize(extension_count);

    vkEnumerateDeviceExtensionProperties(deviceToCheck, nullptr, &extension_count, extensionProperties.data());

    Console::printLine("Found " + std::to_string(extension_count) + " KHR device extentions:" );

    for(uint32_t i = 0; i < static_cast<uint32_t >(extensionProperties.size()); ++i)
    {
        cout << extensionProperties[i] << std::endl;
    }

    return extensionProperties;
}

VkResult VulkanCore::vk_enumerate_extension_layers_and_extensions() {

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

VkResult VulkanCore::vk_enumerate_KHR_extensions() {
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

VkResult VulkanCore::load_layers_and_extensions() {
    VkResult result;

    result = vk_enumerate_KHR_extensions();

    if(result != VK_SUCCESS)
        throw std::runtime_error("Error enumerating KHR extensions!");

    result = vk_enumerate_extension_layers_and_extensions();

    if(result != VK_SUCCESS)
        throw std::runtime_error("Error enumerating validation layers and extensions!");


    for(const char* name : m_enabledInstanceKHRExtensionNames)
    {
        if(!is_instance_KHR_extension_supported(name))
        {
            throw std::runtime_error("KHR Extension not supported!");
        }
    }

    if(m_isDebugEnabled)
    {
        for(const char* name : m_enabledInstanceValidationLayerNames)
        {
            if(!is_validation_layer_supported(name))
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
    return result;
}

bool VulkanCore::check_device_extentions(const VkPhysicalDevice deviceToCheck, vector<const char*> extensionNames) {
    for(uint32_t i = 0; i < static_cast<uint32_t >(extensionNames.size()); ++i)
    {
        bool isSupported;
        isSupported = is_device_extension_supported(extensionNames[i], deviceToCheck);
        if(!isSupported)
            return false;
    }
    return true;
}