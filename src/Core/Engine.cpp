//
// Created by hindrik on 27-11-16.
//

#include "Engine.h"
#include "SynchedTask.h"
#include "../Utility Classes/Nullable.h"

void Engine::run() {
    while(!m_Quit)
    {
        std::chrono::nanoseconds deltaTime = m_InternalClock.getDeltaTime();

        processFrameTime(deltaTime);

        if(!m_Renderer.processAPI(deltaTime))
        {
            m_Quit = false;
            break;
        }
        m_Renderer.render(deltaTime);
    }
}

Engine::Engine() : m_Threadpool(( readConfigFiles(), std::thread::hardware_concurrency())), m_Renderer(*this)
{

}

Engine::~Engine() 
{
}

ThreadPool &Engine::threadPool()
{
    return m_Threadpool;
}

void Engine::processFrameTime(nanoseconds dt)
{
    if(dt == nanoseconds(0))
    {
        dt = nanoseconds(1);
    }
}

void Engine::readConfigFiles()
{
    m_ConfigManager.parseFile("config.cfg");

    for(auto& a : m_ConfigManager.map())
    {
        Logger::log("Option: " + a.first + " Value: " + a.second);
    }

}

const map<string, string> &Engine::configuration() {
    return m_ConfigManager.map();
}
