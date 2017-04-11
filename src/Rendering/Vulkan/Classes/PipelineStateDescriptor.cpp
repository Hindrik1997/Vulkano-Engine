//
// Created by hindrik on 2-4-17.
//

#include "PipelineStateDescriptor.h"
#include "../../Utilities/VulkanUtilityFunctions.h"
#include "ShaderModule.h"
#include "Renderpass.h"
#include "../../Utilities/VulkanEnums.h"
#include "Swapchain.h"

VkPipelineVertexInputStateCreateInfo PipelineStateDescriptor::defaultVertexInputState() {

    VkPipelineVertexInputStateCreateInfo info   = {};
    info.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.pNext                                  = nullptr;
    info.vertexBindingDescriptionCount          = 0;
    info.vertexAttributeDescriptionCount        = 0;
    info.pVertexAttributeDescriptions           = nullptr;
    info.pVertexBindingDescriptions             = nullptr;

    return info;
}

VkPipelineInputAssemblyStateCreateInfo PipelineStateDescriptor::defaultInputAssemplyState() {

    VkPipelineInputAssemblyStateCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.primitiveRestartEnable = VK_FALSE;

    return info;
}

VkViewport PipelineStateDescriptor::defaultViewport(VkExtent2D& swapchainExtent) {

    VkViewport port = {};

    port.x = 0.0f;
    port.y = 0.0f;
    port.width = static_cast<float>(swapchainExtent.width);
    port.height = static_cast<float>(swapchainExtent.height);
    port.maxDepth = 1.0f;
    port.minDepth = 0.0f;

    return port;
}

VkRect2D PipelineStateDescriptor::defaultScissorRectangle(VkExtent2D& swapchainExtent) {

    VkRect2D rect = {};
    rect.offset = {0,0};
    rect.extent = swapchainExtent;
    return rect;
}

VkPipelineViewportStateCreateInfo PipelineStateDescriptor::defaultViewPortState(VkViewport& viewport, VkRect2D& scissorRectangle) {

    VkPipelineViewportStateCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.scissorCount = 1;
    info.viewportCount = 1;
    info.pScissors = &scissorRectangle;
    info.pViewports = &viewport;

    return info;
}

VkPipelineRasterizationStateCreateInfo PipelineStateDescriptor::defaultRasterizerState() {

    VkPipelineRasterizationStateCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.pNext = nullptr;
    //TODO: probably check for this later on
    info.depthClampEnable = VK_FALSE;
    info.rasterizerDiscardEnable = VK_FALSE;
    info.polygonMode = VK_POLYGON_MODE_FILL;
    info.lineWidth = 1.0f;
    info.cullMode = VK_CULL_MODE_BACK_BIT;
    info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    info.depthBiasEnable = VK_FALSE;
    info.depthBiasConstantFactor = 0.0f;
    info.depthBiasClamp = 0.0f;
    info.depthBiasSlopeFactor = 0.0f;

    return info;
}

VkPipelineMultisampleStateCreateInfo PipelineStateDescriptor::defaultMultisampleState() {

    VkPipelineMultisampleStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.sampleShadingEnable = VK_FALSE;
    info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    info.minSampleShading = 1.0f;
    info.pSampleMask = nullptr;
    info.alphaToCoverageEnable = VK_FALSE;
    info.alphaToOneEnable = VK_FALSE;

    return info;
}

VkPipelineColorBlendAttachmentState PipelineStateDescriptor::defaultColorBlendAttachmentState() {

    VkPipelineColorBlendAttachmentState attachment = {};
    attachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    attachment.blendEnable = VK_FALSE;
    attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    attachment.colorBlendOp = VK_BLEND_OP_ADD;
    attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    attachment.alphaBlendOp = VK_BLEND_OP_ADD;

    return attachment;
}

VkPipelineColorBlendStateCreateInfo PipelineStateDescriptor::defaultColorBlendState(vector<VkPipelineColorBlendAttachmentState>& attachmentStates) {

    VkPipelineColorBlendStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info.logicOpEnable = VK_FALSE;
    info.logicOp = VK_LOGIC_OP_COPY;
    info.attachmentCount = static_cast<uint32_t >(attachmentStates.size());
    info.pAttachments = attachmentStates.data();
    info.blendConstants[0] = 0.0f;
    info.blendConstants[1] = 0.0f;
    info.blendConstants[2] = 0.0f;
    info.blendConstants[3] = 0.0f;

    return info;
}

VkPipelineDynamicStateCreateInfo PipelineStateDescriptor::defaultDynamicState() {

    vector<VkDynamicState> defaultDynamicStates =
            {
                    VK_DYNAMIC_STATE_LINE_WIDTH,
            };

    VkPipelineDynamicStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    info.dynamicStateCount = static_cast<uint32_t >(defaultDynamicStates.size());
    info.pDynamicStates = defaultDynamicStates.data();

    return info;
}

