//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_RENDERER_H
#define VULKANOENGINE_RENDERER_H

#include "Vulkan/VulkanPlatforms/VulkanPlatform.h"
#include "Vulkan/VkCore.h"
#include "../Utility Classes/NonCopyableNonMovable.h"
#include "Vulkan/Classes/RenderTargetOutput.h"

const bool enableDebugLayers =
#ifdef NDEBUG
        false;
#else
        true;
#endif

static vector<const char*> const enabledInstanceValidationLayers = {
        //"VK_LAYER_LUNARG_standard_validation",
        //"VK_LAYER_LUNARG_api_dump"

#ifndef NDEBUG

        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_GOOGLE_unique_objects"

#endif

};

static vector<const char*> const enabledInstanceExtensions = {
#ifndef NDEBUG
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
        VK_KHR_SURFACE_EXTENSION_NAME,
};

static vector<const char*> const enabledDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

class Engine;

class Renderer final : NonCopyableNonMovable {
private:
    Engine& m_Engine;
    VK_PLATFORM m_Platform;
    VkCore m_VkCore;
    vector<RenderTargetOutput> m_Outputs;
public:
    Renderer(Engine& engine);
    ~Renderer();
public:
    void render(float deltaTime);
    bool processAPI(float deltaTime);







};


#endif //VULKANOENGINE_RENDERER_H
