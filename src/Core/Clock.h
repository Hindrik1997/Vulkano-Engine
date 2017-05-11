//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_CLOCK_H
#define VULKANO_ENGINE_CLOCK_H

#include <chrono>

class Clock {
private:
    std::chrono::time_point<std::chrono::steady_clock> m_previousFrame;
public:
    long long getDeltaTime();

};


#endif //VULKANO_ENGINE_CLOCK_H
