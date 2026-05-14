#include <time_shield/CpuTickTimer.hpp>
#include <time_shield/time_utils.hpp>

#include "test_assert.hpp"
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <thread>

/// \brief Basic checks for time utility helpers.
int main() {
    using namespace time_shield;

    int ns = ns_of_sec();
    TIME_SHIELD_TEST_CHECK(ns >= 0 && ns < NS_PER_SEC);

    int us = us_of_sec();
    TIME_SHIELD_TEST_CHECK(us >= 0 && us < US_PER_SEC);

    int ms = ms_of_sec();
    TIME_SHIELD_TEST_CHECK(ms >= 0 && ms < MS_PER_SEC);

    ts_ms_t t1 = ts_ms();
    ts_ms_t t2 = timestamp_ms();
    TIME_SHIELD_TEST_CHECK(t2 >= t1 && t2 - t1 < MS_PER_SEC);

    ts_us_t u1 = ts_us();
    ts_us_t u2 = timestamp_us();
    TIME_SHIELD_TEST_CHECK(u2 >= u1 && u2 - u1 < US_PER_SEC);

    const int64_t rt1 = now_realtime_us();
    const int64_t rt2 = now_realtime_us();
    TIME_SHIELD_TEST_CHECK(rt2 >= rt1);

    const ts_t mono_sec_1 = monotonic_sec();
    const ts_t mono_sec_2 = monotonic_sec();
    TIME_SHIELD_TEST_CHECK(mono_sec_2 >= mono_sec_1);

    const ts_ms_t mono_ms_1 = monotonic_ms();
    const ts_ms_t mono_ms_2 = monotonic_ms();
    TIME_SHIELD_TEST_CHECK(mono_ms_2 >= mono_ms_1);

    const ts_us_t mono_us_1 = monotonic_us();
    const ts_us_t mono_us_2 = monotonic_us();
    TIME_SHIELD_TEST_CHECK(mono_us_2 >= mono_us_1);

    const ts_ms_t mono_ms_from_us_1 = static_cast<ts_ms_t>(mono_us_1 / 1000);
    const ts_ms_t mono_ms_from_us_2 = static_cast<ts_ms_t>(mono_us_2 / 1000);
    TIME_SHIELD_TEST_CHECK(mono_ms_from_us_2 >= mono_ms_from_us_1);
    TIME_SHIELD_TEST_CHECK(std::llabs(mono_ms_1 - mono_ms_from_us_1) <= 1);
    TIME_SHIELD_TEST_CHECK(std::llabs(mono_ms_2 - mono_ms_from_us_2) <= 1);
    TIME_SHIELD_TEST_CHECK(static_cast<ts_t>(mono_ms_1 / 1000) >= mono_sec_1);
    TIME_SHIELD_TEST_CHECK(static_cast<ts_t>(mono_ms_2 / 1000) >= mono_sec_2);
    TIME_SHIELD_TEST_CHECK(std::llabs(static_cast<long long>(mono_sec_1 - static_cast<ts_t>(mono_ms_1 / 1000))) <= 1);
    TIME_SHIELD_TEST_CHECK(std::llabs(static_cast<long long>(mono_sec_2 - static_cast<ts_t>(mono_ms_2 / 1000))) <= 1);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    const int64_t rt3 = now_realtime_us();
    TIME_SHIELD_TEST_CHECK(rt3 >= rt2);

    const ts_t mono_sec_3 = monotonic_sec();
    const ts_ms_t mono_ms_3 = monotonic_ms();
    const ts_us_t mono_us_3 = monotonic_us();
    TIME_SHIELD_TEST_CHECK(mono_sec_3 >= mono_sec_2);
    TIME_SHIELD_TEST_CHECK(mono_ms_3 >= mono_ms_2);
    TIME_SHIELD_TEST_CHECK(mono_us_3 >= mono_us_2);
    TIME_SHIELD_TEST_CHECK(mono_ms_3 - mono_ms_1 >= 1);
    TIME_SHIELD_TEST_CHECK(mono_us_3 - mono_us_1 >= 1000);

    CpuTickTimer timer{};
    double first_sample = timer.record_sample();
    TIME_SHIELD_TEST_CHECK(timer.sample_count() == 1);
    TIME_SHIELD_TEST_CHECK(timer.total_ticks() >= 0.0);
    TIME_SHIELD_TEST_CHECK(timer.last_sample_ticks() == first_sample);
    TIME_SHIELD_TEST_CHECK(!std::isnan(timer.average_ticks()));

    timer.stop();
    double frozen_elapsed = timer.elapsed();
    timer.stop();
    TIME_SHIELD_TEST_CHECK(timer.elapsed() == frozen_elapsed);

    double resumed_sample = timer.record_sample();
    TIME_SHIELD_TEST_CHECK(resumed_sample == 0.0);
    TIME_SHIELD_TEST_CHECK(timer.last_sample_ticks() == 0.0);
    TIME_SHIELD_TEST_CHECK(timer.sample_count() == 1);

    double second_sample = timer.record_sample();
    TIME_SHIELD_TEST_CHECK(second_sample >= 0.0);
    TIME_SHIELD_TEST_CHECK(timer.sample_count() == 2);

    timer.reset_samples();
    TIME_SHIELD_TEST_CHECK(timer.sample_count() == 0);
    TIME_SHIELD_TEST_CHECK(std::isnan(timer.average_ticks()));

    CpuTickTimer manual_timer{false};
    TIME_SHIELD_TEST_CHECK(manual_timer.elapsed() == 0.0);
    TIME_SHIELD_TEST_CHECK(std::isnan(manual_timer.average_ticks()));
    TIME_SHIELD_TEST_CHECK(manual_timer.sample_count() == 0);

    double no_sample = manual_timer.record_sample();
    TIME_SHIELD_TEST_CHECK(no_sample == 0.0);
    TIME_SHIELD_TEST_CHECK(manual_timer.sample_count() == 0);
    double collected = manual_timer.record_sample();
    TIME_SHIELD_TEST_CHECK(collected >= 0.0);
    TIME_SHIELD_TEST_CHECK(manual_timer.sample_count() == 1);

    manual_timer.stop();
    double manual_frozen = manual_timer.elapsed();
    TIME_SHIELD_TEST_CHECK(manual_timer.elapsed() == manual_frozen);

    manual_timer.restart();
    TIME_SHIELD_TEST_CHECK(manual_timer.sample_count() == 0);

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
    (void)mono_sec_1;
    (void)mono_sec_2;
    (void)mono_sec_3;
    (void)mono_ms_1;
    (void)mono_ms_2;
    (void)mono_ms_3;
    (void)mono_us_1;
    (void)mono_us_2;
    (void)mono_us_3;
    (void)mono_ms_from_us_1;
    (void)mono_ms_from_us_2;
    (void)first_sample;
    (void)frozen_elapsed;
    (void)resumed_sample;
    (void)second_sample;
    (void)no_sample;
    (void)collected;
    (void)manual_frozen;

    return 0;
}
