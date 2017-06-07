//
// Created by hindrik on 26-3-17.
//

#include "RenderTarget.h"
#include "../../../Core/Logger.h"

RenderTarget::RenderTarget(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform, vk_queue presentQueue)
        : m_VkCore(vkCore), m_Platform(platform), m_PresentQueue(presentQueue), m_Surface({m_VkCore.instance(), vkDestroySurfaceKHR}), m_Window(m_Platform)
{
    m_Window.reset(platform.createWindow(windowWidth, windowHeight, "Vulkan"));


    VkResult result = createSurface(m_VkCore.instance(), *m_Surface.reset(), m_Window, platform);
    vkIfFailThrowMessage(result, "Surface creation failed!");

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_VkCore.physicalDevice(), m_PresentQueue.m_FamilyIndex, m_Surface, &presentSupport);
    if(!presentSupport)
        Logger::failure("Error, selected queue does not support presentation!");

    createSwapchain(windowWidth, windowHeight);
}

RenderTarget::RenderTarget(uint32_t windowWidth, uint32_t windowHeight, VkCore &vkCore, VK_PLATFORM &platform)
        : m_VkCore(vkCore), m_Platform(platform), m_Surface({m_VkCore.instance(), vkDestroySurfaceKHR}), m_Window(m_Platform)
{
    m_Window.reset(platform.createWindow(windowWidth, windowHeight, "Vulkan"));

    VkResult result = createSurface(m_VkCore.instance(), *m_Surface.reset(), m_Window, platform);
    vkIfFailThrowMessage(result, "Surface creation failed!");

    m_PresentQueue = getPresentCapableQueue(m_Surface, m_VkCore);
    createSwapchain(windowWidth, windowHeight);
}

VkResult RenderTarget::createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle, VK_PLATFORM& platform)
{
    return platform.createSurface(instance, surface, handle);
}

VkSurfaceKHR RenderTarget::surface() {
    return m_Surface;
}

auto RenderTarget::getPresentCapableQueue(VkSurfaceKHR surface, VkCore &vkCore) -> vk_queue
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
    Logger::failure("No supported surface found!");
    throw;
}


void RenderTarget::createSwapchain(uint32_t width, uint32_t height)
{
    m_Swapchain.reset(new Swapchain(width,height, m_VkCore, m_PresentQueue, m_Surface));
}

Swapchain &RenderTarget::swapchain()
{
    return *m_Swapchain;
}

bool RenderTarget::processAPI(float deltaTime)
{
    return m_Platform.processAPI(m_Window,deltaTime);
}

VkCore &RenderTarget::vkCore()
{
    return m_VkCore;
}

VK_PLATFORM &RenderTarget::platform()
{
    return m_Platform;
}

WindowHandleWrapper& RenderTarget::window()
{
    return m_Window;
}
