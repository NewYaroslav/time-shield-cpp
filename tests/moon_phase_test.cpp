#include <time_shield/MoonPhase.hpp>
#include <time_shield/astronomy_conversions.hpp>

#include "test_assert.hpp"
#include <cmath>

/// \brief Basic regression checks for the MoonPhase calculator.
int main() {
    using namespace time_shield;

    astronomy::MoonPhase calculator{};

    // 2024-01-01T00:00:00Z
    const double ts = 1704067200.0;
    const auto res = calculator.compute(ts);

    TIME_SHIELD_TEST_CHECK(res.phase >= 0.0 && res.phase < 1.0);
    TIME_SHIELD_TEST_CHECK(res.illumination >= 0.0 && res.illumination <= 1.0);
    TIME_SHIELD_TEST_CHECK(res.distance_km > 300000.0 && res.distance_km < 410000.0);
    TIME_SHIELD_TEST_CHECK(res.diameter_deg > 0.48 && res.diameter_deg < 0.57);
    TIME_SHIELD_TEST_CHECK(res.sun_distance_km > 140000000.0 && res.sun_distance_km < 160000000.0);
    TIME_SHIELD_TEST_CHECK(res.sun_diameter_deg > 0.5 && res.sun_diameter_deg < 0.55);

    const auto signal = moon_phase_sincos(ts);
    TIME_SHIELD_TEST_CHECK(std::abs(signal.phase_sin - res.phase_sin) < 1e-12);
    TIME_SHIELD_TEST_CHECK(std::abs(signal.phase_cos - res.phase_cos) < 1e-12);
    TIME_SHIELD_TEST_CHECK(std::abs(signal.phase_angle_rad - res.phase_angle_rad) < 1e-12);
    TIME_SHIELD_TEST_CHECK(std::abs(moon_illumination(ts) - res.illumination) < 1e-12);

    const double approx_phase = moon_phase_jd_approx(ts);
    TIME_SHIELD_TEST_CHECK(approx_phase >= 0.0 && approx_phase < 1.0);
    TIME_SHIELD_TEST_CHECK(std::abs(approx_phase - res.phase) < 0.1);
    TIME_SHIELD_TEST_CHECK(std::abs(moon_phase(ts) - res.phase) < 1e-12);

    const double approx_age_days = moon_age_days_jd_approx(ts);
    TIME_SHIELD_TEST_CHECK(approx_age_days >= 0.0 && approx_age_days < 30.0);
    TIME_SHIELD_TEST_CHECK(std::abs(approx_age_days - res.age_days) < 2.0);
    TIME_SHIELD_TEST_CHECK(std::abs(moon_age_days(ts) - res.age_days) < 1e-12);

    TIME_SHIELD_TEST_CHECK(std::abs(res.phase - calculator.compute_phase(ts)) < 1e-12);

    const auto quarters_array = calculator.quarter_times_unix(ts);
    for (std::size_t i = 1; i < quarters_array.size(); ++i) {
        TIME_SHIELD_TEST_CHECK(quarters_array[i] > quarters_array[i - 1]);
    }

    const auto quarters_struct = moon_quarters(ts);
    TIME_SHIELD_TEST_CHECK(std::abs(quarters_struct.previous_new_unix_s - quarters_array[0]) < 1e-6);
    TIME_SHIELD_TEST_CHECK(std::abs(quarters_struct.next_last_quarter_unix_s - quarters_array[7]) < 1e-6);

    const double lunar_cycle = quarters_array[4] - quarters_array[0];
    TIME_SHIELD_TEST_CHECK(lunar_cycle > 2300000.0 && lunar_cycle < 2700000.0);
    TIME_SHIELD_TEST_CHECK(ts >= quarters_array[0] && ts <= quarters_array[4]);

    const double first_gap = quarters_array[1] - quarters_array[0];
    const double second_gap = quarters_array[2] - quarters_array[1];
    const double third_gap = quarters_array[3] - quarters_array[2];
    TIME_SHIELD_TEST_CHECK(first_gap > 450000.0 && first_gap < 750000.0);
    TIME_SHIELD_TEST_CHECK(second_gap > 450000.0 && second_gap < 750000.0);
    TIME_SHIELD_TEST_CHECK(third_gap > 450000.0 && third_gap < 750000.0);

    const double before_new = quarters_struct.previous_new_unix_s - 900.0;
    const double after_new = quarters_struct.previous_new_unix_s + 900.0;
    const auto signal_before = moon_phase_sincos(before_new);
    const auto signal_after = moon_phase_sincos(after_new);
    const double dot_product = signal_before.phase_cos * signal_after.phase_cos
        + signal_before.phase_sin * signal_after.phase_sin;
    TIME_SHIELD_TEST_CHECK(dot_product > 0.95);

    TIME_SHIELD_TEST_CHECK(is_new_moon_window(before_new));
    TIME_SHIELD_TEST_CHECK(is_new_moon_window(after_new));
    TIME_SHIELD_TEST_CHECK(!is_full_moon_window(before_new, 1800.0));
    TIME_SHIELD_TEST_CHECK(!is_last_quarter_window(before_new, 1800.0));

    const double far_from_event = quarters_struct.previous_first_quarter_unix_s + 172800.0;
    TIME_SHIELD_TEST_CHECK(!is_new_moon_window(far_from_event, 3600.0));

    (void)ts;
    (void)res;
    (void)signal;
    (void)approx_phase;
    (void)approx_age_days;
    (void)quarters_array;
    (void)quarters_struct;
    (void)lunar_cycle;
    (void)first_gap;
    (void)second_gap;
    (void)third_gap;
    (void)dot_product;
    (void)far_from_event;

    return 0;
}
