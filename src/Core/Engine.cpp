//
// Created by hindrik on 27-11-16.
//

#include "Engine.h"
#include <memory>

void Engine::run() {
    while(!m_quit)
    {
        long long deltaTime = m_internalClock.getDeltaTime();
		float dt = static_cast<float>(deltaTime);


        if(!m_renderer.processAPI(dt)) {
            m_quit = false;
            break;
        }
        m_renderer.render(dt);
    }
}

Engine::Engine() : m_Threadpool(std::thread::hardware_concurrency()), m_renderer(*this)
{
}

Engine::~Engine() 
{
}

ThreadPool &Engine::threadPool()
{
    return m_Threadpool;
}
