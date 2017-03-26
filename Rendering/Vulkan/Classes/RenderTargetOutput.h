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
    VK_PLATFORM& m_Platform;
    WindowHandle m_Window;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
public:
    RenderTargetOutput(uint32_t windowWidth, uint32_t windowHeight, VkCore& vkCore, VK_PLATFORM& platform);
    ~RenderTargetOutput();

private:
    static VkResult createSurface(VkInstance instance, VkSurfaceKHR& surface, WindowHandle handle, VK_PLATFORM& platform);


};


#endif //VULKANOENGINE_RENDERTARGETOUTPUT_H
