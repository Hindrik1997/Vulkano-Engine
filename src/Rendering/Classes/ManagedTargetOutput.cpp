//
// Created by hindrik on 30-3-17.
//

#include "ManagedTargetOutput.h"

ManagedTargetOutput::ManagedTargetOutput(RenderTargetOutput&& output) : m_Manager(std::move(output))
{
}

auto ManagedTargetOutput::getRenderpass(uint16_t handle) -> Renderpass&
{
    return m_Manager.getRenderpass(handle);
}

auto ManagedTargetOutput::getPipelineStateObject(uint16_t handle) -> PipelineStateObject&
{
    return m_Manager.getPipelineStateObject(handle);
}