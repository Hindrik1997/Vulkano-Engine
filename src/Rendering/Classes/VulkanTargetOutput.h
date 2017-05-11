//
// Created by hindrik on 30-3-17.
//

#ifndef VULKANOENGINE_MANAGEDTARGETOUTPUT_H
#define VULKANOENGINE_MANAGEDTARGETOUTPUT_H

#include "../../Utility Classes/NonCopyable.h"
#include "VulkanPipelineManager.h"
#include "../Vulkan/Classes/PipelineStateObject.h"
#include "../Vulkan/Classes/Renderpass.h"

class VulkanTargetOutput final : NonCopyable
{
private:
    VulkanPipelineManager   m_Manager;
public:
    VulkanTargetOutput(RenderTargetOutput&& output);

    VulkanTargetOutput(const VulkanTargetOutput&) = delete;
    VulkanTargetOutput& operator=(const VulkanTargetOutput&) = delete;

    VulkanTargetOutput(VulkanTargetOutput&&) = default;
    VulkanTargetOutput& operator=(VulkanTargetOutput&&) = default;


    template<typename ...Args>
    uint16_t getNewRenderpass(Args... args);

    template<typename ...Args>
    uint16_t getNewPipelineStateObject(Args... args);

    Renderpass& getRenderpass(uint16_t handle);
    PipelineStateObject& getPipelineStateObject(uint16_t handle);

    Swapchain& swapchain();




};

template<typename ...Args>
uint16_t VulkanTargetOutput::getNewPipelineStateObject(Args... args)
{
    return m_Manager.getNewPipelineStateObject(args...);
}

template<typename ...Args>
uint16_t VulkanTargetOutput::getNewRenderpass(Args... args)
{
    return m_Manager.getNewRenderpass(args...);
}



#endif //VULKANOENGINE_MANAGEDTARGETOUTPUT_H
