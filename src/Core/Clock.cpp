//
// Created by hindrik on 29-11-16.
//

#include "Clock.h"

long long Clock::getDeltaTime() {
    const auto current = std::chrono::steady_clock::now();
    const auto elapsed = current - m_previousFrame;

    m_previousFrame = current;
    return static_cast<long long>(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count());
}