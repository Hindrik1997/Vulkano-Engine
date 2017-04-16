//
// Created by hindrik on 27-11-16.
//

#include "Engine.h"
#include <memory>


void Engine::run() {
    while(!m_quit)
    {
        float deltaTime = m_internalClock.getDeltaTime();

        if(!m_renderer.processAPI(deltaTime)) {
            m_quit = false;
            break;
        }
        m_renderer.render(deltaTime);
    }
}

Engine::Engine() : m_Threadpool(std::thread::hardware_concurrency()), m_renderer(*this)
{
}

Engine::~Engine() {
}

ThreadPool &Engine::threadPool()
{
    return m_Threadpool;
}
