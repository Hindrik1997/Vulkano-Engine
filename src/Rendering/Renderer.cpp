//
// Created by hindrik on 30-1-17.
//

#include <limits>
#include "Renderer.h"

auto fill_vk_core_create_info(VK_PLATFORM& platform) -> vk_core_create_info
{
    vk_core_create_info info;

    platform.ProcessExtensions(enabledInstanceExtensions);

    info.m_ApplicationName                          = "Vulkano Engine";
    info.m_EnabledInstanceValidationLayerNames      = &enabledInstanceValidationLayers;
    info.m_EnabledInstanceExtensionNames            = &enabledInstanceExtensions;
    info.m_EnabledDeviceExtentionNames              = &enabledDeviceExtensions;
    info.m_EnableDebugLayers                        = enableDebugLayers;
    info.m_EnumerateLayersAndExtensionsInConsole    = false;

    return info;
}


auto Renderer::render(float deltaTime) -> void
{
    for(const auto& output : m_Outputs)
    {

    }
}

auto Renderer::processAPI(float deltaTime) -> bool
{
    return m_Platform.processAPI(deltaTime);
}

Renderer::Renderer() : m_VkCore(fill_vk_core_create_info(m_Platform))
{
    m_Outputs.emplace_back(RenderTargetOutput(1280, 800, m_VkCore,m_Platform));

    //ManagedTargetOutput& targetOutput = m_Outputs.back();

    //Swapchain& swapchain = targetOutput.swapchain();

    VkAttachmentDescription colorAttachment = {};
    //colorAttachment.format = swapchain.surfaceFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    //targetOutput.getNewRenderpass();





}

Renderer::~Renderer()
{
}


