//
// Created by hindrik on 18-2-17.
//

#include "VK_GLFW_PLATFORM.h"
#include <string>
#include <cstring>
#include <stdexcept>

using std::string;

VkResult VK_GLFW_PLATFORM::CreateSurface(VkInstance instance, VkSurfaceKHR& surface) {

    return glfwCreateWindowSurface(instance, m_Window, nullptr, &surface);
}

VK_GLFW_PLATFORM::VK_GLFW_PLATFORM() {

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkano Engine", nullptr, nullptr);

    if(glfwVulkanSupported() != GLFW_TRUE)
        throw std::runtime_error("Vulkan loader not found!");

}

bool VK_GLFW_PLATFORM::ProcessAPI(float deltaTime) {

    bool shouldClose = glfwWindowShouldClose(m_Window) > 0;
    if(shouldClose)
        return false;

    glfwPollEvents();

    if(deltaTime > 0.0f)
    {
        float fps = 1000.0f / deltaTime;
        string c = "Vulkano Engine ";
        c.append(std::to_string(static_cast<int>(fps)));
        c.append(" FPS");
        glfwSetWindowTitle(m_Window, c.c_str());
    }

    return true;
}

void VK_GLFW_PLATFORM::ProcessExtensions(vector<const char *>& instanceExtensions) {
    uint32_t glfwExtensionCount = 0;
    const char** extensions;

    extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for(uint32_t i = 0; i < glfwExtensionCount; ++i)
    {
        bool isFound = false;
        for(uint32_t j = 0; j < static_cast<uint32_t >(instanceExtensions.size()); ++j)
        {
            if(isFound)
                continue;

            if(strcmp(instanceExtensions[j], extensions[i]) == 0)
                isFound = true;
        }

        if(isFound)
            continue;
        instanceExtensions.push_back(extensions[i]);
    }
}
