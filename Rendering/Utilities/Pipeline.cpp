//
// Created by hindrik on 14-2-17.
//

#include "Pipeline.h"

VkPipelineVertexInputStateCreateInfo Pipeline::defaultVertexInputState() {

    VkPipelineVertexInputStateCreateInfo info   = {};
    info.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.pNext                                  = nullptr;
    info.vertexBindingDescriptionCount          = 0;
    info.vertexAttributeDescriptionCount        = 0;
    info.pVertexAttributeDescriptions           = nullptr;
    info.pVertexBindingDescriptions             = nullptr;

    return info;
}

VkPipelineInputAssemblyStateCreateInfo Pipeline::defaultInputAssemplyState() {

    VkPipelineInputAssemblyStateCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.primitiveRestartEnable = VK_FALSE;

    return info;
}

VkViewport Pipeline::defaultViewport(VkExtent2D& swapchainExtent) {

    VkViewport port = {};

    port.x = 0.0f;
    port.y = 0.0f;
    port.width = static_cast<float>(swapchainExtent.width);
    port.height = static_cast<float>(swapchainExtent.height);
    port.maxDepth = 1.0f;
    port.minDepth = 0.0f;

    return port;
}

VkRect2D Pipeline::defaultScissorRectangle(VkExtent2D& swapchainExtent) {

    VkRect2D rect = {};
    rect.offset = {0,0};
    rect.extent = swapchainExtent;
    return rect;
}

VkPipelineViewportStateCreateInfo Pipeline::defaultViewPortState(VkViewport& viewport, VkRect2D& scissorRectangle) {

    VkPipelineViewportStateCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.scissorCount = 1;
    info.viewportCount = 1;
    info.pScissors = &scissorRectangle;
    info.pViewports = &viewport;

    return info;
}

VkPipelineRasterizationStateCreateInfo Pipeline::defaultRasterizerState() {

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

VkPipelineMultisampleStateCreateInfo Pipeline::defaultMultisampleState() {

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

VkPipelineColorBlendAttachmentState Pipeline::defaultColorBlendAttachmentState() {

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

VkPipelineColorBlendStateCreateInfo Pipeline::defaultColorBlendState(vector<VkPipelineColorBlendAttachmentState>& attachmentStates) {

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

VkPipelineDynamicStateCreateInfo Pipeline::defaultDynamicState() {

    VkDynamicState defaultDynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_LINE_WIDTH,
    };

    VkPipelineDynamicStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    info.dynamicStateCount = 2;
    info.pDynamicStates = defaultDynamicStates;

    return info;
}

VkResult Pipeline::defaultPipelineLayout(VkPipelineLayout* layout, VkDevice device) {

    VkPipelineLayoutCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = 0;
    info.pSetLayouts = nullptr;
    info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = 0;

    return vkCreatePipelineLayout(device, &info, nullptr, layout);
}

VkPipelineDepthStencilStateCreateInfo Pipeline::defaultDepthStencilState() {
    //TODO: IMPLEMENT
}

