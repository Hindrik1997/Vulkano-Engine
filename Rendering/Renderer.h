//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_RENDERER_H
#define VULKANOENGINE_RENDERER_H

#include "VulkanPlatforms/VulkanPlatform.h"
#include "VulkanCore.h"

static const vector<const char*> enabledKHRInstanceExtensions = {
#ifndef NDEBUG
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
    VK_KHR_SURFACE_EXTENSION_NAME,
};

static const vector<const char*> enabledInstanceValidationLayers = {
        "VK_LAYER_LUNARG_standard_validation",
        //"VK_LAYER_LUNARG_api_dump"
};

static const vector<const char*> enabledDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};



class Renderer {
private:
    VulkanCore m_VulkanCore;
public:
    Renderer();
    ~Renderer();
public:
    void render(float deltaTime);
    bool processAPI(float deltaTime);








};


#endif //VULKANOENGINE_RENDERER_H
