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

using std::vector;
using std::string;

class VkCore final : NonCopyable
{
private:
    bool                                    m_IsDebugEnabled                                = false;
    VkDebugReportCallbackEXT                m_DebugCallback                                 = nullptr;

    VkUniqueHandle<VkInstance>              m_Instance                                      = {vkDestroyInstance};
    VkUniqueHandle<VkDevice>                m_Device                                        = {vkDestroyDevice};
    VkPhysicalDevice                        m_PhysicalDevice                                = VK_NULL_HANDLE;
    vk_physical_device_info                 m_PhysicalDeviceInfo                            = {};
    vector<vk_queue_family>                 m_QueueFamilies                                 = {};

    vector<vk_queue>                        m_GraphicsQueues                                = {};
    vector<vk_queue>                        m_ComputeQueues                                 = {};
    vector<vk_queue>                        m_TransferOnlyQueues                            = {};
    vector<vk_queue>                        m_SparseBindingQueues                           = {};

    vector<uint32_t>                        m_TransferQueueFamilies                         = {};
    vector<CommandPool>                     m_TransferCommandPools                          = {};
public:
    VkCore                                  (vk_core_create_info createInfo);
    ~VkCore                                 ();

    VkCore(const VkCore&) = delete;
    VkCore(VkCore&&) = delete;

    VkCore& operator=(const VkCore&) = delete;
    VkCore& operator=(VkCore&&) = delete;
private:
    auto vkInit                             (vk_core_create_info createInfo)                                -> void;
    auto vkInitInstance                     (vk_core_create_info createInfo)                                -> void;
    auto vkInitPhysicalDevice               (vk_core_create_info createInfo)                                -> void;
    auto vkInitSetupQueueFamilies           (const vector<VkQueueFamilyProperties>& queueFamilies)          -> void;
    auto vkInitLogicalDevice                (vk_core_create_info createInfo)                                -> void;
    auto vkInitAssignQueues                 ()                                                              -> void;
    auto vkInitSetupTransferFacilities      ()                                                              -> void;
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
    const VkInstance               instance()           const;
    const VkDevice                 device()             const;
    const VkPhysicalDevice         physicalDevice()     const;
    const vk_physical_device_info  physicalDeviceInfo() const;

    auto        graphicsQueues()         const -> const vector<vk_queue>&;
    auto        computeQueues()          const -> const vector<vk_queue>&;
    auto        transferOnlyQueues()     const -> const vector<vk_queue>&;
    auto        sparseBindingQueues()    const -> const vector<vk_queue>&;
    auto        transferQueueFamilies()  const -> const vector<uint32_t>&;
public:
    auto        copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0) -> void;

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
