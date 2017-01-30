//
// Created by hindrik on 30-1-17.
//

#include "Renderer.h"

void Renderer::render(float deltaTime) {

}

bool Renderer::processAPI(float deltaTime) {
    return m_vk_core.process_platform_API(deltaTime);
}

void Renderer::present() {

}

Renderer::Renderer() : m_vk_core("Vulkano Engine"){

}

Renderer::~Renderer() {

}
