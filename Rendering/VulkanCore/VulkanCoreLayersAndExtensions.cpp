//
// Created by hindrik on 13-2-17.
//

#include <iostream>
#include <algorithm>
#include <cstring>
#include "../VulkanCore.h"
#include "../../Core/Console.h"

bool VulkanCore::isValidationLayerSupported(const char *name) {
    for(vk_layer_extension_properties& prop : m_InstanceLayersAndExtentions)
    {
        if(strcmp(prop.m_layerProperties.layerName, name) == 0)
        {
            return true;
        }
    }
    Console::print("Validation layer not supported: ");
    Console::printLine(name);
    return false;
}

bool VulkanCore::isInstanceKHRExtensionSupported(const char *name) {
    for(VkExtensionProperties& prop : m_InstanceKHRExtensions)
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

bool VulkanCore::isDeviceExtensionSupported(const char *name, const VkPhysicalDevice deviceToCheck) {

    vector<VkExtensionProperties> props = vkEnumerateDeviceExtensions(deviceToCheck);

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

vector<VkExtensionProperties> VulkanCore::vkEnumerateDeviceExtensions(const VkPhysicalDevice deviceToCheck) {

    vector<VkExtensionProperties> extensionProperties;
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(deviceToCheck, nullptr, &extensionCount, nullptr);

    if(extensionCount == 0)
        return extensionProperties;

    extensionProperties.resize(extensionCount);

    vkEnumerateDeviceExtensionProperties(deviceToCheck, nullptr, &extensionCount, extensionProperties.data());

    Console::printLine("Found " + std::to_string(extensionCount) + " KHR device extentions:");

    for(uint32_t i = 0; i < static_cast<uint32_t >(extensionProperties.size()); ++i)
    {
        cout << extensionProperties[i] << std::endl;
    }

    return extensionProperties;
}

VkResult VulkanCore::vkEnumerateExtensionLayersAndExtensions() {

    VkResult result;
    uint32_t instanceLayerCount;
    vector<VkLayerProperties> instanceLayerProperties;

    result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);

    VK_IF_FAIL_THROW_MSG(result, "Error occurred when fetching instance layer count");

    instanceLayerProperties.resize(instanceLayerCount);

    m_InstanceLayersAndExtentions.resize(instanceLayerCount);

    result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());

    VK_IF_FAIL_THROW_MSG(result, "Error occurred when fetching the instance layer properties");

    for(uint32_t i = 0; i < instanceLayerProperties.size(); ++i)
    {
        VkLayerProperties& layerProperty = instanceLayerProperties[i];
        uint32_t extensionCount;
        vector<VkExtensionProperties> extensionProperties;

        result = vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &extensionCount, nullptr);

        VK_IF_FAIL_THROW_MSG(result, "Error occurred when fetching the instance layer extension count");

        extensionProperties.resize(extensionCount);

        result = vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &extensionCount, extensionProperties.data());

        VK_IF_FAIL_THROW_MSG(result, "Error occurred when fetching the instance layer extension properties");

        vk_layer_extension_properties layerProps;

        layerProps.m_layerProperties        = layerProperty;
        layerProps.m_ExtensionProperties    = extensionProperties;

        m_InstanceLayersAndExtentions[i]    = (layerProps);
    }

    std::cout << "Found " << m_InstanceLayersAndExtentions.size() << " Extension Layers" << std::endl;

    for(vk_layer_extension_properties ep : m_InstanceLayersAndExtentions)
    {
        std::cout << ep;
    }

    return result;
}

VkResult VulkanCore::vkEnumerateKHRExtensions() {
    VkResult result;
    uint32_t extensionCount;

    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    VK_IF_FAIL_THROW_MSG(result, "Error occurred when fetching the instance KHR extension count");

    m_InstanceKHRExtensions.resize(extensionCount);

    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_InstanceKHRExtensions.data());

    VK_IF_FAIL_THROW_MSG(result, "Error occurred when fetching the instance KHR extension properties");

    Console::printLine("Found " + std::to_string(m_InstanceKHRExtensions.size()) + " KHR Extensions");

    for(VkExtensionProperties ep : m_InstanceKHRExtensions)
    {
        cout << ep;
    }

    return result;
}

VkResult VulkanCore::loadLayersAndExtensions() {
    VkResult result;

    m_Platform.ProcessExtensions(m_EnabledInstanceKHRExtensionNames);

    result = vkEnumerateKHRExtensions();

    if(result != VK_SUCCESS)
        throw std::runtime_error("Error enumerating KHR extensions!");

    result = vkEnumerateExtensionLayersAndExtensions();

    if(result != VK_SUCCESS)
        throw std::runtime_error("Error enumerating validation layers and extensions!");


    for(const char* name : m_EnabledInstanceKHRExtensionNames)
    {
        if(!isInstanceKHRExtensionSupported(name))
        {
            throw std::runtime_error("KHR Extension not supported!");
        }
    }

    if(m_IsDebugEnabled)
    {
        for(const char* name : m_EnabledInstanceValidationLayerNames)
        {
            if(!isValidationLayerSupported(name))
            {
                throw std::runtime_error("Validation layer not supported!");
            }
        }

        bool debugExtensionFound = false;
        for(const char* name : m_EnabledInstanceKHRExtensionNames)
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

bool VulkanCore::checkDeviceExtensions(const VkPhysicalDevice deviceToCheck, vector<const char *> extensionNames) {
    for(uint32_t i = 0; i < static_cast<uint32_t >(extensionNames.size()); ++i)
    {
        bool isSupported;
        isSupported = isDeviceExtensionSupported(extensionNames[i], deviceToCheck);
        if(!isSupported)
            return false;
    }
    return true;
}



