//
// Created by hindrik on 2-4-17.
//

#ifndef VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H
#define VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H

#include <bitset>
#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkUniqueHandle.h"
#include "ViewportSet.h"
#include "../../Utilities/VulkanEnums.h"

class Swapchain;
class ShaderModule;
class Renderpass;

struct PipelineStateDescriptor
{
private:
    vector<VkPipelineShaderStageCreateInfo> m_ShaderStages          = {};
    VkPipelineVertexInputStateCreateInfo    m_VertexInputState      = {};
    VkPipelineInputAssemblyStateCreateInfo  m_InputAssemblyState    = {};
    VkPipelineRasterizationStateCreateInfo  m_RasterizerState       = {};
    VkPipelineMultisampleStateCreateInfo    m_MultisampleState      = {};
    VkPipelineDepthStencilStateCreateInfo   m_DepthStencilState     = {};
    VkPipelineTessellationStateCreateInfo   m_TesselationStageState = {};
    VkPipelineColorBlendStateCreateInfo     m_ColorBlendState       = {};
    VkPipelineDynamicStateCreateInfo        m_DynamicState          = {};
    VkPipelineLayout                        m_PipelineLayout        = {};
    VkRenderPass                            m_Renderpass            = {};
    uint32_t                                m_SubPassIndex          = {};
    ViewportSet                             m_Viewport              = {};

    std::bitset<12>                         m_IsSetBookkeeper;

public:
    auto    attachShader(ShaderModule &shaderModule)                            -> void;
    auto    attachRenderpass(Renderpass &renderpass, uint32_t subPassIndex)     -> void;
    auto    setViewportPropsFromSwapchain(Swapchain& swapchain)                 -> void;
    auto    setInputAssemblerState(VkPipelineInputAssemblyStateCreateInfo info) -> void;
    auto    setVertexInputState(VkPipelineVertexInputStateCreateInfo info)      -> void;
    auto    setRasterizerState(VkPipelineRasterizationStateCreateInfo info)     -> void;
    auto    setMultisampleState(VkPipelineMultisampleStateCreateInfo info)      -> void;
    auto    setDepthStencilState(VkPipelineDepthStencilStateCreateInfo info)    -> void;
    auto    setColorBlendingState(VkPipelineColorBlendStateCreateInfo info)     -> void;
    auto    setDynamicState(VkPipelineDynamicStateCreateInfo info)              -> void;
    auto    setPipelineLayout(VkPipelineLayout layout)                          -> void;
    auto    setTesselationState(VkPipelineTessellationStateCreateInfo info)     -> void;


    auto    createGraphicsPipeline(VkDevice device)    -> VkUniqueHandle<VkPipeline>;
    auto    getCreateInfo()                            -> VkGraphicsPipelineCreateInfo;


public:
    static VkPipelineVertexInputStateCreateInfo     defaultVertexInputState();
    static VkPipelineInputAssemblyStateCreateInfo   defaultInputAssemplyState();
    static VkViewport                               defaultViewport(VkExtent2D& swapchainExtent);
    static VkRect2D                                 defaultScissorRectangle(VkExtent2D& swapchainExtent);
    static VkPipelineViewportStateCreateInfo        defaultViewPortState(VkViewport& viewport, VkRect2D& scissorRectangle);
    static VkPipelineRasterizationStateCreateInfo   defaultRasterizerState();
    static VkPipelineMultisampleStateCreateInfo     defaultMultisampleState();
    static VkPipelineDepthStencilStateCreateInfo    defaultDepthStencilState();
    static VkPipelineColorBlendAttachmentState      defaultColorBlendAttachmentState();
    static VkPipelineColorBlendStateCreateInfo      defaultColorBlendState(vector<VkPipelineColorBlendAttachmentState>& attachmentStates);
    static VkPipelineDynamicStateCreateInfo         defaultDynamicState();
    static VkResult                                 defaultPipelineLayout(VkPipelineLayout* layout, VkDevice device);


    static VkPipelineMultisampleStateCreateInfo     predefinedMultisampleState(SampleCount samples);


};

#endif //VULKANOENGINE_PIPELINESTATEDESCRIPTOR_H