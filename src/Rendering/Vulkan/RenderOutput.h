//
// Created by hindrik on 8-4-17.
//

#ifndef VULKANOENGINE_RENDERTARGETOUTPUT_H
#define VULKANOENGINE_RENDERTARGETOUTPUT_H


#include "VulkanPlatforms/VulkanPlatform.h"
#include "../../Utility Classes/Nullable.h"
#include "VulkanPlatforms/WindowHandleWrapper.h"
#include "VkUniqueHandle.h"
#include "Classes/Swapchain.h"
#include <memory>

using std::unique_ptr;

class RenderOutput
{
private:
    WindowHandleWrapper             m_Window;
    VK_PLATFORM*                    m_Platform;
    VkUniqueHandle<VkSurfaceKHR>    m_Surface;
    VkInstance                      m_Instance;
    Nullable<Swapchain>             m_Swapchain;
public:
    RenderOutput(VK_PLATFORM& platform, VkInstance instance, uint32_t width, uint32_t height);
    ~RenderOutput() = default;

    RenderOutput(RenderOutput&&) = default;
    RenderOutput(const RenderOutput&) = delete;

    RenderOutput& operator=(const RenderOutput&) = delete;
    RenderOutput& operator=(RenderOutput&&) = default;

public:
    template<typename ...Args>
    void initializeSwapchain(Args&&... args);
    bool processAPI(nanoseconds deltaTime);

    VkSurfaceKHR surface();
};


template<typename ...Args>
void RenderOutput::initializeSwapchain(Args&&... args)
{
    m_Swapchain.set(std::forward<Args>(args)...);
}




#endif //VULKANOENGINE_RENDERTARGETOUTPUT_H
