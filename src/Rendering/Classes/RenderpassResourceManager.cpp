//
// Created by hindrik on 8-4-17.
//

#include "RenderpassResourceManager.h"

RenderpassResourceManager::RenderpassResourceManager(Renderpass &&renderpass) : m_Renderpass(std::move(renderpass))
{

}

Renderpass &RenderpassResourceManager::renderpass()
{
    return m_Renderpass;
}

PipelineStateObject &RenderpassResourceManager::getPSObyHandle(uint16_t handle)
{
    return m_PSOs[handle];
}
