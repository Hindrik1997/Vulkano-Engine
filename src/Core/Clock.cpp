//
// Created by hindrik on 29-11-16.
//

#include "Clock.h"

std::chrono::nanoseconds Clock::getDeltaTime() {
    const auto current = std::chrono::steady_clock::now();
    const auto elapsed = current - m_previousFrame;

    m_previousFrame = current;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
}