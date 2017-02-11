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
    VkInstance          m_instance                      = VK_NULL_HANDLE;
    VkDevice            m_device                        = VK_NULL_HANDLE;
    VkPhysicalDevice    m_physical_device               = VK_NULL_HANDLE;










private:
    string m_application_name;
    VulkanLayerAndExtensionManager m_layerAndExtensionManager;
public:
    VulkanCore(string applicationName, vector<const char*> enabledKHRInstanceExtensions, vector<const char*> enabledInstanceValidationLayers, bool enableValidationLayers);
    ~VulkanCore();
private:
    VkResult vk_init();
    VkResult vk_init_instance();
    VkResult vk_init_device();
    VkResult vk_init_select_physical_device();


public:
    bool process_platform_API(float deltaTime);


};


#endif //VULKANO_ENGINE_VULKANCORE_H