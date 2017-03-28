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

inline void vk_if_fail_throw_message(VkResult result, std::string message){
    if(result != VK_SUCCESS)
        throw std::runtime_error(message);
}

#endif //VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H