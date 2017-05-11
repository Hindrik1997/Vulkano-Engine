//
// Created by hindrik on 30-3-17.
//

#include "VulkanPipelineResourceManager.h"

VulkanPipelineResourceManager::VulkanPipelineResourceManager(RenderTargetOutput&& output) : m_Output(std::move(output))
{

}

Renderpass &VulkanPipelineResourceManager::getRenderpass(uint16_t handle)
{
    return m_Renderpasses[handle];
}

PipelineStateObject& VulkanPipelineResourceManager::getPipelineStateObject(uint16_t handle)
{
    return m_PSOs[handle];
}

Swapchain& VulkanPipelineResourceManager::swapchain()
{
    return m_Output.swapchain();
}