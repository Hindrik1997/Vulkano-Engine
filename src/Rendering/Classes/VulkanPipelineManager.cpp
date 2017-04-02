//
// Created by hindrik on 30-3-17.
//

#include "VulkanPipelineManager.h"

VulkanPipelineManager::VulkanPipelineManager(RenderTargetOutput&& output) : m_Output(std::move(output))
{

}

Renderpass &VulkanPipelineManager::getRenderpass(uint16_t handle)
{
    return m_Renderpasses[handle];
}

PipelineStateObject& VulkanPipelineManager::getPipelineStateObject(uint16_t handle)
{
    return m_PSOs[handle];
}