//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCORE_H
#define VULKANO_ENGINE_VULKANCORE_H

#include <vector>
#include <string>
#include "VulkanPlatforms/VulkanPlatform.h"
#include "Utilities/VulkanStructs.h"
#include "Utilities/VulkanUtilityFunctions.h"

#define VK_IF_FAIL_MSG(result, message) if(result != VK_SUCCESS) { Console::printLine(message); return result; }

using std::vector;
using std::string;

class VulkanCore {
private:
    VkInstance                              m_Instance                      = VK_NULL_HANDLE;
    VkDevice                                m_Device                        = VK_NULL_HANDLE;
    VkPhysicalDevice                        m_PhysicalDevice                = VK_NULL_HANDLE;
    vk_physical_device_info                 m_PhysicalDeviceInfo            = {};
    vector<vk_queue_family>                 m_QueueFamilies;

    VkQueue                                 m_GraphicsQueue                 = VK_NULL_HANDLE;
    vector<VkQueue>                         m_AdditionalGraphicsQueues;
    vector<VkQueue>                         m_ComputeQueues;
    vector<VkQueue>                         m_TransferOnlyQueues;
    vector<VkQueue>                         m_SparseBindingQueues;

    VkSurfaceKHR                            m_Surface                       = VK_NULL_HANDLE;
    VkSwapchainKHR                          m_Swapchain                     = VK_NULL_HANDLE;
    vector<VkImage>                         m_SwapChainImages               = {};
    vector<VkImageView>                     m_SwapChainImageViews           = {};
    VkExtent2D                              m_SwapChainExtent               = {};
    VkFormat                                m_SwapChainFormat               = {};

    string                                  m_ApplicationName               = "";
    VulkanPlatform                          m_Platform;

    vector<vk_layer_extension_properties>   m_InstanceLayersAndExtentions;
    vector<VkExtensionProperties>           m_InstanceKHRExtensions;

    vector<const char*>                     m_EnabledInstanceKHRExtensionNames;
    vector<const char*>                     m_EnabledInstanceValidationLayerNames;
    vector<const char*>                     m_EnabledDeviceExtensions;

    bool                                    m_IsDebugEnabled;
    VkDebugReportCallbackEXT                m_DebugCallback = nullptr;




public:
    VulkanCore(
            string applicationName,
            vector<const char*> enabledKHRInstanceExtensions,
            vector<const char*> enabledInstanceValidationLayers,
            vector<const char*> enabledDeviceExtenions,
            bool enableDebugLayers);
    ~VulkanCore();

private:
    VkResult vkInit();
    VkResult vkInitInstance();
    VkResult vkInitPhysicalDevice();
    void     vkInitSetupQueueFamilies(const vector<VkQueueFamilyProperties> &queueFamilies);
    VkResult vkInitLogicalDevice();
    void     vkInitAssignQqueues();
    VkResult vkInitCreateSurface();
    VkResult vkInitCreateSwapchain();
    VkResult vkInitCreateSwapchainImageViews();

    static  vk_physical_device_info     vkInitGetQueueFamilies(const VkPhysicalDevice device);
    static  bool                        vkInitCheckDevice(const VkPhysicalDevice deviceToCheck, const vector<const char *> &DeviceExtentions, const VkSurfaceKHR surface);
    static  int32_t                     vkInitSuitabilityRating(const VkPhysicalDevice deviceToRate);
private:

    VkResult    loadLayersAndExtensions();
    bool        isValidationLayerSupported(const char *name);
    bool        isInstanceKHRExtensionSupported(const char *name);
    void        setupDebugFacilities();
    void        cleanUpDebugFacilities();
    void        cleanUpSwapchainImageViews();



    VkResult vkEnumerateExtensionLayersAndExtensions();
    VkResult vkEnumerateKHRExtensions();

    static  vector<VkExtensionProperties>   vkEnumerateDeviceExtentions(const VkPhysicalDevice deviceToCheck);
    static  bool                            isDeviceExtensionSupported(const char *name, const VkPhysicalDevice deviceToCheck);
    static  bool                            checkDeviceExtentions(const VkPhysicalDevice deviceToCheck, vector<const char *> extensionNames);

public:
    bool processPlatformAPI(float deltaTime);


};

VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);
VkResult destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks *pAllocator);

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