//
// Created by hindrik on 2-4-17.
//

#include "PipelineStateDescriptor.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

auto PipelineStateDescriptor::attachShader(ShaderModule &shaderModule) -> void
{
    if(shaderModule.type() == ShaderModuleType::Null)
        throw std::runtime_error("Invalid shader module type!");

    m_ShaderStages.push_back(shaderModule.info());
}

auto PipelineStateDescriptor::attachRenderpass(Renderpass &renderpass, uint32_t subPassIndex) -> void
{
    m_Renderpass    = renderpass.renderpass();
    m_SubPassIndex  = subPassIndex;
}

auto PipelineStateDescriptor::createGraphicsPipeline(VkDevice device) -> VkUniqueHandle<VkPipeline>
{

    VkGraphicsPipelineCreateInfo info = getCreateInfo();

    VkUniqueHandle<VkPipeline> uHandle{device, vkDestroyPipeline};

    VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, uHandle.reset());
    vkIfFailThrowMessage(result, "Error creating graphics pipeline!");

    return uHandle;
}

auto PipelineStateDescriptor::getCreateInfo() -> VkGraphicsPipelineCreateInfo
{
    VkGraphicsPipelineCreateInfo info = {};

    info.sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext                  = nullptr;
    info.stageCount             = static_cast<uint32_t >(m_ShaderStages.size());
    info.pStages                = m_ShaderStages.data();
    info.pVertexInputState      = &m_VertexInputState;
    info.pInputAssemblyState    = &m_InputAssemblyState;
    info.pViewportState         = &m_ViewportState;
    info.pRasterizationState    = &m_RasterizerState;
    info.pMultisampleState      = &m_MultisampleState;
    info.pDepthStencilState     = &m_DepthStencilState;
    info.pColorBlendState       = &m_ColorBlendState;
    info.pDynamicState          = &m_DynamicState;
    info.pTessellationState     = &m_TesselationStageState;
    info.layout                 = m_PipelineLayout;
    info.renderPass             = m_Renderpass;
    info.subpass                = m_SubPassIndex;
    info.basePipelineHandle     = VK_NULL_HANDLE;
    info.basePipelineIndex      = -1;

    return info;
}