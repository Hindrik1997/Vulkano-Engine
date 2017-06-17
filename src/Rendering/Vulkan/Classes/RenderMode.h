//
// Created by hindrik on 5-4-17.
//

#ifndef VULKANOENGINE_RENDERMODE_H
#define VULKANOENGINE_RENDERMODE_H

#include <string>
#include <iostream>
#include "RenderTarget.h"
#include <chrono>

using std::chrono::nanoseconds;
using std::string;

class Engine;

class RenderMode
{
protected:
    Engine& m_Engine;
    string m_Name;
    RenderTarget m_Target;
public:
    RenderMode(string name, RenderTarget&& target, Engine& engine);
    virtual ~RenderMode() = default;
public:
    virtual void render(nanoseconds deltaTime)        = 0;

    bool processAPI(nanoseconds deltaTime);
};

inline RenderMode::RenderMode(string name, RenderTarget&& target, Engine& engine) : m_Name(name), m_Target(std::move(target)), m_Engine(engine)
{
    std::stringstream str;
    str << "Initializing " << name ;
    Logger::log(str.str());
}

inline bool RenderMode::processAPI(nanoseconds deltaTime)
{
    return m_Target.processAPI(deltaTime);
}


class NullRenderMode : public RenderMode
{
public:
    NullRenderMode(RenderTarget&& target, Engine& engine);

    void render(nanoseconds deltaTime);

};

inline NullRenderMode::NullRenderMode(RenderTarget&& target, Engine& engine) : RenderMode("Null rendering mode", std::move(target), engine)
{
}

inline void NullRenderMode::render(nanoseconds deltaTime)
{

}

#endif //VULKANOENGINE_RENDERMODE_H
