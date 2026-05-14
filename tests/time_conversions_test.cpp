#include <time_shield/time_conversions.hpp>
#include "test_assert.hpp"

/// \brief Basic checks for time conversion helpers.
int main() {
    using namespace time_shield;

    TIME_SHIELD_TEST_CHECK(get_unix_day_ms(86400000) == 1);
    TIME_SHIELD_TEST_CHECK(unix_day_to_timestamp(2) == 2 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(unix_day_to_timestamp_ms(2) == 2 * MS_PER_DAY);
    TIME_SHIELD_TEST_CHECK(end_of_day_from_unix_day(0) == SEC_PER_DAY - 1);
    TIME_SHIELD_TEST_CHECK(start_of_next_day_from_unix_day(0) == SEC_PER_DAY);

    TIME_SHIELD_TEST_CHECK(start_of_min(61) == 60);
    TIME_SHIELD_TEST_CHECK(end_of_min(60) == 119);

    const ts_t first_workday_start = start_of_first_workday_month(2024, 6);
    TIME_SHIELD_TEST_CHECK(first_workday_start == to_timestamp(2024, 6, 3));
    TIME_SHIELD_TEST_CHECK(end_of_first_workday_month(2024, 6) == end_of_day(first_workday_start));

    const ts_t last_workday_start = start_of_last_workday_month(2024, 3);
    TIME_SHIELD_TEST_CHECK(last_workday_start == to_timestamp(2024, 3, 29));
    TIME_SHIELD_TEST_CHECK(end_of_last_workday_month_ms(2024, 3) == end_of_day_ms(sec_to_ms(last_workday_start)));

    TIME_SHIELD_TEST_CHECK(start_of_first_workday_month(2024, 13) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(end_of_last_workday_month_ms(2024, 0) == ERROR_TIMESTAMP);

    (void)first_workday_start;
    (void)last_workday_start;

    return 0;
}
