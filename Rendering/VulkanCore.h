//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCORE_H
#define VULKANO_ENGINE_VULKANCORE_H

#define VK_CHECK(result) if(result != VK_SUCCESS) return result;

#include <vector>
#include <string>
#include "VulkanPlatform.h"
#include "VulkanCoreStructs.h"
#include "VulkanCoreUtilityFunctions.h"

using std::vector;
using std::string;

class VulkanCore {
private:
    VkInstance m_instance;
    VkDevice m_device;
    VkSurfaceKHR m_surface;

    //TODO: FIX LATER
    VulkanPlatform* platform;

    vector<vk_physical_device_info> m_physical_devices_queue_families;

    vector<VkQueueFamilyProperties> m_selected_device_queue_family_properties;
    uint32_t m_selected_queue_family;
    VkCommandPool m_selected_device_command_pool;
    VkCommandBuffer m_selected_device_default_command_buffer;

    string m_application_name;
public:
    VulkanCore(string applicationName);
    ~VulkanCore();
private:
    VkResult vk_init();
    VkResult vk_init_instance();
    VkResult vk_init_enumerate_physical_devices_and_queues();
    VkResult vk_init_select_device_and_queues();
    VkResult vk_init_create_command_pool();
    VkResult vk_init_create_default_command_buffer();
    VkResult vk_init_setup_swapchain();
public:
    bool process_platform_API(float deltaTime);


};


#endif //VULKANO_ENGINE_VULKANCORE_H