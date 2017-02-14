//
// Created by hindrik on 14-2-17.
//

#ifndef VULKANOENGINE_PIPELINE_H
#define VULKANOENGINE_PIPELINE_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include <vector>

using std::vector;


class Pipeline {
public:




    static VkPipelineVertexInputStateCreateInfo     defaultVertexInputState();
    static VkPipelineInputAssemblyStateCreateInfo   defaultInputAssemplyState();
    static VkViewport                               defaultViewport(VkExtent2D& swapchainExtent);
    static VkRect2D                                 defaultScissorRectangle(VkExtent2D& swapchainExtent);
    static VkPipelineViewportStateCreateInfo        defaultViewPortState(VkViewport viewport, VkRect2D scissorRectangle);
    static VkPipelineRasterizationStateCreateInfo   defaultRasterizerState();
    static VkPipelineMultisampleStateCreateInfo     defaultMultisampleState();
    static VkPipelineDepthStencilStateCreateInfo    defaultDepthStencilState();
    static VkPipelineColorBlendAttachmentState      defaultColorBlendAttachmentState();
    static VkPipelineColorBlendStateCreateInfo      defaultColorBlendState(vector<VkPipelineColorBlendAttachmentState>& attachmentStates);
    static VkPipelineDynamicStateCreateInfo         defaultDynamicState();
    static VkResult                                 defaultPipelineLayout(VkPipelineLayout* layout, VkDevice device);




};


#endif //VULKANOENGINE_PIPELINE_H
