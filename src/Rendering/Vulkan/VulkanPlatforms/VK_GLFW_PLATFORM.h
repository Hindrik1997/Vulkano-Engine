//
// Created by hindrik on 18-2-17.
//

#ifndef VULKANOENGINE_VK_GLFW_PLATFORM_H
#define VULKANOENGINE_VK_GLFW_PLATFORM_H

#include "VulkanPlatform.h"
#include "../../../Data Storage/CacheOptimizedStorage.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

#define VK_PLATFORM VK_GLFW_PLATFORM


class VK_GLFW_PLATFORM : VulkanPlatform {
private:
    CacheOptimizedStorage<GLFWwindow*, 64> m_Windows;
    GLFWwindow* m_Window;
public:
    VK_GLFW_PLATFORM();
    ~VK_GLFW_PLATFORM();
public:
    WindowHandle createWindow(uint32_t windowWidth, uint32_t windowHeight, string windowTitle);
    void destroyWindow(WindowHandle handle);

    VkResult createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle);

    void processExtensions(vector<const char *> &instanceExtensions);
    bool processAPI(WindowHandle window, float deltaTime);
};


#endif //VULKANOENGINE_VK_GLFW_PLATFORM_H