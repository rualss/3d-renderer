#pragma once

#include <chrono>

namespace renderer {

class Time {
public:
    using TimeUnit = double;
    using Duration = std::chrono::high_resolution_clock::duration;

    Time(Duration duration);
    TimeUnit ToSeconds();
    TimeUnit ToMilliseconds();
    TimeUnit ToMicroseconds();

private:
    Duration duration_;
};

class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

    Timer();
    Time Elapsed();

private:
    TimePoint latest_time_;
};

}  // namespace renderer
