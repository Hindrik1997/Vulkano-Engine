//
// Created by hindrik on 30-1-17.
//

#include <algorithm>
#include "Renderer.h"
#include "RenderModes/ForwardRenderMode.h"
#include "../Core/Engine.h"

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
    info.m_EnableDebugLayers                        = enableDebugLayers;
    info.m_EnumerateLayersAndExtensionsInConsole    = false;

    return info;
}


auto Renderer::render(nanoseconds deltaTime) -> void
{

}

auto Renderer::processAPI(nanoseconds deltaTime) -> bool
{
    return m_Output.get().processAPI(deltaTime);
}

Renderer::~Renderer()
{
}

Renderer::Renderer(Engine& engine) : m_Engine(&engine)
{
    initialize();
}

void Renderer::initialize()
{
    uint32_t width, height;
    width = static_cast<uint32_t >(std::stoi(m_Engine->configuration().at("width")));
    height = static_cast<uint32_t >(std::stoi(m_Engine->configuration().at("height")));

    vk_core_create_info coreCreateInfo = fill_vk_core_create_info(m_Platform);
    m_Instance.set(coreCreateInfo, m_Engine);

    m_Output.set(m_Platform, m_Instance.get().instance(), width, height);

    vk_present_device_create_info info = {};
    info.m_Instance = m_Instance.get().instance();
    info.m_Surface  = m_Output.get().surface();
    info.m_EnabledDeviceExtentionNames = enabledDeviceExtensions;
    info.m_EnumerateLayersAndExtensionsInConsole = false;
    info.m_EnumeratePossibleDevicesInConsole = false;

    m_Device.set(info);

    m_ResourceOpsManager.set(m_Device.get());

    m_Output.get().initializeSwapchain(width, height, m_Instance.get(), m_Device.get(),m_Device.get().presentQueue(), m_Output.get().surface());

    m_MemoryManager.set(m_Device.get());
}
