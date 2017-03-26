//
// Created by hindrik on 26-3-17.
//

#ifndef VULKANOENGINE_RENDERTARGETOUTPUT_H
#define VULKANOENGINE_RENDERTARGETOUTPUT_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkCore.h"

class RenderTargetOutput {
private:
    VkCore& m_VkCore;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
public:
    RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore);
    ~RenderTargetOutput();
public:



};


#endif //VULKANOENGINE_RENDERTARGETOUTPUT_H
