//
// Created by hindrik on 27-1-17.
//

#include <iostream>
#include <algorithm>
#include "VulkanCore.h"
#include "../Core/Console.h"


VulkanCore::VulkanCore(string applicationName, vector<const char*> enabledKHRInstanceExtensions, vector<const char*> enabledInstanceValidationLayers, bool enableValidationLayers)
        : m_application_name(applicationName) , m_layerAndExtensionManager(enabledKHRInstanceExtensions,enabledInstanceValidationLayers, enableValidationLayers)
{
    VkResult r = vk_init();
    if(r == VK_SUCCESS)
        Console::printLine("Initialized Vulkan API succesfully.");
}

VulkanCore::~VulkanCore() {
    m_layerAndExtensionManager.cleanupDebugFacilities();
    vkDestroyInstance(m_instance,nullptr);
}

VkResult VulkanCore::vk_init() {
    VkResult result;

    result = vk_init_instance();
    VK_CHECK(result);

    //Enable debug if necessary
    if(m_layerAndExtensionManager.getDebugEnabled())
    {
        m_layerAndExtensionManager.setupDebugFacilities();
    }

    result = vk_init_device();
    VK_CHECK(result);

    return result;
}

bool VulkanCore::process_platform_API(float deltaTime) {
    return false;
    //return platform->processAPI(deltaTime);
}

VkResult VulkanCore::vk_init_instance() {

    VkResult result;

    VkApplicationInfo applicationInfo = {};

    vector<const char *> enabledExtensions = m_layerAndExtensionManager.getEnabledInstanceKHRExtensions();
    vector<const char *> enabledLayers = m_layerAndExtensionManager.getEnabledInstanceValidationLayers();

    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = m_application_name.c_str();
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "Vulkano Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t >(enabledExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t >(enabledLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);

    if (result == VK_SUCCESS)
    {
        m_layerAndExtensionManager.setVkInstanceHandle(m_instance);
    }

    return result;
}

VkResult VulkanCore::vk_init_device() {
    VkResult result;

    uint32_t physical_device_count = 0;
    vector<VkPhysicalDevice> physical_devices;
    VkPhysicalDevice selected_device = VK_NULL_HANDLE;


    vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);

    if(physical_device_count <= 0) {
        throw std::runtime_error("Error, no physical devices supporting Vulkan found!.");
    }

    physical_devices.resize(physical_device_count);

    result = vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());
    VK_IF_FAIL_MSG(result, "Error when retrieving physical devices.");

    vector<VkPhysicalDevice> possibleGPUs;

    for(auto& device : physical_devices)
    {
        if(vk_init_check_device(device))
        {
            possibleGPUs.push_back(device);
        }
    }

    if(possibleGPUs.size() == 0)
    {
        throw::std::runtime_error("Failed to find a suitable GPU. The GPU needs to be a discrete or integrated GPU, and needs to support geometry shaders and tesselation.");
    }

    vector<vk_device_rating> ratings;

    for(auto& device : possibleGPUs)
    {
        ratings.push_back(vk_device_rating{device,vk_init_suitability_rating(device)});
    }

    std::sort(ratings.begin(), ratings.end(), [](const vk_device_rating& left, const vk_device_rating& right){ return right.m_rating < left.m_rating; });

    selected_device = ratings.front().m_device;

    if(selected_device == VK_NULL_HANDLE)
    {
        throw::std::runtime_error("Selected device is a VK_NULL_HANDLE! ");
    }






    return result;
}

bool VulkanCore::vk_init_check_device(VkPhysicalDevice deviceToCheck) {

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(deviceToCheck, &deviceProperties);
    vkGetPhysicalDeviceFeatures(deviceToCheck, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU && deviceFeatures.geometryShader && deviceFeatures.tessellationShader;
}

int32_t VulkanCore::vk_init_suitability_rating(VkPhysicalDevice deviceToRate) {

    int32_t rating = 0;

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(deviceToRate, &deviceProperties);
    vkGetPhysicalDeviceFeatures(deviceToRate, &deviceFeatures);

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        rating += 1000;

    rating += deviceProperties.limits.maxImageDimension2D;

    return rating;
}

