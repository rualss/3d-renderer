#pragma once

#include <chrono>

namespace application {
class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using TimeUnit = double;

    Timer();
    void Tick();
    TimeUnit GetDelta() const;

private:
    static constexpr TimeUnit kDefaultDelta = 0.;

    TimeUnit time_delta_ = kDefaultDelta;
    TimePoint latest_time_;
};

}  // namespace application
