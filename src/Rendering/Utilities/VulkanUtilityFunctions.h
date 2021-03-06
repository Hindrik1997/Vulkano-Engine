//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H
#define VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H

#include <iostream>
#include "../Vulkan/VulkanPlatforms/VulkanPlatform.h"
#include "VulkanStructs.h"

using std::ostream;

ostream& operator<<(ostream& o, VkPhysicalDeviceType& t);

ostream& operator<<(ostream& o, VkPhysicalDeviceProperties& p);

ostream& operator<<(ostream& o, VkQueueFamilyProperties p);

ostream& operator<<(ostream& o, vk_layer_extension_properties p);

ostream& operator<<(ostream& o, VkExtensionProperties p);

ostream& operator<<(ostream& o, vk_memory_heap p);


void vkIfFailThrowMessage(VkResult result, std::string message);

string getVendorNameForID(uint32_t id);

uint32_t getSuitableMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);


#endif //VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H