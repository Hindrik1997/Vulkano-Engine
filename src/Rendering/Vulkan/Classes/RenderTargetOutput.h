//
// Created by hindrik on 26-3-17.
//

#ifndef VULKANOENGINE_RENDERTARGETOUTPUT_H
#define VULKANOENGINE_RENDERTARGETOUTPUT_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkCore.h"
#include "Swapchain.h"
#include <memory>

using std::make_unique;
using std::unique_ptr;

class RenderTargetOutput final : NonCopyable {
private:
    VkCore&                         m_VkCore;
    VK_PLATFORM&                    m_Platform;
    WindowHandle                    m_Window;
    VkUniqueHandle<VkSurfaceKHR>    m_Surface;
    vk_queue                        m_PresentQueue      = {};

    unique_ptr<Swapchain>           m_CurrentSwapchain;
public:
    RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform, vk_queue presentQueue);
    RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform);
    ~RenderTargetOutput();

    RenderTargetOutput(RenderTargetOutput&&) = default;
    RenderTargetOutput& operator=(RenderTargetOutput&&) = default;
public:
    VkSurfaceKHR surface();
    Swapchain&   swapchain();
private:
    void            createSwapchain(uint32_t width, uint32_t height);
    static VkResult createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle, VK_PLATFORM& platform);
    static vk_queue getPresentCapableQueue(VkSurfaceKHR surface, VkCore& vkCore);
};


#endif //VULKANOENGINE_RENDERTARGETOUTPUT_H
