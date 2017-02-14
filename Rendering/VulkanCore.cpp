//
// Created by hindrik on 27-1-17.
//

#include <iostream>
#include "VulkanCore.h"
#include "../Core/Console.h"
#include "Utilities/Pipeline.h"


VulkanCore::VulkanCore(string applicationName,
                       vector<const char*> enabledKHRInstanceExtensions,
                       vector<const char*> enabledInstanceValidationLayers,
                       vector<const char*> enabledDeviceExtensions,
                       bool enableDebugLayers)
        : m_ApplicationName(applicationName), m_EnabledInstanceKHRExtensionNames(enabledKHRInstanceExtensions), m_EnabledInstanceValidationLayerNames(enabledInstanceValidationLayers), m_IsDebugEnabled(enableDebugLayers), m_EnabledDeviceExtensions(enabledDeviceExtensions)
{
    loadLayersAndExtensions();
    VkResult r = vkInit();
    if(r == VK_SUCCESS)
        Console::printLine("Initialized Vulkan API succesfully.");
}

VulkanCore::~VulkanCore() {

    cleanUpDebugFacilities();

    if(m_RenderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

    if(m_Swapchain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);

    cleanUpSwapchainImageViews();

    if(m_Surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

    if(m_Device != VK_NULL_HANDLE)
        vkDestroyDevice(m_Device, nullptr);

    if(m_Instance != VK_NULL_HANDLE)
        vkDestroyInstance(m_Instance,nullptr);
}

bool VulkanCore::processPlatformAPI(float deltaTime) {
    return m_Platform.processAPI(deltaTime);
}

void VulkanCore::createPipeline() {

    ShaderModule vertex = ShaderModule("/Shaders/shader.vert", ShaderModuleType::Vertex,m_Device);
    ShaderModule fragment = ShaderModule("/Shaders/shader.frag", ShaderModuleType ::Fragment, m_Device);

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertex.info(), fragment.info() };

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = Pipeline::defaultVertexInputState();
    






    VkGraphicsPipelineCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.stageCount = 2;
    info.pStages = shaderStages;







    m_ShaderModules.emplace_back(std::move(vertex));
    m_ShaderModules.emplace_back(std::move(fragment));
}

void VulkanCore::createRenderpass() {

    VkAttachmentDescription attachment = {};
    attachment.format = m_SwapChainFormat;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;

    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference reference = {};
    reference.attachment = 0;
    reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &reference;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &attachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

