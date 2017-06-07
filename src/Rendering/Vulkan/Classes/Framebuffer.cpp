//
// Created by hindrik on 10-4-17.
//

#include "Framebuffer.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

Framebuffer::Framebuffer(VkDevice device, const VkExtent2D swapchainExtent, const Renderpass& renderpass, const vector<VkImageView> &imageViews) : m_FrameBuffer({device, vkDestroyFramebuffer})
{
    VkFramebufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.renderPass = renderpass.renderpass();
    info.attachmentCount = static_cast<uint32_t >(imageViews.size());
    info.pAttachments = imageViews.data();
    info.width = swapchainExtent.width;
    info.height = swapchainExtent.height;
    info.layers = 1;

    VkResult result = vkCreateFramebuffer(device, &info, nullptr, m_FrameBuffer.reset());
    vkIfFailThrowMessage(result, "Failed to create frame buffer!");
}

VkFramebuffer Framebuffer::framebuffer() const
{
    return m_FrameBuffer;
}
