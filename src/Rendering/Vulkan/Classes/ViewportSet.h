//
// Created by hindrik on 9-4-17.
//

#ifndef VULKANOENGINE_VIEWPORTSET_H
#define VULKANOENGINE_VIEWPORTSET_H

#include "../VulkanPlatforms/VulkanPlatform.h"

struct ViewportSet
{
    VkViewport                              m_Viewport;
    VkRect2D                                m_ScissorRectangle;
    VkPipelineViewportStateCreateInfo       m_ViewportState;

    ViewportSet()
    {
        m_Viewport = {};
        m_ScissorRectangle = {};
        m_ViewportState = {};
    }
    ViewportSet(const ViewportSet& rhs)
    {
        m_Viewport                  = rhs.m_Viewport;
        m_ScissorRectangle          = rhs.m_ScissorRectangle;
        m_ViewportState             = rhs.m_ViewportState;
        m_ViewportState.pViewports  = &m_Viewport;
        m_ViewportState.pScissors   = &m_ScissorRectangle;
    }
    ViewportSet& operator=(const ViewportSet& rhs)
    {
        if(this == &rhs)
            return *this;

        m_Viewport                  = rhs.m_Viewport;
        m_ScissorRectangle          = rhs.m_ScissorRectangle;
        m_ViewportState             = rhs.m_ViewportState;
        m_ViewportState.pViewports  = &m_Viewport;
        m_ViewportState.pScissors   = &m_ScissorRectangle;
        return *this;
    }
    ViewportSet(ViewportSet&&) = delete;
    ViewportSet& operator=(const ViewportSet&&) = delete;
};

#endif //VULKANOENGINE_VIEWPORTSET_H
