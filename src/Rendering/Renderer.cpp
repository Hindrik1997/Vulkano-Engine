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
        m_RenderMode->render(output, deltaTime);
    }
}

auto Renderer::processAPI(float deltaTime) -> bool
{
    return m_Platform.processAPI(deltaTime);
}

Renderer::Renderer() : m_VkCore(fill_vk_core_create_info(m_Platform)), m_RenderMode(new NullRenderMode())
{
    m_RenderMode->initialize();

    m_Outputs.emplace_back(RenderTargetOutput(1280, 800, m_VkCore,m_Platform));

    ManagedTargetOutput& output = m_Outputs.back();

}

Renderer::~Renderer()
{
}

RenderMode* Renderer::assignRenderMode(RenderMode *mode)
{
    RenderMode* md = m_RenderMode.release();
    m_RenderMode.reset(mode);
    return md;
}

Renderer::Renderer(RenderMode *renderMode) : m_VkCore(fill_vk_core_create_info(m_Platform)), m_RenderMode(renderMode)
{
    m_RenderMode->initialize();

    m_Outputs.emplace_back(RenderTargetOutput(1280, 800, m_VkCore,m_Platform));

    ManagedTargetOutput& output = m_Outputs.back();
}


