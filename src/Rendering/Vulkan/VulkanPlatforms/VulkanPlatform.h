//
// Created by hindrik on 28-1-17.
//

#ifndef VULKANO_ENGINE_VULKANPLATFORM_H
#define VULKANO_ENGINE_VULKANPLATFORM_H



#include <iostream>
#include <stdint.h>
#include <string>
#include <vulkan/vulkan.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include "FunctionWrapper.h"
#include "../../../Core/Logger.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>


using std::make_pair;
using std::pair;
using std::vector;
using std::string;
using std::function;
using std::remove;
using std::cout;
using std::endl;

////SELECT PLATFORM HERE////

#define VK_USE_PLATFORM_GLFW

////////////////////////////

typedef uint16_t WindowHandle;

class VulkanPlatform
{
private:
    string m_PlatformName;
    vector<FunctionWrapper> m_Callbacks;
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

    virtual vector<const char*> processExtensions(const vector<const char *>& instanceExtensions) = 0;
    virtual bool processAPI(WindowHandle window, float deltaTime) = 0;

    virtual pair<uint32_t,uint32_t> getWindowSize(WindowHandle handle) = 0;

    static void  resizeSwapchain(uint32_t width, uint32_t height, VulkanPlatform* platform);

    void addResizeCallback(function<void(uint32_t, uint32_t)> func, void* compare_ptr);
    void removeResizeCallback(void* compare_ptr);

private:
    void resizeSwapchain(uint32_t width, uint32_t height);

};

inline VulkanPlatform::VulkanPlatform(string platformName) : m_PlatformName(platformName)
{
    std::stringstream str;
    str << "Current platform: " << m_PlatformName;
    Logger::log(str.str());
}

inline VulkanPlatform::~VulkanPlatform()
{
}

inline string VulkanPlatform::getPlatformName()
{
    return m_PlatformName;
}

inline void VulkanPlatform::resizeSwapchain(uint32_t width, uint32_t height, VulkanPlatform* platform)
{
    if(platform != nullptr)
    {
        platform->resizeSwapchain(width, height);
    }
}

inline void VulkanPlatform::resizeSwapchain(uint32_t width, uint32_t height)
{
    for(auto&& func : m_Callbacks)
    {
        func(width, height);
    }
}

inline void VulkanPlatform::addResizeCallback(function<void(uint32_t, uint32_t)> func, void* compare_ptr)
{
    m_Callbacks.emplace_back(func, compare_ptr);
}

inline void VulkanPlatform::removeResizeCallback(void *compare_ptr)
{
    m_Callbacks.erase(remove(m_Callbacks.begin(), m_Callbacks.end(), compare_ptr), m_Callbacks.end());
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