#include <time_shield/validation.hpp>
#include <time_shield/time_parser.hpp>
#include "test_assert.hpp"
#include <string>

/// \brief Validates the is_workday overload set.
int main() {
    using namespace time_shield;

    const ts_t weekday_ts = 1710720000; // 2024-03-18 (Monday)
    const ts_t weekend_ts = 1710547200; // 2024-03-16 (Saturday)

    TIME_SHIELD_TEST_CHECK(is_workday(weekday_ts));
    TIME_SHIELD_TEST_CHECK(!is_workday(weekend_ts));

    const ts_ms_t weekday_ms = weekday_ts * MS_PER_SEC;
    const ts_ms_t weekend_ms = weekend_ts * MS_PER_SEC + 500;

    TIME_SHIELD_TEST_CHECK(is_workday_ms(weekday_ms));
    TIME_SHIELD_TEST_CHECK(!is_workday_ms(weekend_ms));

    TIME_SHIELD_TEST_CHECK(is_workday(2024, 3, 18));
    TIME_SHIELD_TEST_CHECK(!is_workday(2024, 3, 16));

    const std::string weekday_iso = "2024-03-18T00:00:00Z";
    const std::string weekend_iso = "2024-03-16T00:00:00Z";
    const std::string weekday_iso_ms = "2024-03-18T00:00:00.500Z";
    const std::string weekend_iso_ms = "2024-03-16T00:00:00.500Z";

    TIME_SHIELD_TEST_CHECK(is_workday(weekday_iso));
    TIME_SHIELD_TEST_CHECK(!is_workday(weekend_iso));
    TIME_SHIELD_TEST_CHECK(is_workday_ms(weekday_iso_ms));
    TIME_SHIELD_TEST_CHECK(!is_workday_ms(weekend_iso_ms));

    TIME_SHIELD_TEST_CHECK(!is_workday("not-a-date"));
    TIME_SHIELD_TEST_CHECK(!is_workday_ms("2024-13-40T00:00:00.000Z"));

    (void)weekday_ts;
    (void)weekend_ts;
    (void)weekday_ms;
    (void)weekend_ms;
    (void)weekday_iso;
    (void)weekend_iso;
    (void)weekday_iso_ms;
    (void)weekend_iso_ms;

    return 0;
}
