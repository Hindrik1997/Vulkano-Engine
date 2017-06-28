//
// Created by hindrik on 8-4-17.
//

#include "RenderOutput.h"
#include "../Utilities/VulkanUtilityFunctions.h"

using std::chrono::nanoseconds;


RenderOutput::RenderOutput(VK_PLATFORM& platform, VkInstance instance, uint32_t width, uint32_t height) : m_Platform(&platform), m_Instance(instance), m_Surface({instance, vkDestroySurfaceKHR}), m_Window(platform)
{
    m_Window.reset(platform.createWindow(width, height, "Vulkano Engine"));
    VkResult result = platform.createSurface(m_Instance, *m_Surface.reset(), m_Window);
    vkIfFailThrowMessage(result, "Surface creation failed!");
}

bool RenderOutput::processAPI(nanoseconds deltaTime)
{
    return m_Platform->processAPI(m_Window, deltaTime);
}

VkSurfaceKHR RenderOutput::surface()
{
    return m_Surface;
}
