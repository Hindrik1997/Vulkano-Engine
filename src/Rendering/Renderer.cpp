//
// Created by hindrik on 30-1-17.
//

#include <algorithm>
#include "Renderer.h"
#include "RenderModes/ForwardRenderMode.h"

using std::begin;
using std::end;
using std::copy;


auto fill_vk_core_create_info(VK_PLATFORM& platform) -> vk_core_create_info
{
    vk_core_create_info info;

    vector<const char*> platform_extensions = platform.processExtensions(enabledInstanceExtensions);

    platform_extensions.reserve(platform_extensions.size() + enabledInstanceExtensions.size());
    copy(begin(enabledInstanceExtensions),end(enabledInstanceExtensions), std::inserter(platform_extensions, end(platform_extensions)));

    info.m_ApplicationName                          = "Vulkano Engine";
    info.m_EnabledInstanceValidationLayerNames      = enabledInstanceValidationLayers;
    info.m_EnabledInstanceExtensionNames            = platform_extensions;
    info.m_EnabledDeviceExtentionNames              = enabledDeviceExtensions;
    info.m_EnableDebugLayers                        = enableDebugLayers;
    info.m_EnumerateLayersAndExtensionsInConsole    = true;

    return info;
}


auto Renderer::render(float deltaTime) -> void
{
    for(const auto& output : m_Outputs)
    {
        output.render(deltaTime);
    }
}

auto Renderer::processAPI(float deltaTime) -> bool
{
    vector<uint32_t > mustBeClosed;
    uint32_t  i = 0;
    for(auto& output : m_Outputs)
    {
        bool x = output.processAPI(deltaTime);
        if(!x)
        {
            mustBeClosed.push_back(i);
        }
        ++i;
    }

    std::sort(mustBeClosed.begin(), mustBeClosed.end(), std::greater<uint32_t>());
    for(auto& item : mustBeClosed)
    {
        m_Outputs.erase(m_Outputs.begin() + item);
    }


    return m_Outputs.size() != 0;
}

Renderer::~Renderer()
{
}

Renderer::Renderer(Engine& engine) : m_VkCore(fill_vk_core_create_info(m_Platform), engine), m_Engine(engine)
{
    m_Outputs.emplace_back(new ForwardRenderMode(RenderTarget(800, 600, m_VkCore, m_Platform)));
    //m_Outputs.emplace_back(new NullRenderMode(RenderTarget(800, 600, m_VkCore, m_Platform)));
}


