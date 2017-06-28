//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_RENDERER_H
#define VULKANOENGINE_RENDERER_H

#include "Vulkan/VulkanPlatforms/VulkanPlatform.h"
#include "Vulkan/Instance.h"
#include "../Utility Classes/NonCopyableNonMovable.h"
#include "Vulkan/RenderOutput.h"
#include "Vulkan/PresentDevice.h"
#include "Vulkan/Classes/ResourceOperationManager.h"
#include <chrono>

using std::chrono::nanoseconds;

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
        "VK_LAYER_GOOGLE_unique_objects",
        //"VK_LAYER_RENDERDOC_Capture"

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
    Engine*                             m_Engine;
    VK_PLATFORM                         m_Platform;
    Nullable<Instance>                  m_Instance;
    Nullable<PresentDevice>             m_Device;
    Nullable<RenderOutput>              m_Output;
    Nullable<ResourceOperationManager>  m_ResourceOpsManager;
    Nullable<MemoryManager>             m_MemoryManager;
public:
    Renderer(Engine& engine);
    ~Renderer();
public:
    void render(nanoseconds deltaTime);
    bool processAPI(nanoseconds deltaTime);
private:
    void initialize();



};


#endif //VULKANOENGINE_RENDERER_H
