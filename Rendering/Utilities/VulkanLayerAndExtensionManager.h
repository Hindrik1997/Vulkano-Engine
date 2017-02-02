//
// Created by hindrik on 2-2-17.
//

#ifndef VULKANOENGINE_VULKANLAYEREXTENSIONMANAGER_H
#define VULKANOENGINE_VULKANLAYEREXTENSIONMANAGER_H

#define VK_IF_FAIL_MSG(result, message) if(result != VK_SUCCESS) { Console::printLine(message); return result; }

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "VulkanCoreStructs.h"
#include "VulkanCoreUtilityFunctions.h"

class VulkanLayerAndExtensionManager {
private:
    vector<vk_layer_extension_properties> m_instanceLayersAndExtentions;
    vector<VkExtensionProperties> m_instanceKHRExtensions;

    vector<string> m_enabledInstanceKHRExtensionNames;
    vector<string>  m_enabledInstanceValidationLayerNames;


public:
    VulkanLayerAndExtensionManager(vector<string> enabledInstanceKHRExtensionNames, vector<string> enabledInstanceValidationLayerNames, bool enableValidationLayers);


private:
    VkResult vk_enumerate_extension_layers_and_extensions();
    VkResult vk_enumerate_KHR_extensions();
    bool isValidationLayerSupported(string name);
    bool isKHRExtensionSupported(string name);



};


#endif //VULKANOENGINE_VULKANLAYEREXTENSIONMANAGER_H
