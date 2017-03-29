//
// Created by hindrik on 26-3-17.
//

#ifndef VULKANOENGINE_RENDERTARGETOUTPUT_H
#define VULKANOENGINE_RENDERTARGETOUTPUT_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkCore.h"
#include "Swapchain.h"

class RenderTargetOutput : NonCopyable {
private:
    VkCore&         m_VkCore;
    VK_PLATFORM&    m_Platform;
    WindowHandle    m_Window;
    VkSurfaceKHR    m_Surface           = VK_NULL_HANDLE;
    vk_queue        m_PresentQueue      = {};
    Swapchain*      m_CurrentSwapchain  = nullptr;
    Swapchain*      m_OldSwapchain      = nullptr;
public:
    RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform, vk_queue presentQueue);
    RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform);
    ~RenderTargetOutput();

    RenderTargetOutput(RenderTargetOutput&&);
    RenderTargetOutput& operator=(RenderTargetOutput&&) = delete;
public:
    VkSurfaceKHR surface();

private:
    static VkResult createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle, VK_PLATFORM& platform);
    static vk_queue getPresentCapableQueue(VkSurfaceKHR surface, VkCore& vkCore);
};


#endif //VULKANOENGINE_RENDERTARGETOUTPUT_H
