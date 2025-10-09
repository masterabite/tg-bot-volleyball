#pragma once

#ifndef TIMING_H
#define TIMING_H

#include <iostream>
#include <chrono>
#include <string>

enum UOM {
    SEC = 1,
    MSEC = 2
};

class TimerDrop {
public:
    ~TimerDrop();
    TimerDrop(std::string _label, UOM _unit = MSEC);

private:
    std::string label;
    UOM unit;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

class Timer {
public:
    Timer(UOM _unit = MSEC, bool _printMode = false);

    void start();
    double checkpoint(std::string label);
    double total();

private:
    bool printMode;
    UOM unit;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastCheckpointTime;
};

#endif // TIMING_H