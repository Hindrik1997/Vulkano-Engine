//
// Created by hindrik on 27-11-16.
//

#include "Engine.h"

void Engine::run() {
    while(!m_quit)
    {
        float deltaTime = m_internalClock.getDeltaTime();

        if(!m_renderer.processAPI(deltaTime))
        {
            m_quit = false;
            break;
        }

        m_renderer.render(deltaTime);
    }
}

Engine::Engine() {
}

Engine::~Engine() {
}