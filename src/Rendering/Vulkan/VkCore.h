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

using std::vector;
using std::string;

class VkCore : public NonCopyable
{
private:
    bool                                    m_IsDebugEnabled                                = false;
    VkDebugReportCallbackEXT                m_DebugCallback                                 = nullptr;

    VkInstance                              m_Instance                                      = VK_NULL_HANDLE;
    VkDevice                                m_Device                                        = VK_NULL_HANDLE;
    VkPhysicalDevice                        m_PhysicalDevice                                = VK_NULL_HANDLE;
    vk_physical_device_info                 m_PhysicalDeviceInfo                            = {};
    vector<vk_queue_family>                 m_QueueFamilies                                 = {};

    vector<vk_queue>                        m_GraphicsQueues                                = {};
    vector<vk_queue>                        m_ComputeQueues                                 = {};
    vector<vk_queue>                        m_TransferOnlyQueues                            = {};
    vector<vk_queue>                        m_SparseBindingQueues                           = {};
public:
    VkCore                                  (vk_core_create_info createInfo);
    ~VkCore                                 ();
private:
    auto vkInit                             (vk_core_create_info createInfo)                                -> void;
    auto vkInitInstance                     (vk_core_create_info createInfo)                                -> void;
    auto vkInitPhysicalDevice               (vk_core_create_info createInfo)                                -> void;
    auto vkInitSetupQueueFamilies           (const vector<VkQueueFamilyProperties>& queueFamilies)          -> void;
    auto vkInitLogicalDevice                (vk_core_create_info createInfo)                                -> void;
    auto vkInitAssignQueues                 ()                                                              -> void;
private:
    auto setupDebugFacilities               ()                                                              -> void;
    auto cleanUpDebugFacilities             ()                                                              -> void;
private:
    static auto     isValidationLayerSupported      (const char *name)                                      -> bool;
    static auto     isInstanceExtensionSupported    (const char *name)                                      -> bool;
    static auto     isDeviceExtensionSupported      (const char *name, VkPhysicalDevice device)             -> bool;

    static auto     enumerateValidationLayers       ()                                                      -> vector<vk_layer_extension_properties>;
    static auto     enumerateInstanceExtensions     ()                                                      -> vector<VkExtensionProperties>;
    static auto     enumerateDeviceExtensions       (VkPhysicalDevice device)                               -> vector<VkExtensionProperties>;

    static auto     checkLayersAndInstanceExtensionsSupport     (vk_core_create_info createInfo)                                            -> void;
    static auto     checkDeviceExtensions                       (VkPhysicalDevice device, vector<const char *> extensionNames)              -> bool;
    static auto     checkDevice                                 (VkPhysicalDevice device, const vector<const char *> &deviceExtensions)     -> bool;
    static auto     rateDeviceSuitability                       (VkPhysicalDevice device)                                                   -> int32_t;

    static auto     getDeviceQueueFamilies                      (VkPhysicalDevice device)                    ->vk_physical_device_info;
public:
    VkInstance               instance();
    VkDevice                 device();
    VkPhysicalDevice         physicalDevice();
    vk_physical_device_info  physicalDeviceInfo();

    vector<vk_queue>&        graphicsQueues();
    vector<vk_queue>&        computeQueues();
    vector<vk_queue>&        transferOnlyQueues();
    vector<vk_queue>&        sparseBindingQueues();



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
