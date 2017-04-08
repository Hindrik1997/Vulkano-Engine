//
// Created by hindrik on 5-4-17.
//

#ifndef VULKANOENGINE_RENDERMODE_H
#define VULKANOENGINE_RENDERMODE_H

#include <string>
#include <iostream>
#include "RenderTarget.h"

using std::string;

class RenderMode
{
private:
    string m_Name;
protected:
    RenderTarget m_Target;
public:
    RenderMode(string name, RenderTarget&& target);
    virtual void initialize()                   = 0;
    virtual void render(float deltaTime)        = 0;

    bool processAPI(float deltaTime);

};

inline RenderMode::RenderMode(string name, RenderTarget&& target) : m_Name(name), m_Target(std::move(target))
{
    std::cout << "Initializing " << name << std::endl;
}

inline bool RenderMode::processAPI(float deltaTime)
{
    return m_Target.processAPI(deltaTime);
}


class NullRenderMode : public RenderMode
{
public:
    NullRenderMode(RenderTarget&& target);

    void initialize();
    void render(float deltaTime);

};

inline NullRenderMode::NullRenderMode(RenderTarget&& target) : RenderMode("Null rendering mode", std::move(target))
{
}

inline void NullRenderMode::render(float deltaTime)
{

}

inline void NullRenderMode::initialize()
{
    std::cout << "Initializing null rendering mode. This will swallow all rendering commands." << std::endl;
}


#endif //VULKANOENGINE_RENDERMODE_H
