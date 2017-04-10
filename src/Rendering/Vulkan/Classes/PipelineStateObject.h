//
// Created by hindrik on 30-3-17.
//

#ifndef VULKANOENGINE_PIPELINESTATEOBJECT_H
#define VULKANOENGINE_PIPELINESTATEOBJECT_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../../../Utility Classes/NonCopyable.h"
#include "../VkUniqueHandle.h"

class PipelineStateObject final : NonCopyable
{
private:
    VkUniqueHandle<VkPipeline> m_Pipeline;
public:
    PipelineStateObject(VkUniqueHandle<VkPipeline>&& pipeline);

    PipelineStateObject()   = delete;
    ~PipelineStateObject()  = default;

    PipelineStateObject(PipelineStateObject&&)              = default;
    PipelineStateObject& operator=(PipelineStateObject&&)   = default;

    VkPipeline pipeline();
public:

};


#endif //VULKANOENGINE_PIPELINESTATEOBJECT_H
