//
// Created by hindrik on 8-4-17.
//

#include "RenderTargetOutput.h"

void RenderTargetOutput::render(float deltaTime) const
{
    m_RenderMode->render(deltaTime);
}

RenderTargetOutput::RenderTargetOutput(RenderMode* renderMode) : m_RenderMode(renderMode)
{

}

bool RenderTargetOutput::processAPI(float deltaTime) const
{
    return m_RenderMode->processAPI(deltaTime);
}