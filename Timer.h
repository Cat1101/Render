//
// Created by kotk4 on 30.08.2024.
//

#ifndef RENDER_TIMER_H
#define RENDER_TIMER_H

#include "chrono"

class Timer{
private:
    bool isRunning = false;

    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> stop;
public:
    Timer();
    float GetMsElapsed();
    bool Start();
    bool Stop();
    bool Restart();
};

#endif //RENDER_TIMER_H
