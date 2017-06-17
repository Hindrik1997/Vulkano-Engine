//
// Created by hindrik on 27-11-16.
//

#ifndef VULKANO_ENGINE_ENGINE_H
#define VULKANO_ENGINE_ENGINE_H

#include <map>
#include "Clock.h"
#include "../Data Storage/CacheOptimizedStorage.h"
#include "../Rendering/Renderer.h"
#include "../Utility Classes/NonCopyableNonMovable.h"
#include "Threadpool.h"
#include "ConfigurationLoader.h"

using std::map;
using std::chrono::nanoseconds;


class Engine final : NonCopyableNonMovable {
public:
    Engine();
    ~Engine();
private:
    ConfigurationLoader m_ConfigManager;
    bool        m_Quit = false;
    Clock       m_InternalClock;
    ThreadPool  m_Threadpool;
    Renderer    m_Renderer;
public:
    void run();
    const map<string, string> & configuration();
    ThreadPool& threadPool();
private:
    void processFrameTime(nanoseconds dt);
    void readConfigFiles();
};


#endif //ASCENSION_CORE_ENGINE_H
