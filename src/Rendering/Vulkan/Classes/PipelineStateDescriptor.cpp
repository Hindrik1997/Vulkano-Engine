//
// Created by hindrik on 2-4-17.
//

#include "PipelineStateDescriptor.h"

void PipelineStateDescriptor::AttachShader(ShaderModule& shaderModule)
{
    if(shaderModule.type() == ShaderModuleType::Null)
        throw std::runtime_error("Invalid shader module type!");

    m_ShaderStages.push_back(shaderModule.info());
}
