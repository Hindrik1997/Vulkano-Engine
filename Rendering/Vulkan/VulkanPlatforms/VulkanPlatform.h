//
// Created by hindrik on 28-1-17.
//

#ifndef VULKANO_ENGINE_VULKANPLATFORM_H
#define VULKANO_ENGINE_VULKANPLATFORM_H

#define WIDTH 800
#define HEIGHT 600

#include <stdint.h>

////SELECT PLATFORM HERE////

#define VK_USE_PLATFORM_GLFW

////////////////////////////

typedef uint16_t WindowHandle;

#if defined(VK_USE_PLATFORM_GLFW)
    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>
    #include "VK_GLFW_PLATFORM.h"
#endif

#if defined(VK_USE_PLATFORM_XCB_KHR)
    #include "VK_XCB_PLATFORM.h"
#endif

#ifndef VK_USE_PLATFORM_GLFW
    #include <vulkan/vulkan.h>
#endif

#endif //VULKANO_ENGINE_VULKANPLATFORM_H