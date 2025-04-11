#include "timer.h"
#include <chrono>

namespace application {

Timer::Timer() : latest_time_(Clock::now()) {
}

void Timer::Tick() {
    TimePoint current_time = Clock::now();
    time_delta_ =
        std::chrono::duration_cast<std::chrono::microseconds>(current_time - latest_time_).count();
    latest_time_ = current_time;
}

namespace {

static constexpr Timer::TimeUnit kSecondsInMicrosecond = 1 / 1000000.;

}

Timer::TimeUnit Timer::GetDelta() const {
    return time_delta_ * kSecondsInMicrosecond;
}

}  // namespace application
