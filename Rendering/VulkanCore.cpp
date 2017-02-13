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
        : m_ApplicationName(applicationName), m_EnabledInstanceKHRExtensionNames(enabledKHRInstanceExtensions), m_EnabledInstanceValidationLayerNames(enabledInstanceValidationLayers), m_IsDebugEnabled(enableDebugLayers), m_EnabledDeviceExtensions(enabledDeviceExtensions)
{
    loadLayersAndExtensions();
    VkResult r = vkInit();
    if(r == VK_SUCCESS)
        Console::printLine("Initialized Vulkan API succesfully.");
}

VulkanCore::~VulkanCore() {

    cleanUpDebugFacilities();

    if(m_Swapchain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);

    cleanUpSwapchainImageViews();

    if(m_Surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

    if(m_Device != VK_NULL_HANDLE)
        vkDestroyDevice(m_Device, nullptr);

    if(m_Instance != VK_NULL_HANDLE)
        vkDestroyInstance(m_Instance,nullptr);
}

bool VulkanCore::processPlatformAPI(float deltaTime) {
    return m_Platform.processAPI(deltaTime);
}





