//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H
#define VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H

#include <iostream>
#include <vulkan/vulkan.h>
#include "VulkanCoreStructs.h"

using std::ostream;

ostream& operator<<(ostream& o, VkPhysicalDeviceType& t);

ostream& operator<<(ostream& o, VkPhysicalDeviceProperties& p);

ostream& operator<<(ostream& o, VkQueueFamilyProperties p);

ostream& operator<<(ostream& o, vk_layer_extension_properties p);

ostream& operator<<(ostream& o, VkExtensionProperties p);









#endif //VULKANO_ENGINE_VULKANCOREUTILITYFUNCTIONS_H