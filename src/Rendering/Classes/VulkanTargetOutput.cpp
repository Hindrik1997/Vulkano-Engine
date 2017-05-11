//
// Created by hindrik on 30-3-17.
//

#include "VulkanTargetOutput.h"

VulkanTargetOutput::VulkanTargetOutput(RenderTargetOutput&& output) : m_Manager(std::move(output))
{
}

auto VulkanTargetOutput::getRenderpass(uint16_t handle) -> Renderpass&
{
    return m_Manager.getRenderpass(handle);
}

auto VulkanTargetOutput::getPipelineStateObject(uint16_t handle) -> PipelineStateObject&
{
    return m_Manager.getPipelineStateObject(handle);
}



Swapchain &VulkanTargetOutput::swapchain()
{
    return m_Manager.swapchain();
}