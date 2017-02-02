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
    VkInstance m_instance;
    VkDevice m_device;
private:
    string m_application_name;
    VulkanLayerAndExtensionManager m_layerAndExtensionManager;
public:
    VulkanCore(string applicationName, vector<string> enabledKHRInstanceExtensions, vector<string> enabledInstanceValidationLayers, bool enableValidationLayers);
    ~VulkanCore();
private:
    VkResult vk_init();
    VkResult vk_init_instance();




public:
    bool process_platform_API(float deltaTime);


};


#endif //VULKANO_ENGINE_VULKANCORE_H