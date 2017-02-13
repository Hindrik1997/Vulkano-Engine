//
// Created by hindrik on 28-1-17.
//

#ifndef VULKANO_ENGINE_VULKANPLATFORM_H
#define VULKANO_ENGINE_VULKANPLATFORM_H

#define WIDTH 800
#define HEIGHT 600

////SELECT PLATFORM HERE////

#define VK_USE_PLATFORM_XCB_KHR

////////////////////////////


#if defined(VK_USE_PLATFORM_XCB_KHR)

#include "VK_XCB_PLATFORM.h"

#endif


#include <vulkan/vulkan.h>

#endif //VULKANO_ENGINE_VULKANPLATFORM_H