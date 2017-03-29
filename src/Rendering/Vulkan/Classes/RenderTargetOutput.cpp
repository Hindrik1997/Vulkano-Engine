//
// Created by hindrik on 26-3-17.
//

#include "RenderTargetOutput.h"

RenderTargetOutput::RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform, vk_queue presentQueue) : m_VkCore(vkCore), m_Platform(platform), m_PresentQueue(presentQueue)
{
    m_Window = platform.createWindow(windowWidth, windowHeight, "Vulkan");
    VkResult result = createSurface(m_VkCore.instance(), m_Surface, m_Window, platform);
    vkIfFailThrowMessage(result, "Surface creation failed!");

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_VkCore.physicalDevice(), m_PresentQueue.m_FamilyIndex, m_Surface, &presentSupport);
    if(!presentSupport)
        throw std::runtime_error("Error, selected queue does not support presentation!");
}

RenderTargetOutput::~RenderTargetOutput()
{
    if(m_Surface != VK_NULL_HANDLE)
       vkDestroySurfaceKHR(m_VkCore.instance(), m_Surface, nullptr);
}

VkResult RenderTargetOutput::createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle, VK_PLATFORM& platform) {
    return platform.createSurface(instance, surface, handle);
}

VkSurfaceKHR RenderTargetOutput::surface() {
    return m_Surface;
}

RenderTargetOutput::RenderTargetOutput(RenderTargetOutput && temp) : m_Surface(temp.m_Surface), m_VkCore(temp.m_VkCore), m_Platform(temp.m_Platform), m_Window(temp.m_Window), m_PresentQueue(temp.m_PresentQueue), m_CurrentSwapchain(temp.m_CurrentSwapchain), m_OldSwapchain(temp.m_OldSwapchain)
{
    temp.m_OldSwapchain = nullptr;
    temp.m_CurrentSwapchain = nullptr;
    temp.m_Surface = VK_NULL_HANDLE;
}

RenderTargetOutput::RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore &vkCore, VK_PLATFORM &platform) : m_VkCore(vkCore), m_Platform(platform)
{
    m_Window = platform.createWindow(windowWidth, windowHeight, "Vulkan");
    VkResult result = createSurface(m_VkCore.instance(), m_Surface, m_Window, platform);
    vkIfFailThrowMessage(result, "Surface creation failed!");

    m_PresentQueue = getPresentCapableQueue(m_Surface, m_VkCore);
}

auto RenderTargetOutput::getPresentCapableQueue(VkSurfaceKHR surface, VkCore &vkCore) -> vk_queue
{
    VkBool32 presentSupport = VK_FALSE;
    for(vk_queue q : vkCore.graphicsQueues())
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(vkCore.physicalDevice(), q.m_FamilyIndex, surface, &presentSupport);
        if(presentSupport == VK_TRUE)
        {
            return q;
        }
    }
    throw std::runtime_error("No supported surface found!");
}
