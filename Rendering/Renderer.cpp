//
// Created by hindrik on 30-1-17.
//

#include "Renderer.h"





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