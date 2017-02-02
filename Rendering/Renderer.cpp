//
// Created by hindrik on 30-1-17.
//

#include "Renderer.h"


const vector<string> enabledKHRInstanceExtensions = {

};

const vector<string> enabledInstanceValidationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};


#ifdef NDEBUG
    const bool enableValidationlayers = false;
#else
    const bool enableValidationLayers = true;
#endif


void Renderer::render(float deltaTime) {

}

bool Renderer::processAPI(float deltaTime) {
    return m_vk_core.process_platform_API(deltaTime);
}

Renderer::Renderer() : m_vk_core("Vulkano Engine", enabledKHRInstanceExtensions, enabledInstanceValidationLayers, enableValidationLayers)
{

}

Renderer::~Renderer() {

}