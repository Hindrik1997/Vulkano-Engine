//
// Created by hindrik on 8-4-17.
//

#ifndef VULKANOENGINE_FORWARDRENDERMODE_H
#define VULKANOENGINE_FORWARDRENDERMODE_H


#include "../Vulkan/Classes/RenderMode.h"
#include "../Vulkan/Classes/RenderTarget.h"
#include "../Classes/RenderpassResourceManager.h"
#include "../Vulkan/Classes/Framebuffer.h"
#include "../Vulkan/Classes/CommandPool.h"

class ForwardRenderMode final : public RenderMode
{
private:
    CacheOptimizedStorage<RenderpassResourceManager, 128> m_Renderpasses;
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
    uint16_t createDefaultRenderpass();
};


#endif //VULKANOENGINE_FORWARDRENDERMODE_H
