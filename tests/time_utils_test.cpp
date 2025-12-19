#include <time_shield/CpuTickTimer.hpp>
#include <time_shield/time_utils.hpp>

#include <cassert>
#include <cmath>
#include <thread>
#include <chrono>

/// \brief Basic checks for time utility helpers.
int main() {
    using namespace time_shield;

    int ns = ns_of_sec();
    assert(ns >= 0 && ns < NS_PER_SEC);

    int us = us_of_sec();
    assert(us >= 0 && us < US_PER_SEC);

    int ms = ms_of_sec();
    assert(ms >= 0 && ms < MS_PER_SEC);

    ts_ms_t t1 = ts_ms();
    ts_ms_t t2 = timestamp_ms();
    assert(t2 >= t1 && t2 - t1 < MS_PER_SEC);

    ts_us_t u1 = ts_us();
    ts_us_t u2 = timestamp_us();
    assert(u2 >= u1 && u2 - u1 < US_PER_SEC);

    const int64_t rt1 = now_realtime_us();
    const int64_t rt2 = now_realtime_us();
    assert(rt2 >= rt1);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    const int64_t rt3 = now_realtime_us();
    assert(rt3 >= rt2);

    CpuTickTimer timer{};
    double first_sample = timer.record_sample();
    assert(timer.sample_count() == 1);
    assert(timer.total_ticks() >= 0.0);
    assert(timer.last_sample_ticks() == first_sample);
    assert(!std::isnan(timer.average_ticks()));

    timer.stop();
    double frozen_elapsed = timer.elapsed();
    timer.stop();
    assert(timer.elapsed() == frozen_elapsed);

    double resumed_sample = timer.record_sample();
    assert(resumed_sample == 0.0);
    assert(timer.last_sample_ticks() == 0.0);
    assert(timer.sample_count() == 1);

    double second_sample = timer.record_sample();
    assert(second_sample >= 0.0);
    assert(timer.sample_count() == 2);

    timer.reset_samples();
    assert(timer.sample_count() == 0);
    assert(std::isnan(timer.average_ticks()));

    CpuTickTimer manual_timer{false};
    assert(manual_timer.elapsed() == 0.0);
    assert(std::isnan(manual_timer.average_ticks()));
    assert(manual_timer.sample_count() == 0);

    double no_sample = manual_timer.record_sample();
    assert(no_sample == 0.0);
    assert(manual_timer.sample_count() == 0);
    double collected = manual_timer.record_sample();
    assert(collected >= 0.0);
    assert(manual_timer.sample_count() == 1);

    manual_timer.stop();
    double manual_frozen = manual_timer.elapsed();
    assert(manual_timer.elapsed() == manual_frozen);

    manual_timer.restart();
    assert(manual_timer.sample_count() == 0);

    (void)ns;
    (void)us;
    (void)ms;
    (void)t1;
    (void)t2;
    (void)u1;
    (void)u2;
    (void)rt1;
    (void)rt2;
    (void)rt3;
    (void)first_sample;
    (void)frozen_elapsed;
    (void)resumed_sample;
    (void)second_sample;
    (void)no_sample;
    (void)collected;
    (void)manual_frozen;

    return 0;
}
