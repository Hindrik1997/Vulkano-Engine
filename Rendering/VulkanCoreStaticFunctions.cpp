//
// Created by hindrik on 12-2-17.
//

#include "VulkanCore.h"
#include "../Core/Console.h"

bool VulkanCore::vk_init_check_device(const VkPhysicalDevice deviceToCheck) {

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(deviceToCheck, &deviceProperties);
    vkGetPhysicalDeviceFeatures(deviceToCheck, &deviceFeatures);

    cout << deviceProperties << std::endl;

    vk_physical_device_info deviceInfo = vk_init_get_queue_families(deviceToCheck);

    bool supportsGraphics = false;

    for(auto& queueFamily : deviceInfo.m_queue_family_properties)
    {
        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            supportsGraphics = true;
            break;
        }
    }

    return supportsGraphics && (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) && deviceFeatures.geometryShader && deviceFeatures.tessellationShader;
}

int32_t VulkanCore::vk_init_suitability_rating(const VkPhysicalDevice deviceToRate) {

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

vk_physical_device_info VulkanCore::vk_init_get_queue_families(const VkPhysicalDevice& device) {

    uint32_t queueFamilyCount = 0;
    vector<VkQueueFamilyProperties> queueFamilies;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    if(queueFamilyCount == 0)
    {
        throw std::runtime_error("No device queue families found!");
    }

    queueFamilies.resize(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for(const auto& queueFamily : queueFamilies)
    {
        cout << queueFamily << std::endl;
    }

    vk_physical_device_info info;
    info.m_physical_device = device;
    info.m_queue_family_properties = queueFamilies;
    info.m_queue_family_count = static_cast<uint32_t >(queueFamilies.size());
    return info;
}