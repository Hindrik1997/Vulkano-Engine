//
// Created by hindrik on 27-11-16.
//

#ifndef VULKANO_ENGINE_ENGINE_H
#define VULKANO_ENGINE_ENGINE_H

#include "Clock.h"
#include "../Data Storage/CacheOptimizedStorage.h"
#include "../Rendering/Renderer.h"
#include "../Utility Classes/NonCopyableNonMovable.h"
#include "Threadpool.h"

class Engine final : NonCopyableNonMovable {
public:
    Engine();
    ~Engine();
private:
    bool m_quit = false;
    Clock m_internalClock;
    ThreadPool m_Threadpool;
    Renderer m_renderer;
public:
    void run();

    ThreadPool& threadPool();


};


#endif //ASCENSION_CORE_ENGINE_H
