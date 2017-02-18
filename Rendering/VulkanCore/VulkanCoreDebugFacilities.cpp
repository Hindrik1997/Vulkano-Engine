//
// Created by hindrik on 13-2-17.
//
#include <iostream>
#include <algorithm>
#include <cstring>
#include "../VulkanCore.h"
#include "../../Core/Console.h"

VkBool32 debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char *layerPrefix,
        const char *msg,
        void *userData)
{
    std::cerr << "Validation layer: " << msg << std::endl;

    return VK_FALSE;

}

VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkResult destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
        return VK_SUCCESS;
    } else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void VulkanCore::cleanUpDebugFacilities() {
    if(m_IsDebugEnabled)
    {

        VkResult result = destroyDebugReportCallbackEXT(m_Instance, m_DebugCallback, NULL);
        if(result  != VK_SUCCESS)
            Console::printLine("Error when destroying debug reporter callback.");
    }
}

void VulkanCore::setupDebugFacilities() {
    if(!m_IsDebugEnabled)
    {
        throw std::runtime_error("Error, enabling debug facilities while the debug layers are not enabled.");
    }

    VkDebugReportCallbackCreateInfoEXT createInfo = {};


    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.pNext = NULL;
    createInfo.flags =
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT |
            VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
    createInfo.pfnCallback = debugCallback;
    createInfo.pUserData = NULL;

    if (createDebugReportCallbackEXT(m_Instance, &createInfo, nullptr, &m_DebugCallback) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug callback!");
    }
    Console::printLine("Succesfully set up the debug facilities!");
}