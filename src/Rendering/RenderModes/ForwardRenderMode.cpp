//
// Created by hindrik on 8-4-17.
//


#include <chrono>
#include "ForwardRenderMode.h"
#include "../Vulkan/Classes/ShaderModule.h"
#include "../Vulkan/Classes/PipelineStateDescriptor.h"
#include "../Vulkan/Classes/Vertex.h"
#include "../Vulkan/Classes/Images/Image2D.h"
#include "../../Core/Logger.h"


ForwardRenderMode::ForwardRenderMode(RenderTarget&& target, Engine& engine) : RenderMode("Forward render mode", std::move(target), engine), m_TempLayout({ m_Target.vkCore().device(), vkDestroyPipelineLayout }), m_Commandpool(m_Target.vkCore().device(), m_Target.swapchain().presentQueue().m_FamilyIndex), m_ComparePtr(this), m_DescriptorSetLayout({m_Target.vkCore().device(), vkDestroyDescriptorSetLayout}), m_DescriptorPool({m_Target.vkCore().device(), vkDestroyDescriptorPool})
{
    m_Target.platform().addResizeCallback([this](uint32_t width, uint32_t height){ recreateSwapchain(width, height); }, m_ComparePtr);

    createDescriptorSetLayout();
    createUniformBuffer();
    createDescriptorPool();
    addDescriptorSet();
    createVertexBuffer();
    createRenderpass();
    createPipeline();
    createFramebuffers();
    createCommandbuffers();

    int width, height, texChannels;
    stbi_uc* pixels = stbi_load("textures/stone_grid.jpg", &width, &height,&texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = static_cast<VkDeviceSize>(width * height * 4);

    if(!pixels)
        Logger::failure("Error loading image file!");

    uint32_t index = m_Target.swapchain().presentQueue().m_FamilyIndex;
    vector<uint32_t> t = m_Target.vkCore().transferQueueFamilies();
    t.push_back(index);
    Image2D image(static_cast<void*>(pixels),imageSize,m_Target.vkCore().device(), m_Target.vkCore().physicalDevice(), static_cast<uint32_t >(width),static_cast<uint32_t >(height),1, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT, t);
    stbi_image_free(pixels);
}

Renderpass ForwardRenderMode::createDefaultRenderpass()
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

    return Renderpass(m_Target.vkCore().device(),ad,sd,sdy);
}

