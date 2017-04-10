//
// Created by hindrik on 30-3-17.
//

#include "PipelineStateObject.h"



VkPipeline PipelineStateObject::pipeline()
{
    return m_Pipeline;
}

PipelineStateObject::PipelineStateObject(VkUniqueHandle<VkPipeline> &&pipeline) : m_Pipeline(std::move(pipeline))
{
}
