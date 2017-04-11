//
// Created by hindrik on 26-3-17.
//

#include <limits>
#include "Swapchain.h"

Swapchain::Swapchain(uint32_t width, uint32_t height, VkCore& vkCore, vk_queue presentQueue, VkSurfaceKHR surface) :
    m_PresentQueue(presentQueue),
    m_Height(height),
    m_Width(width),
    m_VkCore(vkCore),
    m_Surface(surface),
    m_Swapchain({m_VkCore.device(), vkDestroySwapchainKHR}),
    m_ImageAvailableSemaphore({m_VkCore.device(), vkDestroySemaphore}),
    m_RenderingFinishedSemaphore({m_VkCore.device(), vkDestroySemaphore})
{
    createSwapchain();
    retrieveSwapchainImages();
    createSwapchainImageViews();
    createSemaphores();
}


auto Swapchain::fillSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) -> vk_swapchain_details
{

    vk_swapchain_details details    = {};
    uint32_t formatCount            = 0;
    uint32_t presentModeCount       = 0;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.m_Capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    if(formatCount != 0)
    {
        details.m_Formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.m_Formats.data());
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if(presentModeCount != 0)
    {
        details.m_PresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.m_PresentModes.data());
    }

    return details;
}

auto Swapchain::checkSwapChainDetails(const vk_swapchain_details &details) -> bool
{
    return !details.m_PresentModes.empty() && !details.m_Formats.empty();
}

