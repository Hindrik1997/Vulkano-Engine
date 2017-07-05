//
// Created by hindrik on 23-6-2017.
//

#ifndef VULKANOENGINE_DEVICE_H
#define VULKANOENGINE_DEVICE_H

#include "VulkanPlatforms/VulkanPlatform.h"
#include "VkUniqueHandle.h"
#include "../Utilities/VulkanStructs.h"

class PresentDevice final
{
private:
    VkUniqueHandle<VkDevice>                m_Device                                        = {vkDestroyDevice};
    VkInstance                              m_Instance                                      = VK_NULL_HANDLE;
    VkPhysicalDevice                        m_PhysicalDevice                                = VK_NULL_HANDLE;
    vk_queue                                m_PresentQueue                                  = {};
    vector<vk_queue_set>                    m_AsyncComputeQueues                            = {};
    vector<vk_queue_set>                    m_TransferQueues                                = {};
public:
    PresentDevice(vk_present_device_create_info createInfo);
    ~PresentDevice();
private:
    auto vkInitPhysicalDevice               (vk_present_device_create_info createInfo)                                -> void;
    auto vkInitLogicalDevice                (vk_present_device_create_info createInfo)                                -> void;
    auto vkInitSetupQueueFamilies           (vk_present_device_create_info createInfo)                                -> vk_queue_family_selected_set;
    auto vkInitSetupQueues                  (vk_queue_family_selected_set& queueFamilySet)                            -> void;

    static auto     checkDeviceExtensions                       (VkPhysicalDevice device, vector<const char *> extensionNames)              -> bool;
    static auto     checkDevice                                 (VkPhysicalDevice device, vk_present_device_create_info createInfo)         -> bool;
    static auto     rateDeviceSuitability                       (VkPhysicalDevice device)                                                   -> int32_t;

    static auto     getDeviceQueueFamilies                      (VkPhysicalDevice device)                    ->vk_physical_device_queue_family_info;
    static auto     enumerateDeviceExtensions       (VkPhysicalDevice device)                               -> vector<VkExtensionProperties>;
    static auto     isDeviceExtensionSupported      (const char *name, VkPhysicalDevice device)             -> bool;

private:


public:
    VkDevice                       device() const;
    VkPhysicalDevice               physicalDevice() const;
    vk_queue                       presentQueue() const;
    const vector<vk_queue_set>&    asyncComputeQueues() const;
    const vector<vk_queue_set>&    transferQueues() const;


};


#endif //VULKANOENGINE_DEVICE_H
