//
// Created by hindrik on 25-3-17.
//

#ifndef VULKANOENGINE_VKCORE_H
#define VULKANOENGINE_VKCORE_H

#include "VulkanPlatforms/VulkanPlatform.h"
#include "../../Utility Classes/NonCopyable.h"
#include "../../Utility Classes/NonMovable.h"
#include "../Utilities/VulkanStructs.h"
#include "../Utilities/VulkanUtilityFunctions.h"
#include "VkUniqueHandle.h"
#include "Classes/CommandPool.h"
#include "MemoryManagement/MemoryManager.h"
#include "../../Utility Classes/Nullable.h"


using std::vector;
using std::string;

class Engine;

class Instance final : NonCopyable
{
private:
    Engine*                                 m_Engine;
    bool                                    m_IsDebugEnabled                                = false;
    VkDebugReportCallbackEXT                m_DebugCallback                                 = nullptr;

    VkUniqueHandle<VkInstance>              m_Instance                                      = {vkDestroyInstance};
public:
    Instance                                  (vk_core_create_info createInfo, Engine* engine);
    ~Instance                                 ();

    Instance(const Instance&) = delete;
    Instance(Instance&&) = delete;

    Instance& operator=(const Instance&) = delete;
    Instance& operator=(Instance&&) = delete;
private:
    auto vkSetupInit                        (vk_core_create_info&      info)                                -> void;
    auto vkInit                             (vk_core_create_info createInfo)                                -> void;
    auto vkInitInstance                     (vk_core_create_info createInfo)                                -> void;

private:
    auto setupDebugFacilities               ()                                                              -> void;
    auto cleanUpDebugFacilities             ()                                                              -> void;
private:
    static auto     isValidationLayerSupported      (const char *name)                                      -> bool;
    static auto     isInstanceExtensionSupported    (const char *name)                                      -> bool;

    static auto     enumerateValidationLayers       ()                                                      -> vector<vk_layer_extension_properties>;
    static auto     enumerateInstanceExtensions     ()                                                      -> vector<VkExtensionProperties>;

    static auto     checkLayersAndInstanceExtensionsSupport     (vk_core_create_info createInfo)                                            -> void;

public:
    const VkInstance               instance()           const;
};

VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);
VkResult destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks *pAllocator);

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData);




#endif //VULKANOENGINE_VKCORE_H
