//
// Created by hindrik on 27-11-16.
//

#ifndef VULKANO_ENGINE_ENGINE_H
#define VULKANO_ENGINE_ENGINE_H

#include "Clock.h"
#include "../Data Storage/CacheOptimizedStorage.h"
#include "../Rendering/Renderer.h"

#define SAFE_PNTR_DEL(x) if(x != nullptr) delete x;

class Engine {
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
