//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCORE_H
#define VULKANO_ENGINE_VULKANCORE_H

#include <vector>
#include <string>
#include "VulkanPlatforms/VulkanPlatform.h"
#include "Utilities/VulkanCoreStructs.h"
#include "Utilities/VulkanCoreUtilityFunctions.h"
#include "Utilities/VulkanLayerAndExtensionManager.h"


#define VK_CHECK(result) if(result != VK_SUCCESS) return result;

using std::vector;
using std::string;

class VulkanCore {
private:
    VkInstance                      m_instance                      = VK_NULL_HANDLE;
    VkDevice                        m_device                        = VK_NULL_HANDLE;
    VkPhysicalDevice                m_physical_device               = VK_NULL_HANDLE;
    vk_physical_device_info         m_physical_device_info          = {};
    vector<vk_queue_family>         m_queue_families;

    VkQueue                         m_graphics_queue;
    vector<VkQueue>                 m_additional_graphics_queues;
    vector<VkQueue>                 m_compute_queues;
    vector<VkQueue>                 m_transfer_only_queues;
    vector<VkQueue>                 m_sparse_binding_queues;

    VkSurfaceKHR                    m_surface                       = VK_NULL_HANDLE;

private:
    string                          m_application_name;
    VulkanLayerAndExtensionManager  m_layerAndExtensionManager;
    VulkanPlatform                  m_platform;
public:
    VulkanCore(string applicationName, vector<const char*> enabledKHRInstanceExtensions, vector<const char*> enabledInstanceValidationLayers, bool enableValidationLayers);
    ~VulkanCore();
private:
    VkResult vk_init();
    VkResult vk_init_instance();
    VkResult vk_init_physical_device();
    void vk_init_setup_queue_families(const vector<VkQueueFamilyProperties>& queueFamilies);
    VkResult vk_init_logical_device();
    void vk_init_assign_queues();
    VkResult vk_init_create_surface();


    static vk_physical_device_info vk_init_get_queue_families(const VkPhysicalDevice& device);
    static bool vk_init_check_device(const VkPhysicalDevice deviceToCheck);
    static int32_t vk_init_suitability_rating(const VkPhysicalDevice deviceToRate);





public:
    bool process_platform_API(float deltaTime);


};


#endif //VULKANO_ENGINE_VULKANCORE_H