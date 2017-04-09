//
// Created by hindrik on 2-4-17.
//

#ifndef VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H
#define VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H


#include <bitset>
#include "ShaderModule.h"
#include "Renderpass.h"

using std::bitset;

struct PipelineStateDescriptor
{
private:
    vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
    VkPipelineVertexInputStateCreateInfo    m_VertexInputState;
    VkPipelineInputAssemblyStateCreateInfo  m_InputAssemblyState;
    VkPipelineViewportStateCreateInfo       m_ViewportState;
    VkPipelineRasterizationStateCreateInfo  m_RasterizerState;
    VkPipelineMultisampleStateCreateInfo    m_MultisampleState;
    VkPipelineDepthStencilStateCreateInfo   m_DepthStencilState;
    VkPipelineTessellationStateCreateInfo   m_TesselationStageState;
    VkPipelineColorBlendStateCreateInfo     m_ColorBlendState;
    VkPipelineDynamicStateCreateInfo        m_DynamicState;
    VkPipelineLayout                        m_PipelineLayout;
    VkRenderPass                            m_Renderpass;
    uint32_t                                m_SubPassIndex;
public:
    auto    attachShader(ShaderModule &shaderModule)                            -> void;
    auto    attachRenderpass(Renderpass &renderpass, uint32_t subPassIndex)     -> void;

    auto    createGraphicsPipeline(VkDevice device)    -> VkUniqueHandle<VkPipeline>;
    auto    getCreateInfo()                            -> VkGraphicsPipelineCreateInfo;

};

#endif //VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H