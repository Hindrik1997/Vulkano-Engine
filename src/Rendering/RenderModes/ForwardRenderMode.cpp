//
// Created by hindrik on 8-4-17.
//

#include "ForwardRenderMode.h"
#include "../Vulkan/Classes/ShaderModule.h"
#include "../Vulkan/Classes/PipelineStateDescriptor.h"


void ForwardRenderMode::render(float deltaTime)
{

}

ForwardRenderMode::ForwardRenderMode(RenderTarget&& target) : RenderMode("Forward render mode", std::move(target)), m_TempLayout({ m_Target.vkCore().device(), vkDestroyPipelineLayout }), m_Commandpool(m_Target.vkCore().device(), m_Target.swapchain().presentQueue().m_FamilyIndex)
{
    uint16_t defaultRenderpass = createDefaultRenderpass();
    RenderpassResourceManager& resourceManager = m_Renderpasses[defaultRenderpass];

    ShaderModule vertex("vert.spv", ShaderModuleType::Vertex, m_Target.vkCore().device());
    ShaderModule fragment("frag.spv", ShaderModuleType::Fragment, m_Target.vkCore().device());

    PipelineStateDescriptor descriptor;
    descriptor.attachShader(vertex);
    descriptor.attachShader(fragment);
    descriptor.attachRenderpass(resourceManager.renderpass(), 0);
    descriptor.setViewportPropsFromSwapchain(m_Target.swapchain());

    descriptor.setInputAssemblerState(PipelineStateDescriptor::defaultInputAssemplyState());
    descriptor.setVertexInputState(PipelineStateDescriptor::defaultVertexInputState());
    descriptor.setRasterizerState(PipelineStateDescriptor::defaultRasterizerState());
    descriptor.setMultisampleState(PipelineStateDescriptor::predefinedMultisampleState(SampleCount::None));

    vector<VkPipelineColorBlendAttachmentState> vec = { PipelineStateDescriptor::defaultColorBlendAttachmentState() };
    descriptor.setColorBlendingState(PipelineStateDescriptor::defaultColorBlendState(vec));

    descriptor.setDynamicState(PipelineStateDescriptor::defaultDynamicState());

    PipelineStateDescriptor::defaultPipelineLayout(m_TempLayout.reset(), m_Target.vkCore().device());
    descriptor.setPipelineLayout(m_TempLayout);

    uint16_t handle = resourceManager.getNewPipelineStateObject(descriptor.createGraphicsPipeline(m_Target.vkCore().device()));

    uint32_t frameBufferCount = static_cast<uint32_t >(m_Target.swapchain().imageViews().size());
    for(uint32_t i = 0; i < frameBufferCount; ++i)
    {
        m_Framebuffers.push_back(Framebuffer(m_Target.vkCore().device(), m_Target.swapchain().extent2D(), resourceManager.renderpass(), vector<VkImageView>{m_Target.swapchain().imageViews()[i]}));
    }

    vector<VkCommandBuffer> buffers = m_Commandpool.allocateCommandBuffers(frameBufferCount, CommandBufferLevel::Primary);


    








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

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


    vector<VkAttachmentDescription> ad = { colorAttachment };
    vector<VkSubpassDescription> sd = { subpass };
    vector<VkSubpassDependency> sdy = { dependency };

    uint16_t t = m_Renderpasses.getNewItem(Renderpass(m_Target.vkCore().device(),ad,sd,sdy));
    return t;
}
