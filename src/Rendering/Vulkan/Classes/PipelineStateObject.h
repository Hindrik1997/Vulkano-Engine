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
    PipelineStateObject()   = delete;
    ~PipelineStateObject()  = default;

    PipelineStateObject(PipelineStateObject&&)              = default;
    PipelineStateObject& operator=(PipelineStateObject&&)   = default;

    VkPipeline pipeline();
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
};


#endif //VULKANOENGINE_PIPELINESTATEOBJECT_H
