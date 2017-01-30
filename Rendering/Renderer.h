//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_RENDERER_H
#define VULKANOENGINE_RENDERER_H

#include "VulkanCore.h"

class Renderer {
private:
    VulkanCore m_vk_core;
public:
    Renderer();
    ~Renderer();
public:
    void render(float deltaTime);
    bool processAPI(float deltaTime);
    void present();








};


#endif //VULKANOENGINE_RENDERER_H
