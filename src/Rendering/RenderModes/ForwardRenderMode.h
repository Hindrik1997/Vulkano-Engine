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

class ForwardRenderMode final : public RenderMode
{
private:
    vector<Renderpass>                                    m_Renderpasses;
    vector<PipelineStateObject>                           m_PSOs;
    VkUniqueHandle<VkPipelineLayout>                      m_TempLayout;
    vector<Framebuffer>                                   m_Framebuffers;
    CommandPool                                           m_Commandpool;
    vector<VkCommandBuffer> m_Buffers;
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
    void createPipeline();
    void createFramebuffers();
    void createCommandbuffers();

    Renderpass createDefaultRenderpass();
    void recreateSwapchain(uint32_t  width, uint32_t height);


};


#endif //VULKANOENGINE_FORWARDRENDERMODE_H