auto Swapchain::pickSwapChainSurfaceFormat(const vk_swapchain_details& details) -> VkSurfaceFormatKHR
{

    if(details.m_Formats.size() == 1 && details.m_Formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return VkSurfaceFormatKHR{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& format : details.m_Formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    return details.m_Formats[0];
}

auto Swapchain::pickSwapChainPresentMode(const vk_swapchain_details& details) -> VkPresentModeKHR
{

    for(const auto& presentMode : details.m_PresentModes)
    {
        if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return presentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

auto Swapchain::pickSwapChainExtent2D(const vk_swapchain_details& details, uint32_t width, uint32_t height) -> VkExtent2D
{

    if(details.m_Capabilities.currentExtent.width != std::numeric_limits<uint32_t >::max())
    {
        return details.m_Capabilities.currentExtent;
    }

    VkExtent2D newExtent = { width, height };
    newExtent.width = std::max(details.m_Capabilities.minImageExtent.width, std::min(details.m_Capabilities.maxImageExtent.width, newExtent.width));
    newExtent.height = std::max(details.m_Capabilities.minImageExtent.height, std::min(details.m_Capabilities.maxImageExtent.height, newExtent.height));

    return newExtent;
}

auto Swapchain::createSwapchain(VkSwapchainKHR oldSwapchain) -> void
{
    vk_swapchain_details details = fillSwapChainDetails(m_VkCore.physicalDevice(), m_Surface);
    if(!checkSwapChainDetails(details))
        throw std::runtime_error("Error, something wrong with the swapchain details.");

    m_SurfaceFormat = pickSwapChainSurfaceFormat(details);
    m_PresentMode = pickSwapChainPresentMode(details);
    m_Extent = pickSwapChainExtent2D(details, m_Width, m_Height);

    uint32_t imageCount = details.m_Capabilities.minImageCount + 1;

    if (details.m_Capabilities.maxImageCount > 0 && imageCount > details.m_Capabilities.maxImageCount) {
        imageCount = details.m_Capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};

    createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface                  = m_Surface;
    createInfo.pNext                    = nullptr;
    createInfo.minImageCount            = imageCount;
    createInfo.imageFormat              = m_SurfaceFormat.format;
    createInfo.imageColorSpace          = m_SurfaceFormat.colorSpace;
    createInfo.imageExtent              = m_Extent;
    createInfo.imageArrayLayers         = 1;
    createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount    = 0;
    createInfo.pQueueFamilyIndices      = nullptr;
    createInfo.preTransform             = details.m_Capabilities.currentTransform;
    createInfo.compositeAlpha           = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode              = m_PresentMode;
    createInfo.clipped                  = VK_TRUE;

    if(oldSwapchain == VK_NULL_HANDLE)
    {
        createInfo.oldSwapchain             = VK_NULL_HANDLE;
        if (vkCreateSwapchainKHR(m_VkCore.device(), &createInfo, nullptr, m_Swapchain.reset()) != VK_SUCCESS) {
            throw std::runtime_error("Error, failed to create swapchain!");
        }
    }
    else
    {
        createInfo.oldSwapchain = oldSwapchain;
        VkSwapchainKHR newSwapchain;

        if (vkCreateSwapchainKHR(m_VkCore.device(), &createInfo, nullptr, &newSwapchain) != VK_SUCCESS) {
            throw std::runtime_error("Error, failed to create swapchain!");
        }
        m_Swapchain = newSwapchain;
    }

}

auto Swapchain::retrieveSwapchainImages() -> void
{
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(m_VkCore.device(), m_Swapchain, &imageCount, nullptr);
    m_SwapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_VkCore.device(), m_Swapchain, &imageCount, m_SwapchainImages.data());
}

auto Swapchain::createSwapchainImageViews()  -> void
{
    m_SwapchainImageViews.resize(m_SwapchainImages.size());

    VkComponentMapping componentMapping = {};
    componentMapping.a = componentMapping.b = componentMapping.g = componentMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;

    for(uint32_t i = 0; i < static_cast<uint32_t >(m_SwapchainImageViews.size()); ++i)
    {
        VkImageViewCreateInfo createInfo            = {};
        createInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext                            = nullptr;
        createInfo.image                            = m_SwapchainImages[i];
        createInfo.viewType                         = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                           = m_SurfaceFormat.format;
        createInfo.components                       = componentMapping;

        createInfo.subresourceRange.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel    = 0;
        createInfo.subresourceRange.levelCount      = 1;
        createInfo.subresourceRange.baseArrayLayer  = 0;
        createInfo.subresourceRange.layerCount      = 1;

        VkImageView view;

        VkResult result = vkCreateImageView(m_VkCore.device(), &createInfo, nullptr, &view);
        vkIfFailThrowMessage(result, "Error creating VkImageView for swapchain!");
        m_SwapchainImageViews[i] = VkUniqueHandle<VkImageView>(view, m_VkCore.device(), vkDestroyImageView);
    }
}

vector<VkImageView> Swapchain::imageViews() const
{
    vector<VkImageView> vec;

    for(const auto& handle : m_SwapchainImageViews)
    {
        vec.push_back(handle.get());
    }

    return vec;
}

VkSwapchainKHR Swapchain::swapchain() const
{
    return m_Swapchain;
}

VkSurfaceFormatKHR Swapchain::surfaceFormat() const
{
    return m_SurfaceFormat;
}

VkPresentModeKHR Swapchain::presentMode() const
{
    return m_PresentMode;
}

VkExtent2D Swapchain::extent2D() const
{
    return m_Extent;
}

vk_queue Swapchain::presentQueue() const
{
    return m_PresentQueue;
}

VkViewport Swapchain::viewport() const
{
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent2D().width);
    viewport.height = static_cast<float>(extent2D().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    return viewport;
}

VkSemaphore Swapchain::imgAvailableSemaphore() const
{
    return m_ImageAvailableSemaphore;
}

VkSemaphore Swapchain::renderFinishedSemaphore() const
{
    return m_RenderingFinishedSemaphore;
}

auto Swapchain::createSemaphores() -> void
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkResult result = vkCreateSemaphore(m_VkCore.device(), &semaphoreInfo, nullptr, m_ImageAvailableSemaphore.reset());
    vkIfFailThrowMessage(result, "Semaphore creation failed!");
    result = vkCreateSemaphore(m_VkCore.device(), &semaphoreInfo, nullptr, m_RenderingFinishedSemaphore.reset());
    vkIfFailThrowMessage(result, "Semaphore creation failed!");
}

auto Swapchain::getAvailableImageIndex() -> uint32_t
{
    uint32_t index;
    vkAcquireNextImageKHR(m_VkCore.device(), m_Swapchain,UINT64_MAX, m_ImageAvailableSemaphore.get(), VK_NULL_HANDLE, &index);
    return index;
}

auto Swapchain::present(uint32_t presentIndex) -> void
{
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_RenderingFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_Swapchain;
    presentInfo.pImageIndices = &presentIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(m_PresentQueue.m_Queue, &presentInfo);
}

auto Swapchain::recreateSwapchain(uint32_t width, uint32_t height) -> void
{
    m_Width = width;
    m_Height = height;

    VkSwapchainKHR oldSwapper = m_Swapchain;
    createSwapchain(oldSwapper);
    retrieveSwapchainImages();
    createSwapchainImageViews();
}



























