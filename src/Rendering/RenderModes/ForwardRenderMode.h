//
// Created by hindrik on 8-4-17.
//

#ifndef VULKANOENGINE_FORWARDRENDERMODE_H
#define VULKANOENGINE_FORWARDRENDERMODE_H


#include "../Vulkan/Classes/RenderMode.h"
#include "../Vulkan/Classes/RenderTarget.h"

class ForwardRenderMode : public RenderMode
{
private:


public:
    ForwardRenderMode(RenderTarget&& target);
public:
    void initialize() override;

    void render(float deltaTime) override;

};


#endif //VULKANOENGINE_FORWARDRENDERMODE_H