void ForwardRenderMode::render(nanoseconds deltaTime)
{
    updateUniformBuffer(deltaTime);

    VkResult result;
    uint32_t swapImageIndex = m_Target.swapchain().getAvailableImageIndex(result);
    handleSwapchainErrorCodes(result);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_Target.swapchain().imgAvailableSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[swapImageIndex];
    VkSemaphore signalSemaphores[] = {m_Target.swapchain().renderFinishedSemaphore()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    result = vkQueueSubmit(m_Target.swapchain().presentQueue().m_Queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkIfFailThrowMessage(result, "Failed to submit the command buffer!");

    VkResult presentResult = m_Target.swapchain().present(swapImageIndex);
    handleSwapchainErrorCodes(presentResult);

}

ForwardRenderMode::~ForwardRenderMode()
{
    vkDeviceWaitIdle(m_Target.vkCore().device());
    m_Target.platform().removeResizeCallback(m_ComparePtr);
}

void ForwardRenderMode::recreateSwapchain(uint32_t width, uint32_t height)
{
    vkDeviceWaitIdle(m_Target.vkCore().device());

    m_Target.swapchain().recreateSwapchain(width, height);

    m_Framebuffers.clear();
    m_Renderpasses.clear();
    m_PSOs.clear();
    m_Commandpool.deallocateCommandBuffers(m_CommandBuffers);
    m_CommandBuffers.clear();
    m_IndexedVertexBuffers.clear();

    createVertexBuffer();
    createRenderpass();
    createPipeline();
    createFramebuffers();
    createCommandbuffers();
}

void ForwardRenderMode::createRenderpass()
{
    m_Renderpasses.emplace_back(std::move(createDefaultRenderpass()));
}

void ForwardRenderMode::createPipeline()
{
    Renderpass& renderpass = m_Renderpasses.back();

    ShaderModule vertex("vert.spv", ShaderModuleType::Vertex, m_Target.vkCore().device());
    ShaderModule fragment("frag.spv", ShaderModuleType::Fragment, m_Target.vkCore().device());

    PipelineStateDescriptor descriptor;
    descriptor.attachShader(vertex);
    descriptor.attachShader(fragment);
    descriptor.attachRenderpass(renderpass, 0);
    descriptor.setViewportPropsFromSwapchain(m_Target.swapchain());

    descriptor.setInputAssemblerState(PipelineStateDescriptor::defaultInputAssemplyState());

    auto bdesc = Vertex::bindingDescription();
    auto adesc = Vertex::attributeDescriptions();
    vector<VkVertexInputBindingDescription> bdescs = { bdesc };
    vector<VkVertexInputAttributeDescription> adescs = {};
    for(const auto& a : adesc)
        adescs.push_back(a);

    descriptor.setVertexInputState(PipelineStateDescriptor::defaultVertexInputState(bdescs,adescs));
    descriptor.setRasterizerState(PipelineStateDescriptor::defaultRasterizerState());
    descriptor.setMultisampleState(PipelineStateDescriptor::predefinedMultisampleState(SampleCount::None));

    vector<VkPipelineColorBlendAttachmentState> vec = { PipelineStateDescriptor::defaultColorBlendAttachmentState() };
    descriptor.setColorBlendingState(PipelineStateDescriptor::defaultColorBlendState(vec));

    //descriptor.setDynamicState(PipelineStateDescriptor::defaultDynamicState());

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = nullptr;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;

    vkCreatePipelineLayout(m_Target.vkCore().device(), &pipelineLayoutCreateInfo, nullptr, m_TempLayout.reset());

    descriptor.setPipelineLayout(m_TempLayout);

    m_PSOs.emplace_back(std::move(descriptor.createGraphicsPipeline(m_Target.vkCore().device())));

}

void ForwardRenderMode::createFramebuffers()
{
    Renderpass& renderpass = m_Renderpasses.back();
    uint32_t frameBufferCount = static_cast<uint32_t >(m_Target.swapchain().imageViews().size());
    for(uint32_t i = 0; i < frameBufferCount; ++i)
    {
        m_Framebuffers.push_back(Framebuffer(m_Target.vkCore().device(), m_Target.swapchain().extent2D(), renderpass, vector<VkImageView>{m_Target.swapchain().imageViews()[i]}));
    }
}

void ForwardRenderMode::createCommandbuffers()
{
    uint32_t frameBufferCount = static_cast<uint32_t >(m_Target.swapchain().imageViews().size());
    Renderpass& renderpass = m_Renderpasses.back();
    PipelineStateObject& pipeline = m_PSOs.back();

    m_CommandBuffers = m_Commandpool.allocateCommandBuffers(frameBufferCount, CommandBufferLevel::Primary);

    for(uint32_t i = 0; i < static_cast<uint32_t >(m_CommandBuffers.size()); ++i)
    {
        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        vkBeginCommandBuffer(m_CommandBuffers[i], &commandBufferBeginInfo);

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = renderpass.renderpass();
        renderPassBeginInfo.framebuffer = m_Framebuffers[i].framebuffer();
        renderPassBeginInfo.renderArea.offset = {0,0};
        renderPassBeginInfo.renderArea.extent = m_Target.swapchain().extent2D();

        VkClearValue clearColorValue = {0.0f,0.0f,0.0f,1.0f};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColorValue;

        vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline());

        VkBuffer vertexBuffers[] = {m_IndexedVertexBuffers[0].buffer().buffer()};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(m_CommandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(m_CommandBuffers[i], vertexBuffers[0],  m_IndexedVertexBuffers[0].indexOffset(), VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_TempLayout, 0, 1, m_DescriptorSets.data(), 0, nullptr);

        //VkViewport v = m_Target.swapchain().viewport();

        //vkCmdSetViewport(m_CommandBuffers[i], 0,1, &v);

        vkCmdDrawIndexed(m_CommandBuffers[i], m_IndexedVertexBuffers[0].indicesCount(), 1,0,0,0);
        //vkCmdDraw(m_CommandBuffers[i], 3,1,0,0);

        vkCmdEndRenderPass(m_CommandBuffers[i]);

        VkResult result = vkEndCommandBuffer(m_CommandBuffers[i]);
        vkIfFailThrowMessage(result, "Error occured during recording of command buffer!");
    }
}

void ForwardRenderMode::handleSwapchainErrorCodes(VkResult result)
{
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        auto p = m_Target.platform().getWindowSize(m_Target.window());
        recreateSwapchain(p.first,p.second);
        return;
    } else if(result == VK_SUBOPTIMAL_KHR)
            {
                Logger::warn("Swapchain became sub-optimal!");
            }
        else if (result != VK_SUCCESS) {
            Logger::failure("failed to acquire swap chain image!");
    }
}

void ForwardRenderMode::createVertexBuffer()
{
    const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
    };


    uint32_t index = m_Target.swapchain().presentQueue().m_FamilyIndex;
    vector<uint32_t> t = m_Target.vkCore().transferQueueFamilies();
    t.push_back(index);
    m_IndexedVertexBuffers.emplace_back(m_Target.vkCore().device(), m_Target.vkCore().physicalDevice(), vertices, indices, m_Target.vkCore(), t);
}

