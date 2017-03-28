//
// Created by hindrik on 27-11-16.
//

#ifndef VULKANO_ENGINE_ENGINE_H
#define VULKANO_ENGINE_ENGINE_H

#include "Clock.h"
#include "../Data Storage/CacheOptimizedStorage.h"
#include "../Rendering/Renderer.h"
#include "../Utility Classes/NonCopyableNonMovable.h"

class Engine : NonCopyableNonMovable {
public:
    Engine();
    ~Engine();
private:
    bool m_quit = false;
    Clock m_internalClock;
    Renderer m_renderer;
public:
    void run();
};


#endif //ASCENSION_CORE_ENGINE_H
