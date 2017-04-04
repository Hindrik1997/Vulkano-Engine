//
// Created by hindrik on 2-4-17.
//

#ifndef VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H
#define VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H


#include "ShaderModule.h"

struct PipelineStateDescriptor
{
private:
    VkGraphicsPipelineCreateInfo m_CreateInfo = {};
    vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
public:
    void AttachShader(ShaderModule& shaderModule);






};


#endif //VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H