void ForwardRenderMode::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding layoutBinding = {};
    layoutBinding.binding = 0;
    layoutBinding.descriptorCount = 1;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = nullptr;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &layoutBinding;

    VkResult result = vkCreateDescriptorSetLayout(m_Target.vkCore().device(), &descriptorSetLayoutCreateInfo, nullptr, m_DescriptorSetLayout.reset());
    vkIfFailThrowMessage(result, "Error creating descriptor set layout!");
}

void ForwardRenderMode::updateUniformBuffer(nanoseconds deltaTime)
{
    m_UBOData.world = m_UBOData.world * glm::rotate(glm::mat4(), deltaTime.count() * glm::radians(0.0000001f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_UBOData.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_UBOData.projection = glm::perspective(glm::radians(90.0f), m_Target.swapchain().extent2D().width / (float) m_Target.swapchain().extent2D().height, 0.1f, 10.0f);
    //m_UBOData.projection[1][1] *= -1;



    m_UniformBuffers[0].updateGPUBuffer(static_cast<void*>(&m_UBOData), sizeof(WorldViewProjectionUBO));
}

void ForwardRenderMode::createUniformBuffer()
{
    uint32_t index = m_Target.swapchain().presentQueue().m_FamilyIndex;
    vector<uint32_t> t = m_Target.vkCore().transferQueueFamilies();
    t.push_back(index);

    m_UniformBuffers.emplace_back(m_Target.vkCore().device(), m_Target.vkCore().physicalDevice(), sizeof(WorldViewProjectionUBO), m_Target.vkCore(), t);

}

void ForwardRenderMode::createDescriptorPool()
{
    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &poolSize;
    descriptorPoolCreateInfo.maxSets = 1;

    VkResult result = vkCreateDescriptorPool(m_Target.vkCore().device(), &descriptorPoolCreateInfo, nullptr, m_DescriptorPool.reset());
    vkIfFailThrowMessage(result, "Failed to create descriptor pooly babeey!");
}

void ForwardRenderMode::addDescriptorSet()
{
    VkDescriptorSet set;

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_DescriptorSetLayout;

    VkResult result = vkAllocateDescriptorSets(m_Target.vkCore().device(), &allocInfo, &set);
    vkIfFailThrowMessage(result, "Descriptor set allocation failed!");

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = m_UniformBuffers[0].buffer().buffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(WorldViewProjectionUBO);

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = set;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrite.pImageInfo = nullptr;

    vkUpdateDescriptorSets(m_Target.vkCore().device(), 1, &descriptorWrite, 0, nullptr);
    m_DescriptorSets.push_back(set);
}
