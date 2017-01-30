//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCORESTRUCTS_H
#define VULKANO_ENGINE_VULKANCORESTRUCTS_H

#include <vector>
#include <vulkan/vulkan.h>

using std::vector;


typedef struct vk_physical_device_info {
    VkPhysicalDevice m_physical_device;
    vector<VkQueueFamilyProperties> m_queue_family_properties;
    uint32_t m_queue_family_count;
} vk_physical_device_info;











#endif //VULKANO_ENGINE_VULKANCORESTRUCTS_H
