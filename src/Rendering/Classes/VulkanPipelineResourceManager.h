//
// Created by hindrik on 30-3-17.
//

#ifndef VULKANOENGINE_VULKANPIPELINEMANAGER_H
#define VULKANOENGINE_VULKANPIPELINEMANAGER_H


#include "../../Utility Classes/NonCopyable.h"
#include "../../Utility Classes/NonCopyableNonMovable.h"
#include "../Vulkan/Classes/RenderTargetOutput.h"
#include "../Vulkan/Classes/Renderpass.h"
#include "../Vulkan/Classes/PipelineStateObject.h"

class VkCore;
class Swapchain;



class VulkanPipelineResourceManager
{
private:
    RenderTargetOutput m_Output;
    CacheOptimizedStorage<Renderpass,2>           m_Renderpasses;
    CacheOptimizedStorage<PipelineStateObject,2>  m_PSOs;
public:

    VulkanPipelineResourceManager(RenderTargetOutput&& output);
    ~VulkanPipelineResourceManager() = default;

    VulkanPipelineResourceManager(VulkanPipelineResourceManager&&) = default;
    VulkanPipelineResourceManager& operator=(VulkanPipelineResourceManager&&) = default;

    VulkanPipelineResourceManager(const VulkanPipelineResourceManager&) = delete;
    VulkanPipelineResourceManager& operator=(const VulkanPipelineResourceManager&) = delete;

public:

    template<typename ...Args>
    uint16_t getNewRenderpass(Args... args);

    template<typename ...Args>
    uint16_t getNewPipelineStateObject(Args... args);

    Renderpass& getRenderpass(uint16_t handle);
    PipelineStateObject& getPipelineStateObject(uint16_t handle);

    Swapchain& swapchain();

};


template<typename ...Args>
uint16_t VulkanPipelineResourceManager::getNewPipelineStateObject(Args... args)
{
    return m_PSOs.getNewItem(args...);
}

template<typename ...Args>
uint16_t VulkanPipelineResourceManager::getNewRenderpass(Args... args)
{
    return m_Renderpasses.getNewItem(args...);
}



#endif //VULKANOENGINE_VULKANPIPELINEMANAGER_H