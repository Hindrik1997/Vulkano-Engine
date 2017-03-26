//
// Created by hindrik on 26-3-17.
//

#include "RenderTargetOutput.h"

RenderTargetOutput::RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore) : m_VkCore(vkCore)
{





}

RenderTargetOutput::~RenderTargetOutput()
{
    PFN_vkDestroySurfaceKHR(m_Surface);
}