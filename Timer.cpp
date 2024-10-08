#include "Timer.h"
//
// Created by kotk4 on 30.08.2024.
//

Timer::Timer() {
    start = std::chrono::high_resolution_clock::now();
    stop = std::chrono::high_resolution_clock::now();
}

float Timer::GetMsElapsed() {
    if (isRunning)
    {
        auto elapsed = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start);
        return elapsed.count() / 1000.0f;
    }
    else
    {
        auto elapsed = std::chrono::duration<float, std::milli>(stop - start);
        return elapsed.count() / 1000.0f;
    }

}

bool Timer::Start() {
    if(isRunning){
        return false;
    }else{
        start = std::chrono::high_resolution_clock::now();
        isRunning = true;
        return true;
    }
}

bool Timer::Restart() {
    if(isRunning){
        start = std::chrono::high_resolution_clock::now();
        return true;
    }
    return false;
}

bool Timer::Stop() {
    if(!isRunning){
        return false;
    }else{
        stop = std::chrono::high_resolution_clock::now();
        isRunning = false;
        return true;
    }
}
