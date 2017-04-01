//
// Created by hindrik on 30-3-17.
//

#ifndef VULKANOENGINE_VULKANPIPELINEMANAGER_H
#define VULKANOENGINE_VULKANPIPELINEMANAGER_H


#include "../../Utility Classes/NonCopyable.h"
#include "../../Utility Classes/NonCopyableNonMovable.h"
#include "../Vulkan/Classes/RenderTargetOutput.h"
#include "../Vulkan/Classes/Renderpass.h"

class VkCore;
class Swapchain;



class VulkanPipelineManager : NonCopyable
{
private:
    RenderTargetOutput& m_Output;


    CacheOptimizedStorage<Renderpass> m_Renderpasses;
    //CacheOptimizedStorage<PipelineStateObject> m_PSOs;


public:
    VulkanPipelineManager(RenderTargetOutput& output);
    ~VulkanPipelineManager() = default;

    VulkanPipelineManager(VulkanPipelineManager&&) = default;
    VulkanPipelineManager& operator=(VulkanPipelineManager&&) = default;

    VulkanPipelineManager(const VulkanPipelineManager&) = delete;
    VulkanPipelineManager& operator=(const VulkanPipelineManager&) = delete;



};


#endif //VULKANOENGINE_VULKANPIPELINEMANAGER_H
