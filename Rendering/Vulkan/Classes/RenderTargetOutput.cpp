//
// Created by hindrik on 26-3-17.
//

#include "RenderTargetOutput.h"

RenderTargetOutput::RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform) : m_VkCore(vkCore), m_Platform(platform)
{
    m_Window = platform.createWindow(windowWidth, windowHeight, "Vulkan");
    VkResult result = createSurface(m_VkCore.instance(),m_Surface, m_Window, platform);
    vk_if_fail_throw_message(result, "Surface creation failed!");
}

RenderTargetOutput::~RenderTargetOutput()
{
    if(m_Surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(m_VkCore.instance(), m_Surface, nullptr);
}

VkResult RenderTargetOutput::createSurface(VkInstance instance, VkSurfaceKHR &surface, WindowHandle handle, VK_PLATFORM& platform) {
    return platform.createSurface(instance, surface, handle);
}