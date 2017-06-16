//
// Created by hindrik on 8-4-17.
//

#include "RenderTargetOutput.h"

using std::chrono::nanoseconds;


void RenderTargetOutput::render(nanoseconds deltaTime) const
{
    m_RenderMode->render(deltaTime);
}

RenderTargetOutput::RenderTargetOutput(RenderMode* renderMode) : m_RenderMode(renderMode)
{

}

bool RenderTargetOutput::processAPI(nanoseconds deltaTime) const
{
    return m_RenderMode->processAPI(deltaTime);
}