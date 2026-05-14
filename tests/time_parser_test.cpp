#include <time_shield/time_parser.hpp>
#include "test_assert.hpp"

/// \brief Basic checks for time parsing helpers.
int main() {
    using namespace time_shield;

    TIME_SHIELD_TEST_CHECK(get_month_number("March") == MAR);

    Month m = JAN;
    TIME_SHIELD_TEST_CHECK(try_get_month_number("Aug", m) && m == AUG);

    int sec = 0;
    TIME_SHIELD_TEST_CHECK(sec_of_day("01:02:03", sec));
    TIME_SHIELD_TEST_CHECK(sec == SEC_PER_HOUR + 2 * SEC_PER_MIN + 3);
    TIME_SHIELD_TEST_CHECK(sec_of_day("01:02") == SEC_PER_HOUR + 2 * SEC_PER_MIN);

    TIME_SHIELD_TEST_CHECK(ts("1970-01-01T00:00:00Z") == 0);
    TIME_SHIELD_TEST_CHECK(ts_ms("1970-01-01T00:00:01.500Z") == 1500);
    TIME_SHIELD_TEST_CHECK(ts("2024-01-02T03:04:05Z") == ts("2024-01-02T04:04:05+01:00"));
    TIME_SHIELD_TEST_CHECK(ts("2024-01-02T03:04:05Z") == ts("2024-01-01T22:34:05-04:30"));

    (void)m;
    (void)sec;

    return 0;
}
