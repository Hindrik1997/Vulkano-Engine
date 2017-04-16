//
// Created by hindrik on 8-4-17.
//

#ifndef VULKANOENGINE_FORWARDRENDERMODE_H
#define VULKANOENGINE_FORWARDRENDERMODE_H


#include "../Vulkan/Classes/RenderMode.h"
#include "../Vulkan/Classes/RenderTarget.h"
#include "../Vulkan/Classes/Framebuffer.h"
#include "../Vulkan/Classes/CommandPool.h"
#include "../Vulkan/Classes/PipelineStateObject.h"
#include "../Vulkan/Classes/Buffers/VertexBuffer.h"
#include "../Vulkan/Classes/Buffers/IndexedVertexBuffer.h"
#include "../Vulkan/Classes/Buffers/UniformBuffer.h"
#include "../UniformBuffers/WVP.h"

class ForwardRenderMode final : public RenderMode
{
private:
    vector<Renderpass>                                    m_Renderpasses;
    vector<PipelineStateObject>                           m_PSOs;
    vector<IndexedVertexBuffer>                           m_IndexedVertexBuffers;
    VkUniqueHandle<VkPipelineLayout>                      m_TempLayout;
    VkUniqueHandle<VkDescriptorSetLayout>                 m_DescriptorSetLayout;
    vector<Framebuffer>                                   m_Framebuffers;
    CommandPool                                           m_Commandpool;
    void*                                                 m_ComparePtr;
    vector<VkCommandBuffer>                               m_CommandBuffers;
    vector<UniformBuffer>                                 m_UniformBuffers;
    VkUniqueHandle<VkDescriptorPool>                      m_DescriptorPool;
    vector<VkDescriptorSet>                               m_DescriptorSets;
    WorldViewProjectionUBO                                m_UBOData;
public:
    ForwardRenderMode(RenderTarget&& target);
    ~ForwardRenderMode();

    ForwardRenderMode(const ForwardRenderMode&) = delete;
    ForwardRenderMode& operator=(const ForwardRenderMode&) = delete;

    ForwardRenderMode(ForwardRenderMode&&) = default;
    ForwardRenderMode& operator=(ForwardRenderMode&&) = default;

public:
    void render(float deltaTime) override;


private:
    void handleSwapchainErrorCodes(VkResult result);
    void createRenderpass();
    void createVertexBuffer();
    void createPipeline();
    void createFramebuffers();
    void createCommandbuffers();
    void createDescriptorSetLayout();
    void createUniformBuffer();
    void createDescriptorPool();
    void addDescriptorSet();

    Renderpass createDefaultRenderpass();
    void recreateSwapchain(uint32_t  width, uint32_t height);


    void updateUniformBuffer(float deltaTime);
};


#endif //VULKANOENGINE_FORWARDRENDERMODE_H
