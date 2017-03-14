//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H
#define VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H

#include <iostream>
#include "../VulkanPlatforms/VulkanPlatform.h"
#include "VulkanStructs.h"

using std::ostream;

ostream& operator<<(ostream& o, VkPhysicalDeviceType& t);

ostream& operator<<(ostream& o, VkPhysicalDeviceProperties& p);

ostream& operator<<(ostream& o, VkQueueFamilyProperties p);

ostream& operator<<(ostream& o, vk_layer_extension_properties p);

ostream& operator<<(ostream& o, VkExtensionProperties p);

vk_swapchain_details fillSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

bool checkSwapChainDetails(const vk_swapchain_details& details);

VkSurfaceFormatKHR pickSwapChainSurfaceFormat(const vk_swapchain_details& details);

VkPresentModeKHR pickSwapChainPresentMode(const vk_swapchain_details& details);

VkExtent2D pickSwapChainExtent2D(const vk_swapchain_details& details, uint32_t width, uint32_t height);

#endif //VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H