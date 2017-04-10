//
// Created by hindrik on 10-4-17.
//

#ifndef VULKANOENGINE_FRAMEBUFFER_H
#define VULKANOENGINE_FRAMEBUFFER_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkUniqueHandle.h"
#include "Renderpass.h"

class Framebuffer final
{
private:
    VkUniqueHandle<VkFramebuffer> m_FrameBuffer;
public:
    Framebuffer(VkDevice device, const VkExtent2D swapchainExtent, const Renderpass& renderpass, const vector<VkImageView>& imageViews);

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&&) = default;

    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer& operator=(Framebuffer&&) = default;

public:
    VkFramebuffer framebuffer() const;
};


#endif //VULKANOENGINE_FRAMEBUFFER_H
