#include <tools/Timing.h>

TimerDrop::TimerDrop(std::string _label, UOM _unit) 
    : label(_label), unit(_unit) {
    startTime = std::chrono::high_resolution_clock::now();
}

TimerDrop::~TimerDrop() {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = endTime - startTime;
    
    double elapsedTime  = std::chrono::duration<double>(duration).count() / (unit==MSEC? 1000:1);
    std::string unitStr = unit==MSEC? "ms": "s";
    
    printf("%32s:\t %4.4lf %s\n", label.c_str(), elapsedTime, unitStr.c_str());
}

Timer::Timer(UOM _unit, bool _printMode) : unit(_unit), printMode(_printMode) {
    start();
}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    lastCheckpointTime = startTime;
}

double Timer::checkpoint(std::string label) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = currentTime - lastCheckpointTime;
    
    double elapsedTime = std::chrono::duration<double>(duration).count() * (unit == MSEC ? 1000 : 1);
    
    if (printMode) {
        std::string unitStr = unit == MSEC ? "ms" : "s";
        printf("%32s:\t %4.4lf %s\n", label.c_str(), elapsedTime, unitStr.c_str());fflush(stdout);
    }

    lastCheckpointTime = currentTime;
    return elapsedTime;
}

double Timer::total() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = currentTime - startTime;
    
    double elapsedTime = std::chrono::duration<double>(duration).count() * (unit == MSEC ? 1000 : 1);

    if (printMode) {
        std::string unitStr = unit == MSEC ? "ms" : "s";
        printf("Total:\t %4.4lf %s\n", elapsedTime, unitStr.c_str());
    }
    
    return elapsedTime;
}