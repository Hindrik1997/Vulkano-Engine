//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_RENDERER_H
#define VULKANOENGINE_RENDERER_H

#include "VulkanCore.h"

//extern const vector<const char*> enabledKHRInstanceExtensions;
//extern const vector<const char*> enabledInstanceValidationLayers;

static const vector<const char*> enabledKHRInstanceExtensions = {
#ifndef NDEBUG
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif

};

static const vector<const char*> enabledInstanceValidationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};


class Renderer {
private:
    VulkanCore m_vk_core;
public:
    Renderer();
    ~Renderer();
public:
    void render(float deltaTime);
    bool processAPI(float deltaTime);








};


#endif //VULKANOENGINE_RENDERER_H
