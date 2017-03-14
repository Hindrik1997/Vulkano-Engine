//
// Created by hindrik on 29-11-16.
//

#include "Clock.h"

float Clock::getDeltaTime() {
    auto current = std::chrono::high_resolution_clock::now();
    auto elapsed = current - m_previousFrame;

    m_previousFrame = current;
    return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
}