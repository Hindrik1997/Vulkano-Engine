//
// Created by hindrik on 28-1-17.
//

#ifndef VULKANO_ENGINE_VULKANPLATFORM_H
#define VULKANO_ENGINE_VULKANPLATFORM_H

#include <stdint.h>
#include <string>
#include <vulkan/vulkan.h>
#include <vector>

using std::vector;
using std::string;

////SELECT PLATFORM HERE////

#define VK_USE_PLATFORM_GLFW

////////////////////////////

typedef uint16_t WindowHandle;

class VulkanPlatform
{
private:
    string m_PlatformName;
protected:
    VulkanPlatform(string platformName);
    virtual ~VulkanPlatform() = 0;

    VulkanPlatform(const VulkanPlatform&)   = delete;
    VulkanPlatform(VulkanPlatform&&)        = delete;

    VulkanPlatform& operator=(const VulkanPlatform&) = delete;
    VulkanPlatform& operator=(VulkanPlatform&&)      = delete;
public:

    string getPlatformName();

    virtual WindowHandle createWindow(uint32_t windowWidth, uint32_t windowHeight, string windowTitle) = 0;
    virtual void destroyWindow(WindowHandle handle) = 0;

    virtual VkResult createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle) = 0;

    virtual void processExtensions(vector<const char *> &instanceExtensions) = 0;
    virtual bool processAPI(WindowHandle window, float deltaTime) = 0;
};

inline VulkanPlatform::VulkanPlatform(string platformName) : m_PlatformName(platformName)
{
}

inline VulkanPlatform::~VulkanPlatform()
{
}

inline string VulkanPlatform::getPlatformName()
{
    return m_PlatformName;
}

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