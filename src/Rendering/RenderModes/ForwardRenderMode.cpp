//
// Created by hindrik on 8-4-17.
//

#include "ForwardRenderMode.h"

void ForwardRenderMode::initialize()
{

}

void ForwardRenderMode::render(float deltaTime)
{

}

ForwardRenderMode::ForwardRenderMode(RenderTarget &&target) : RenderMode("Forward render mode", std::move(target))
{


}
