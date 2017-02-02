//
// Created by hindrik on 27-1-17.
//

#include <iostream>
#include "VulkanCore.h"


VulkanCore::VulkanCore(string applicationName, vector<string> enabledKHRInstanceExtensions, vector<string> enabledInstanceValidationLayers, bool enableValidationLayers)
        : m_application_name(applicationName) , m_layerAndExtensionManager(enabledKHRInstanceExtensions,enabledInstanceValidationLayers, enableValidationLayers)
{
    VkResult r = vk_init();
    if(r == VK_SUCCESS)
        std::cout << "Initialized Vulkan API" << std::endl;
}

VulkanCore::~VulkanCore() {
}

VkResult VulkanCore::vk_init() {
    VkResult result;

    result = vk_init_instance();
    VK_CHECK(result);

    return result;
}

bool VulkanCore::process_platform_API(float deltaTime) {
    return false;
    //return platform->processAPI(deltaTime);
}

VkResult VulkanCore::vk_init_instance() {

    VkResult result;

    VkApplicationInfo applicationInfo   = {};

    applicationInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext               = nullptr;
    applicationInfo.pApplicationName    = m_application_name.c_str();
    applicationInfo.applicationVersion  = VK_MAKE_VERSION(1,0,0);
    applicationInfo.pEngineName         = "Vulkano Engine";
    applicationInfo.engineVersion       = VK_MAKE_VERSION(0,0,1);
    applicationInfo.apiVersion          = VK_MAKE_VERSION(1,0,0);

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;








}

