//
// Created by hindrik on 18-2-17.
//

#ifndef VULKANOENGINE_VK_GLFW_PLATFORM_H
#define VULKANOENGINE_VK_GLFW_PLATFORM_H

#include "VulkanPlatform.h"
#include <vector>

using std::vector;

#define VK_PLATFORM VK_GLFW_PLATFORM

class VK_GLFW_PLATFORM {
private:
    GLFWwindow* m_Window;

public:
    VK_GLFW_PLATFORM();
public:
    void ProcessExtensions(vector<const char*>& instanceExtensions);
    VkResult CreateSurface(VkInstance instance, VkSurfaceKHR& surface);
    bool ProcessAPI(float deltaTime);
};


#endif //VULKANOENGINE_VK_GLFW_PLATFORM_H
