//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCORESTRUCTS_H
#define VULKANO_ENGINE_VULKANCORESTRUCTS_H

#include <string>
#include <vector>
#include <array>
#include <vulkan/vulkan.h>

using std::array;
using std::vector;
using std::string;

struct vk_physical_device_info {

    VkPhysicalDevice                m_PhysicalDevice = VK_NULL_HANDLE;
    vector<VkQueueFamilyProperties> m_QueueFamilyProperties;

};


struct vk_layer_extension_properties {

    VkLayerProperties               m_layerProperties;
    vector<VkExtensionProperties>   m_ExtensionProperties;

};


struct vk_device_rating {

    VkPhysicalDevice    m_PhysicalDevice;
    int32_t             m_Rating;

};


struct vk_queue_family
{
    VkQueueFamilyProperties m_QueueFamilyProperties;
    uint32_t m_Index;
    vector<float> m_Priorities;
};

#endif //VULKANO_ENGINE_VULKANCORESTRUCTS_H