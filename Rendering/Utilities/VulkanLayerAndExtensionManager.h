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

    vector<const char*> m_enabledInstanceKHRExtensionNames;
    vector<const char*>  m_enabledInstanceValidationLayerNames;

    bool m_validationLayersEnabled;
    VkDebugReportCallbackEXT m_debugCallbackFP = nullptr;

    VkInstance m_current_instance_handle = VK_NULL_HANDLE;

public:
    VulkanLayerAndExtensionManager(vector<const char*> enabledInstanceKHRExtensionNames, vector<const char*> enabledInstanceValidationLayerNames, bool enableValidationLayers);
    ~VulkanLayerAndExtensionManager();

private:
    VkResult vk_enumerate_extension_layers_and_extensions();
    VkResult vk_enumerate_KHR_extensions();
    bool isValidationLayerSupported(const char* name);
    bool isKHRExtensionSupported(const char* name);

public:
    bool getDebugEnabled();
    void setupDebugFacilities();
    void cleanupDebugFacilities();
    void setVkInstanceHandle(VkInstance& handle);
public:
    vector<const char*> getEnabledInstanceKHRExtensions();
    vector<const char*> getEnabledInstanceValidationLayers();


};





#endif //VULKANOENGINE_VULKANLAYEREXTENSIONMANAGER_H
