//
// Created by hindrik on 18-2-17.
//

#include "VK_GLFW_PLATFORM.h"
#include <string>
#include <cstring>
#include <stdexcept>

using std::string;

static void onWindowResized(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0) return;

    VulkanPlatform* platform = reinterpret_cast<VulkanPlatform*>(glfwGetWindowUserPointer(window));
    platform->resizeSwapchain(static_cast<uint32_t >(width), static_cast<uint32_t>(height), platform);
}




VK_GLFW_PLATFORM::VK_GLFW_PLATFORM() : VulkanPlatform("GLFW")
{

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    if(glfwVulkanSupported() != GLFW_TRUE)
        Logger::failure("Error, Vulkan support not found!");

}

vector<const char*> VK_GLFW_PLATFORM::processExtensions(const vector<const char *> & instanceExtensions) {

    vector<const char*> additionalExtensions;

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
        additionalExtensions.push_back(extensions[i]);
    }
    return additionalExtensions;
}

WindowHandle VK_GLFW_PLATFORM::createWindow(uint32_t windowWidth, uint32_t windowHeight, string windowTitle)
{
    WindowHandle handle = m_Windows.getNewItem(nullptr);
    m_Windows[handle] = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(m_Windows[handle], this);
    glfwSetWindowSizeCallback(m_Windows[handle], onWindowResized);

    return handle;
}

VkResult VK_GLFW_PLATFORM::createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle) {

    GLFWwindow* window = m_Windows[handle];
    return glfwCreateWindowSurface(instance, window, nullptr, &surface);
}

VK_GLFW_PLATFORM::~VK_GLFW_PLATFORM()
{
    glfwTerminate();
}

bool VK_GLFW_PLATFORM::processAPI(WindowHandle window, nanoseconds deltaTime)
{
    GLFWwindow* pointer = m_Windows[static_cast<uint16_t >(window)];
    bool shouldClose = glfwWindowShouldClose(pointer) > 0;
    if(shouldClose)
        return false;

    if(deltaTime > nanoseconds(0))
    {
        float fps = std::chrono::duration_cast<nanoseconds>(std::chrono::seconds(1)) / deltaTime;
        int64_t frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count();

        std::chrono::microseconds fraction  = std::chrono::duration_cast<std::chrono::microseconds>(deltaTime) - std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime));

        string c = "Vulkano Engine ";
        c.append(std::to_string(static_cast<int>(fps)));
        c.append(" FPS - ");
        c.append(std::to_string(frameTime));
        c.append(",");
        c.append(std::to_string(fraction.count()));
        c.append("ms");
        glfwSetWindowTitle(pointer, c.c_str());
    }

    glfwWaitEvents();
	//glfwPollEvents();

    return true;
}

void VK_GLFW_PLATFORM::destroyWindow(WindowHandle handle)
{
    GLFWwindow* pointer = m_Windows[static_cast<uint16_t >(handle)];
    glfwDestroyWindow(pointer);
    m_Windows.removeItem(static_cast<uint16_t >(handle));
}

pair<uint32_t, uint32_t> VK_GLFW_PLATFORM::getWindowSize(WindowHandle handle)
{
    GLFWwindow* pointer = m_Windows[static_cast<uint16_t >(handle)];
    int x,y;
    glfwGetWindowSize(pointer, &x,&y);

    return make_pair(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
}
