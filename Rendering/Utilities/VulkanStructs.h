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

    VkPhysicalDevice                m_PhysicalDevice            = VK_NULL_HANDLE;
    vector<VkQueueFamilyProperties> m_QueueFamilyProperties     = {};

};


struct vk_layer_extension_properties {

    VkLayerProperties               m_layerProperties       = {};
    vector<VkExtensionProperties>   m_ExtensionProperties   = {};

};


struct vk_device_rating {

    VkPhysicalDevice    m_PhysicalDevice    = VK_NULL_HANDLE;
    int32_t             m_Rating            = 0;

};


struct vk_queue_family {

    VkQueueFamilyProperties m_QueueFamilyProperties = {};
    uint32_t m_Index                                = 0;
    vector<float> m_Priorities                      = {};
};

struct vk_swapchain_details {

    VkSurfaceCapabilitiesKHR    m_capabilities  = {};
    vector<VkSurfaceFormatKHR>  m_formats       = {};
    vector<VkPresentModeKHR>    m_presentModes  = {};

};


#endif //VULKANO_ENGINE_VULKANCORESTRUCTS_H