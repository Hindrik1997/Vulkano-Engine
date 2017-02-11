//
// Created by hindrik on 11-2-17.
//

#ifndef VULKANOENGINE_VULKANLAYERANDEXTENSIONMANAGERDEBUG_H
#define VULKANOENGINE_VULKANLAYERANDEXTENSIONMANAGERDEBUG_H

#include "VulkanLayerAndExtensionManager.h"

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
VkResult DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData);

#endif //VULKANOENGINE_VULKANLAYERANDEXTENSIONMANAGERDEBUG_H
