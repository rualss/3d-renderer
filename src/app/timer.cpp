#include "timer.h"
#include <chrono>

namespace renderer {

using namespace std::chrono;

namespace {

static constexpr Time::TimeUnit kSecondsInMicrosecond = 1 / 1000000.;
static constexpr Time::TimeUnit kMillisecondsInMicrosecond = 1 / 1000.;

}  // namespace

Time::Time(Duration duration) : duration_(duration) {
}

Time::TimeUnit Time::ToSeconds() {
    return duration_cast<microseconds>(duration_).count() * kSecondsInMicrosecond;
}

Time::TimeUnit Time::ToMilliseconds() {
    return duration_cast<microseconds>(duration_).count() * kMillisecondsInMicrosecond;
}

Time::TimeUnit Time::ToMicroseconds() {
    return duration_cast<microseconds>(duration_).count();
}

Timer::Timer() : latest_time_(Clock::now()) {
}

Time Timer::Elapsed() {
    TimePoint current_time = Clock::now();
    Time elapsed = (current_time - latest_time_);
    latest_time_ = current_time;
    return elapsed;
}

}  // namespace renderer
