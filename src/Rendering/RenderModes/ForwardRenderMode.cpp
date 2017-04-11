//
// Created by hindrik on 8-4-17.
//

#include "ForwardRenderMode.h"
#include "../Vulkan/Classes/ShaderModule.h"
#include "../Vulkan/Classes/PipelineStateDescriptor.h"




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

    m_Buffers = m_Commandpool.allocateCommandBuffers(frameBufferCount, CommandBufferLevel::Primary);

    for(uint32_t i = 0; i < static_cast<uint32_t >(m_Buffers.size()); ++i)
    {
        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        vkBeginCommandBuffer(m_Buffers[i], &commandBufferBeginInfo);

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = resourceManager.renderpass().renderpass();
        renderPassBeginInfo.framebuffer = m_Framebuffers[i].framebuffer();
        renderPassBeginInfo.renderArea.offset = {0,0};
        renderPassBeginInfo.renderArea.extent = m_Target.swapchain().extent2D();

        VkClearValue clearColorValue = {0.0f,0.0f,0.0f,1.0f};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColorValue;

        vkCmdBeginRenderPass(m_Buffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_Buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, resourceManager.getPSObyHandle(handle).pipeline());

        VkViewport v = m_Target.swapchain().viewport();

        vkCmdSetViewport(m_Buffers[i], 0,1, &v);

        vkCmdDraw(m_Buffers[i], 3,1,0,0);

        vkCmdEndRenderPass(m_Buffers[i]);

        VkResult result = vkEndCommandBuffer(m_Buffers[i]);
        vkIfFailThrowMessage(result, "Error occured during recording of command buffer!");
    }

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

void ForwardRenderMode::render(float deltaTime)
{
    uint32_t swapImageIndex = m_Target.swapchain().getAvailableImageIndex();

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_Target.swapchain().imgAvailableSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_Buffers[swapImageIndex];
    VkSemaphore signalSemaphores[] = {m_Target.swapchain().renderFinishedSemaphore()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_Target.swapchain().presentQueue().m_Queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    m_Target.swapchain().present(swapImageIndex);

}

ForwardRenderMode::~ForwardRenderMode()
{
    vkDeviceWaitIdle(m_Target.vkCore().device());
}