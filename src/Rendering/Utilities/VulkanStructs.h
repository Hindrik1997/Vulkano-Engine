//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCORESTRUCTS_H
#define VULKANO_ENGINE_VULKANCORESTRUCTS_H

#include <string>
#include <vector>
#include <array>
#include "../Vulkan/VulkanPlatforms/VulkanPlatform.h"

using std::array;
using std::vector;
using std::string;

struct vk_physical_device_queue_family_info
{
    VkPhysicalDevice                m_PhysicalDevice            = VK_NULL_HANDLE;
    vector<VkQueueFamilyProperties> m_QueueFamilyProperties     = {};

};


struct vk_layer_extension_properties
{
    VkLayerProperties               m_layerProperties       = {};
    vector<VkExtensionProperties>   m_ExtensionProperties   = {};

};


struct vk_device_rating {

    VkPhysicalDevice    m_PhysicalDevice    = VK_NULL_HANDLE;
    int32_t             m_Rating            = 0;

};

struct vk_queue_family_properties
{
    VkQueueFamilyProperties m_QueueFamilyProperties = {};
    uint32_t m_Index                                = 0;
};

struct vk_queue_family
{
    VkQueueFamilyProperties m_QueueFamilyProperties = {};
    uint32_t                m_FamilyIndex           = 0;
    vector<float>           m_Priorities            = {};
    vector<uint32_t>        m_QueueIndices          = {};
};

struct vk_queue_family_selected_set
{
    vk_queue_family         presentQueue;
    vector<vk_queue_family> computeFamilies;
    vector<vk_queue_family> transferFamlies;
};

struct vk_swapchain_details
{
    VkSurfaceCapabilitiesKHR    m_Capabilities  = {};
    vector<VkSurfaceFormatKHR>  m_Formats       = {};
    vector<VkPresentModeKHR>    m_PresentModes  = {};

};

struct vk_queue
{
    VkQueue                     m_Queue            = VK_NULL_HANDLE;
    uint32_t                    m_FamilyIndex      = 0;

};

struct vk_queue_set
{
    uint32_t            m_FamilyIndex   = 0;
    vector<VkQueue>     m_Queues        = {};
};

struct vk_core_create_info
{
    const char*                         m_ApplicationName                           = "";
    vector<const char*>                 m_EnabledInstanceValidationLayerNames       = {};
    vector<const char*>                 m_EnabledInstanceExtensionNames             = {};
    bool                                m_EnableDebugLayers                         = false;
    bool                                m_EnumerateLayersAndExtensionsInConsole     = false;

};

struct vk_present_device_create_info
{
    VkInstance                          m_Instance                                  = VK_NULL_HANDLE;
    VkSurfaceKHR                        m_Surface                                   = VK_NULL_HANDLE;
    vector<const char*>                 m_EnabledDeviceExtentionNames               = {};
    bool                                m_EnumeratePossibleDevicesInConsole         = false;
    bool                                m_EnumerateLayersAndExtensionsInConsole     = false;
};

struct vk_memory_heap
{
    uint32_t                                            m_HeapIndex     = 0;
    bool                                                m_IsDeviceLocal = false;
    VkMemoryHeap                                        m_MemoryHeap    = {};
    vector<pair<VkMemoryPropertyFlagBits,uint32_t>>     m_MemoryTypes   = {};


};

struct vk_memory_allocation
{
    VkDeviceMemory  m_Memory = VK_NULL_HANDLE;
    VkDeviceSize    m_Offset = 0;
    VkDeviceSize    m_Size   = 0;

};

#endif //VULKANO_ENGINE_VULKANCORESTRUCTS_H