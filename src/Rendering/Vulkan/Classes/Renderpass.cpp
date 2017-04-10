//
// Created by hindrik on 30-3-17.
//

#include "Renderpass.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

Renderpass::Renderpass(VkDevice device, const vector<VkAttachmentDescription>& imageAttachments, const vector<VkSubpassDescription>& subpasses) : m_Renderpass({device,vkDestroyRenderPass})
{
    VkRenderPassCreateInfo renderPassCreateInfo = {};

    renderPassCreateInfo.sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext              = nullptr;
    renderPassCreateInfo.attachmentCount    = static_cast<uint32_t>(imageAttachments.size());
    renderPassCreateInfo.pAttachments       = imageAttachments.data();
    renderPassCreateInfo.subpassCount       = static_cast<uint32_t >(subpasses.size());
    renderPassCreateInfo.pSubpasses         = subpasses.data();
    renderPassCreateInfo.flags              = VkRenderPassCreateFlags{};
    renderPassCreateInfo.dependencyCount    = 0;
    renderPassCreateInfo.pDependencies      = nullptr;

    VkResult result = vkCreateRenderPass(device, &renderPassCreateInfo,nullptr, m_Renderpass.reset());

    vkIfFailThrowMessage(result,"Error creating renderpass!");
}

Renderpass::Renderpass(VkDevice device, const vector<VkAttachmentDescription> &imageAttachments,
                       const vector<VkSubpassDescription> &subpasses, const vector<VkSubpassDependency> &dependecies) : m_Renderpass({device,vkDestroyRenderPass})
{
    VkRenderPassCreateInfo renderPassCreateInfo = {};

    renderPassCreateInfo.sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext              = nullptr;
    renderPassCreateInfo.attachmentCount    = static_cast<uint32_t>(imageAttachments.size());
    renderPassCreateInfo.pAttachments       = imageAttachments.data();
    renderPassCreateInfo.subpassCount       = static_cast<uint32_t >(subpasses.size());
    renderPassCreateInfo.pSubpasses         = subpasses.data();
    renderPassCreateInfo.flags              = VkRenderPassCreateFlags{};
    renderPassCreateInfo.dependencyCount    = static_cast<uint32_t >(dependecies.size());
    renderPassCreateInfo.pDependencies      = dependecies.data();


    VkResult result = vkCreateRenderPass(device, &renderPassCreateInfo,nullptr, m_Renderpass.reset());

    vkIfFailThrowMessage(result,"Error creating renderpass!");
}

VkRenderPass Renderpass::renderpass()const
{
    return m_Renderpass;
}