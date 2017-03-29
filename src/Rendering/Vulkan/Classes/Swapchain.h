//
// Created by hindrik on 26-3-17.
//

#ifndef VULKANOENGINE_SWAPCHAIN_H
#define VULKANOENGINE_SWAPCHAIN_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkCore.h"

class Swapchain : NonCopyable {
private:
    VkCore&                             m_VkCore;
    VkUniqueHandle<VkSwapchainKHR>      m_Swapchain;
    VkUniqueHandle<VkSurfaceKHR>        m_Surface;

    VkSurfaceFormatKHR  m_SurfaceFormat;
    VkPresentModeKHR    m_PresentMode;
    VkExtent2D          m_Extent            = {};

    vk_queue            m_PresentQueue      = {};
    uint32_t            m_Width             = 0;
    uint32_t            m_Height            = 0;
public:
    Swapchain(uint32_t width, uint32_t height, VkCore& vkCore, vk_queue presentQueue, VkSurfaceKHR surface);
public:
    auto        createSwapchain                 ()                                                                      -> void;

    static auto fillSwapChainDetails            (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)                 -> vk_swapchain_details;
    static auto checkSwapChainDetails           (const vk_swapchain_details &details)                                   -> bool;
    static auto pickSwapChainSurfaceFormat      (const vk_swapchain_details& details)                                   -> VkSurfaceFormatKHR;
    static auto pickSwapChainPresentMode        (const vk_swapchain_details& details)                                   -> VkPresentModeKHR;
    static auto pickSwapChainExtent2D           (const vk_swapchain_details& details, uint32_t width, uint32_t height)  -> VkExtent2D;

};


#endif //VULKANOENGINE_SWAPCHAIN_H
