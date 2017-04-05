//
// Created by hindrik on 5-4-17.
//

#ifndef VULKANOENGINE_RENDERMODE_H
#define VULKANOENGINE_RENDERMODE_H

#include <string>
#include <iostream>

using std::string;

class RenderMode
{
private:
    string m_Name;
public:
    RenderMode(string name);
    virtual void initialize()                   = 0;
    virtual void render(const ManagedTargetOutput& output, float deltaTime)        = 0;


};

inline RenderMode::RenderMode(string name) : m_Name(name)
{
}



class NullRenderMode : public RenderMode
{
public:
    NullRenderMode();

    void initialize();
    void render(const ManagedTargetOutput& output,float deltaTime);

};

inline NullRenderMode::NullRenderMode() : RenderMode("Null rendering mode")
{
}

inline void NullRenderMode::render(const ManagedTargetOutput& output, float deltaTime)
{

}

inline void NullRenderMode::initialize()
{
    std::cout << "Initializing null rendering mode. This will swallow all rendering commands." << std::endl;
}


#endif //VULKANOENGINE_RENDERMODE_H
