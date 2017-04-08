//
// Created by hindrik on 8-4-17.
//

#include "ForwardRenderMode.h"

void ForwardRenderMode::render(float deltaTime)
{

}

ForwardRenderMode::ForwardRenderMode(RenderTarget &&target) : RenderMode("Forward render mode", std::move(target))
{
    uint16_t defaultRenderpass = createDefaultRenderpass();


}

uint16_t ForwardRenderMode::createDefaultRenderpass()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format              = m_Target.swapchain().surfaceFormat().format;
    colorAttachment.samples             = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp              = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp             = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp       = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp      = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout       = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout         = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    vector<VkAttachmentDescription> ad = { colorAttachment };
    vector<VkSubpassDescription> sd = { subpass };

    uint16_t t = m_Renderpasses.getNewItem(Renderpass(m_Target.vkCore().device(),ad,sd));
    return t;
}
