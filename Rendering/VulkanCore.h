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
#include "Utilities/ShaderModule.h"

inline void vk_if_fail_throw_message(VkResult result, std::string message){
    if(result != VK_SUCCESS)
        throw std::runtime_error(message);
}


using std::vector;
using std::string;

class VulkanCore {
public:
    VkInstance                              m_Instance                      = VK_NULL_HANDLE;
    VkDevice                                m_Device                        = VK_NULL_HANDLE;
    VkPhysicalDevice                        m_PhysicalDevice                = VK_NULL_HANDLE;
    vk_physical_device_info                 m_PhysicalDeviceInfo            = {};
    vector<vk_queue_family>                 m_QueueFamilies                 = {};

    vk_queue                                m_GraphicsQueue                 = {};
    vector<vk_queue>                        m_AdditionalGraphicsQueues      = {};
    vector<vk_queue>                        m_ComputeQueues                 = {};
    vector<vk_queue>                        m_TransferOnlyQueues            = {};
    vector<vk_queue>                        m_SparseBindingQueues           = {};

    VkSurfaceKHR                            m_Surface                       = VK_NULL_HANDLE;
    VkSwapchainKHR                          m_Swapchain                     = VK_NULL_HANDLE;
    vector<VkImage>                         m_SwapchainImages               = {};
    vector<VkImageView>                     m_SwapchainImageViews           = {};
    VkExtent2D                              m_SwapchainExtent               = {};
    VkFormat                                m_SwapchainFormat               = {};

    vector<ShaderModule>                    m_ShaderModules                 = {};

    VkViewport                              m_Viewport                      = {};
    VkRect2D                                m_Scissors                      = {};

    VkRenderPass                            m_RenderPass                    = VK_NULL_HANDLE;
    VkPipelineLayout                        m_PipelineLayout                = VK_NULL_HANDLE;
    VkPipeline                              m_Pipeline                      = VK_NULL_HANDLE;

    vector<VkFramebuffer>                   m_SwapchainFrameBuffers         = {};

    VkCommandPool                           m_CommandPool                   = VK_NULL_HANDLE;
    vector<VkCommandBuffer>                 m_CommandBuffers                = {};

    VkSemaphore                             m_ImageAvailableSemaphore       = VK_NULL_HANDLE;
    VkSemaphore                             m_RenderingFinsihedSemaphore    = VK_NULL_HANDLE;

    string                                  m_ApplicationName               = "";
    VK_PLATFORM                             m_Platform;

    vector<vk_layer_extension_properties>   m_InstanceLayersAndExtentions;
    vector<VkExtensionProperties>           m_InstanceKHRExtensions;

    vector<const char*>                     m_EnabledInstanceKHRExtensionNames;
    vector<const char*>                     m_EnabledInstanceValidationLayerNames;
    vector<const char*>                     m_EnabledDeviceExtensions;

    bool                                    m_IsDebugEnabled;
    VkDebugReportCallbackEXT                m_DebugCallback;




public:
    VulkanCore(
            string applicationName,
            vector<const char*> enabledKHRInstanceExtensions,
            vector<const char*> enabledInstanceValidationLayers,
            vector<const char*> enabledDeviceExtenions,
            bool enableDebugLayers);
    ~VulkanCore();

private:
    void vkInit();
    void vkInitInstance();
    void vkInitPhysicalDevice();
    void vkInitSetupQueueFamilies(const vector<VkQueueFamilyProperties> &queueFamilies);
    void vkInitLogicalDevice();
    void vkInitAssignQqueues();
    void vkInitCreateSurface();
    void vkInitCreateSwapchain();
    void vkInitCreateSwapchainImageViews();


    void vkInitCreateRenderPass();
    void vkInitCreatePipeline();
    void vkInitCreateFrameBuffers();
    void vkInitCreateCommandPool();
    void vkInitCreateSwapchainSemaphores();
    void vkInitCreateCommandBuffers();

    void cleanUpShaderModules();
    void cleanUpSwapchainImageViews();
    void cleanUpSwapchainFrameBuffers();

    static  vk_physical_device_info     vkInitGetQueueFamilies(const VkPhysicalDevice device);
    static  bool                        vkInitCheckDevice(const VkPhysicalDevice deviceToCheck, const vector<const char *> &DeviceExtentions, const VkSurfaceKHR surface);
    static  int32_t                     vkInitSuitabilityRating(const VkPhysicalDevice deviceToRate);
private:

    VkResult    loadLayersAndExtensions();
    bool        isValidationLayerSupported(const char *name);
    bool        isInstanceKHRExtensionSupported(const char *name);
    void        setupDebugFacilities();
    void        cleanUpDebugFacilities();

    VkResult vkEnumerateExtensionLayersAndExtensions();
    VkResult vkEnumerateKHRExtensions();

    static  vector<VkExtensionProperties>   vkEnumerateDeviceExtensions(const VkPhysicalDevice deviceToCheck);
    static  bool                            isDeviceExtensionSupported(const char *name, const VkPhysicalDevice deviceToCheck);
    static  bool                            checkDeviceExtensions(const VkPhysicalDevice deviceToCheck,
                                                                  vector<const char *> extensionNames);

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