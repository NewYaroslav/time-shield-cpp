#include <time_shield/julian_conversions.hpp>

#include <cassert>
#include <cmath>

int main() {
    using namespace time_shield;

    const double epsilon = 1e-12;
    auto almost_equal = [&](double lhs, double rhs) {
        return std::fabs(lhs - rhs) < epsilon;
    };

    assert(almost_equal(ts_to_jd(static_cast<ts_t>(0)), 2440587.5));
    assert(almost_equal(ts_to_mjd(static_cast<ts_t>(0)), 40587.0));
    assert(gregorian_ymd_to_jdn(1970, 1, 1) == static_cast<jdn_t>(2440588));

    assert(almost_equal(gregorian_ymd_to_jd(2000, 1, 1, 12, 0), 2451545.0));

    assert(almost_equal(ts_to_jd(static_cast<ts_t>(-SEC_PER_DAY)), 2440586.5));
    assert(gregorian_ymd_to_jdn(1969, 12, 31) == static_cast<jdn_t>(2440587));

    assert(almost_equal(gregorian_to_jd(2.5, 5, 2024), gregorian_ymd_to_jd(2024, 5, 2, 12, 0)));

    assert(almost_equal(gregorian_to_jd(2U, 5U, 2024U, 12U, 0U, 0U, 0U), gregorian_ymd_to_jd(2024, 5, 2, 12, 0)));
    assert(gregorian_to_jdn(2U, 5U, 2024U) == gregorian_ymd_to_jdn(2024, 5, 2));

    jd_t jd_value = -1.0;
    assert(try_gregorian_ymd_to_jd(2024, 5, 2, 12, 0, 0, 0, jd_value));
    assert(almost_equal(jd_value, 2460433.0));

    jd_t unchanged_jd = 123.456;
    assert(!try_gregorian_ymd_to_jd(2023, 2, 29, 12, 0, 0, 0, unchanged_jd));
    assert(almost_equal(unchanged_jd, 123.456));
    assert(!try_gregorian_ymd_to_jd(2024, 5, 2, 24, 0, 0, 0, unchanged_jd));
    assert(almost_equal(unchanged_jd, 123.456));

    jdn_t jdn_value = 0;
    assert(try_gregorian_ymd_to_jdn(2024, 5, 2, jdn_value));
    assert(jdn_value == static_cast<jdn_t>(2460433));

    jdn_t unchanged_jdn = 777U;
    assert(!try_gregorian_ymd_to_jdn(2024, 0, 2, unchanged_jdn));
    assert(unchanged_jdn == static_cast<jdn_t>(777));
    assert(!try_gregorian_ymd_to_jdn(2023, 2, 29, unchanged_jdn));
    assert(unchanged_jdn == static_cast<jdn_t>(777));

    return 0;
}
