//
// Created by hindrik on 26-3-17.
//

#ifndef VULKANOENGINE_SWAPCHAIN_H
#define VULKANOENGINE_SWAPCHAIN_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkCore.h"

class Swapchain final : NonCopyable {
private:
    VkCore&                             m_VkCore;
    VkSurfaceKHR                        m_Surface;
    VkUniqueHandle<VkSwapchainKHR>      m_Swapchain;
    vector<VkImage>                     m_SwapchainImages;
    vector<VkUniqueHandle<VkImageView>> m_SwapchainImageViews;
    VkUniqueHandle<VkSemaphore >        m_ImageAvailableSemaphore;
    VkUniqueHandle<VkSemaphore >        m_RenderingFinishedSemaphore;

    VkSurfaceFormatKHR  m_SurfaceFormat;
    VkPresentModeKHR    m_PresentMode;
    VkExtent2D          m_Extent            = {};

    vk_queue            m_PresentQueue      = {};
    uint32_t            m_Width             = 0;
    uint32_t            m_Height            = 0;
public:
    Swapchain(uint32_t width, uint32_t height, VkCore& vkCore, vk_queue presentQueue, VkSurfaceKHR surface);
private:
    auto        createSwapchain                 (VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE)                          -> void;
    auto        retrieveSwapchainImages         ()                                                                      -> void;
    auto        createSwapchainImageViews       ()                                                                      -> void;
    auto        createSemaphores                ()                                                                      -> void;


public:
    auto        present                         (uint32_t presentIndex)                                                 -> void;
    auto        getAvailableImageIndex          ()                                                                      -> uint32_t;
    auto        recreateSwapchain               (uint32_t width, uint32_t height)                                       -> void;

    static auto fillSwapChainDetails            (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)                 -> vk_swapchain_details;
    static auto checkSwapChainDetails           (const vk_swapchain_details &details)                                   -> bool;
    static auto pickSwapChainSurfaceFormat      (const vk_swapchain_details& details)                                   -> VkSurfaceFormatKHR;
    static auto pickSwapChainPresentMode        (const vk_swapchain_details& details)                                   -> VkPresentModeKHR;
    static auto pickSwapChainExtent2D           (const vk_swapchain_details& details, uint32_t width, uint32_t height)  -> VkExtent2D;

public:
    VkSwapchainKHR      swapchain() const;
    VkSurfaceFormatKHR  surfaceFormat() const;
    VkPresentModeKHR    presentMode() const;
    VkExtent2D          extent2D() const;
    vector<VkImageView> imageViews() const;
    vk_queue            presentQueue() const;
    VkViewport          viewport() const;
    VkSemaphore         imgAvailableSemaphore() const;
    VkSemaphore         renderFinishedSemaphore() const;


};


#endif //VULKANOENGINE_SWAPCHAIN_H
