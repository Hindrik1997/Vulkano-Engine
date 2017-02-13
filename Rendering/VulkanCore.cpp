//
// Created by hindrik on 27-1-17.
//

#include <iostream>
#include "VulkanCore.h"
#include "../Core/Console.h"


VulkanCore::VulkanCore(string applicationName,
                       vector<const char*> enabledKHRInstanceExtensions,
                       vector<const char*> enabledInstanceValidationLayers,
                       vector<const char*> enabledDeviceExtensions,
                       bool enableDebugLayers)
        : m_application_name(applicationName), m_enabledInstanceKHRExtensionNames(enabledKHRInstanceExtensions), m_enabledInstanceValidationLayerNames(enabledInstanceValidationLayers), m_isDebugEnabled(enableDebugLayers), m_enabledDeviceExtensions(enabledDeviceExtensions)
{
    load_layers_and_extensions();
    VkResult r = vk_init();
    if(r == VK_SUCCESS)
        Console::printLine("Initialized Vulkan API succesfully.");
}

VulkanCore::~VulkanCore() {
    cleanup_debug_facilities();
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance,nullptr);
}

bool VulkanCore::process_platform_API(float deltaTime) {
    return m_platform.processAPI(deltaTime);
}