VkResult PipelineStateDescriptor::defaultPipelineLayout(VkPipelineLayout* layout, VkDevice device) {

    VkPipelineLayoutCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = 0;
    info.pSetLayouts = nullptr;
    info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = 0;

    return vkCreatePipelineLayout(device, &info, nullptr, layout);
}

VkPipelineDepthStencilStateCreateInfo PipelineStateDescriptor::defaultDepthStencilState() {
    //TODO: IMPLEMENT
    return VkPipelineDepthStencilStateCreateInfo{};
}








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

    m_IsSetBookkeeper[11] = true;
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
    info.pViewportState         = &m_Viewport.m_ViewportState;
    info.pRasterizationState    = &m_RasterizerState;
    info.pMultisampleState      = &m_MultisampleState;
    info.pDepthStencilState     = m_IsSetBookkeeper[5] ? &m_DepthStencilState : nullptr;
    info.pColorBlendState       = &m_ColorBlendState;
    info.pDynamicState          = m_IsSetBookkeeper[8] ? &m_DynamicState : nullptr;
    info.pTessellationState     = m_IsSetBookkeeper[6] ? &m_TesselationStageState : nullptr;
    info.layout                 = m_PipelineLayout;
    info.renderPass             = m_Renderpass;
    info.subpass                = m_SubPassIndex;
    info.basePipelineHandle     = VK_NULL_HANDLE;
    info.basePipelineIndex      = -1;

    return info;
}

auto PipelineStateDescriptor::setViewportPropsFromSwapchain(Swapchain &swapchain) -> void
{
    VkExtent2D extent = swapchain.extent2D();
    m_Viewport.m_Viewport = defaultViewport(extent);
    m_Viewport.m_ScissorRectangle = defaultScissorRectangle(extent);

    VkPipelineViewportStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = {};
    info.pScissors = &m_Viewport.m_ScissorRectangle;
    info.scissorCount = 1;
    info.pViewports = &m_Viewport.m_Viewport;
    info.viewportCount = 1;

    m_Viewport.m_ViewportState = info;

    m_IsSetBookkeeper[10] = true;

}

VkPipelineMultisampleStateCreateInfo PipelineStateDescriptor::predefinedMultisampleState(SampleCount samples)
{
    VkPipelineMultisampleStateCreateInfo multisampling  = {};
    multisampling.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.pNext                                 = nullptr;
    multisampling.sampleShadingEnable                   = VK_FALSE;
    multisampling.rasterizationSamples                  = static_cast<VkSampleCountFlagBits>(samples);
    multisampling.minSampleShading                      = 1.0f;
    multisampling.pSampleMask                           = nullptr;
    multisampling.alphaToCoverageEnable                 = VK_FALSE;
    multisampling.alphaToOneEnable                      = VK_FALSE;

    return multisampling;
}

auto PipelineStateDescriptor::setInputAssemblerState(VkPipelineInputAssemblyStateCreateInfo info) -> void
{
    m_InputAssemblyState = info;
    m_IsSetBookkeeper[2] = true;
}

auto PipelineStateDescriptor::setVertexInputState(VkPipelineVertexInputStateCreateInfo info) -> void
{
    m_VertexInputState = info;
    m_IsSetBookkeeper[1] = true;
}

auto PipelineStateDescriptor::setRasterizerState(VkPipelineRasterizationStateCreateInfo info) -> void
{
    m_RasterizerState = info;
    m_IsSetBookkeeper[3] = true;
}

auto PipelineStateDescriptor::setMultisampleState(VkPipelineMultisampleStateCreateInfo info) -> void
{
    m_MultisampleState = info;
    m_IsSetBookkeeper[4] = true;
}

auto PipelineStateDescriptor::setDepthStencilState(VkPipelineDepthStencilStateCreateInfo info) -> void
{
    m_DepthStencilState = info;
    m_IsSetBookkeeper[5] = true;
}

auto PipelineStateDescriptor::setColorBlendingState(VkPipelineColorBlendStateCreateInfo info) -> void
{
    m_ColorBlendState = info;
    m_IsSetBookkeeper[7] = true;
}

auto PipelineStateDescriptor::setDynamicState(VkPipelineDynamicStateCreateInfo info) -> void
{
    m_DynamicState = info;
    m_IsSetBookkeeper[8] = true;
}

auto PipelineStateDescriptor::setPipelineLayout(VkPipelineLayout layout) -> void
{
    m_PipelineLayout = layout;
    m_IsSetBookkeeper[9] = true;
}

auto PipelineStateDescriptor::setTesselationState(VkPipelineTessellationStateCreateInfo info) -> void
{
    m_TesselationStageState = info;
    m_IsSetBookkeeper[6] = true;
}
