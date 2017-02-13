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


#define VK_CHECK(result) if(result != VK_SUCCESS) return result;
#define VK_IF_FAIL_MSG(result, message) if(result != VK_SUCCESS) { Console::printLine(message); return result; }

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
    VulkanPlatform                  m_platform;
public:
    VulkanCore(
            string applicationName,
            vector<const char*> enabledKHRInstanceExtensions,
            vector<const char*> enabledInstanceValidationLayers,
            vector<const char*> enabledDeviceExtenions,
            bool enableDebugLayers);
    ~VulkanCore();
private:
    VkResult vk_init();
    VkResult vk_init_instance();
    VkResult vk_init_physical_device();
    void     vk_init_setup_queue_families(const vector<VkQueueFamilyProperties>& queueFamilies);
    VkResult vk_init_logical_device();
    void     vk_init_assign_queues();
    VkResult vk_init_create_surface();

    static vk_physical_device_info vk_init_get_queue_families(const VkPhysicalDevice& device);
    static bool vk_init_check_device(const VkPhysicalDevice deviceToCheck, const vector<const char*>& DeviceExtentions);
    static int32_t vk_init_suitability_rating(const VkPhysicalDevice deviceToRate);
private:
    vector<vk_layer_extension_properties> m_instanceLayersAndExtentions;
    vector<VkExtensionProperties> m_instanceKHRExtensions;
    vector<VkExtensionProperties> m_deviceExtensions;

    vector<const char*> m_enabledInstanceKHRExtensionNames;
    vector<const char*>  m_enabledInstanceValidationLayerNames;
    vector<const char*> m_enabledDeviceExtensions;

    bool m_isDebugEnabled;
    VkDebugReportCallbackEXT m_debugCallbackFP = nullptr;
private:
    VkResult load_layers_and_extensions();
    void setup_debug_facilities();
    void cleanup_debug_facilities();

    VkResult vk_enumerate_extension_layers_and_extensions();
    VkResult vk_enumerate_KHR_extensions();
    static vector<VkExtensionProperties> vk_enumerate_device_extentions(const VkPhysicalDevice deviceToCheck);

    bool is_validation_layer_supported(const char* name);
    bool is_instance_KHR_extension_supported(const char *name);
    static bool is_device_extension_supported(const char* name, const VkPhysicalDevice deviceToCheck);
    static bool check_device_extentions(const VkPhysicalDevice deviceToCheck, vector<const char*> extensionNames);
public:
    bool process_platform_API(float deltaTime);


};

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
VkResult DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData);



#endif //VULKANO_ENGINE_VULKANCORE_H