//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_RENDERER_H
#define VULKANOENGINE_RENDERER_H

#include "Vulkan/VulkanPlatforms/VulkanPlatform.h"
#include "Vulkan/VkCore.h"
#include "Vulkan/Classes/RenderTargetOutput.h"
#include "Classes/VulkanPipelineManager.h"
#include "Classes/ManagedTargetOutput.h"
#include "RenderModes/RenderMode.h"

const bool enableDebugLayers =
#ifdef NDEBUG
        false;
#else
        true;
#endif

static vector<const char*> enabledInstanceValidationLayers = {
        "VK_LAYER_LUNARG_standard_validation",
        //"VK_LAYER_LUNARG_api_dump"
};

static vector<const char*> enabledInstanceExtensions = {
#ifndef NDEBUG
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
        VK_KHR_SURFACE_EXTENSION_NAME,
};

static vector<const char*> enabledDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};


class Renderer final : NonCopyableNonMovable {
private:
    VK_PLATFORM m_Platform;
    VkCore m_VkCore;
    vector<ManagedTargetOutput> m_Outputs;
    unique_ptr<RenderMode>      m_RenderMode;
public:
    Renderer();
    Renderer(RenderMode* renderMode);
    ~Renderer();
public:
    void render(float deltaTime);
    bool processAPI(float deltaTime);
    RenderMode* assignRenderMode(RenderMode* mode);







};


#endif //VULKANOENGINE_RENDERER_H
