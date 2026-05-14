#include <time_shield/time_conversions.hpp>
#include <time_shield/detail/floor_math.hpp>

#include "test_assert.hpp"
#include <cstdint>

namespace {
    int64_t expected_ts_ms(int64_t year, int month, int day, int hour, int min, int sec, int ms) {
        const int64_t unix_day = static_cast<int64_t>(time_shield::date_to_unix_day(year, month, day));
        int64_t sec_value = unix_day * static_cast<int64_t>(time_shield::SEC_PER_DAY) +
            static_cast<int64_t>(hour) * static_cast<int64_t>(time_shield::SEC_PER_HOUR) +
            static_cast<int64_t>(min) * static_cast<int64_t>(time_shield::SEC_PER_MIN) +
            static_cast<int64_t>(sec);
        int64_t ms_value = static_cast<int64_t>(ms);
        sec_value += time_shield::detail::floor_div(ms_value, static_cast<int64_t>(time_shield::MS_PER_SEC));
        ms_value = time_shield::detail::floor_mod(ms_value, static_cast<int64_t>(time_shield::MS_PER_SEC));
        return sec_value * static_cast<int64_t>(time_shield::MS_PER_SEC) + ms_value;
    }
}

int main() {
    using namespace time_shield;

    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, 0) == -1000);
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, 1) == -999);
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, 500) == -500);
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, 999) == -1);
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1970, 1, 1, 0, 0, 0, 0) == 0);
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1970, 1, 1, 0, 0, 0, 1) == 1);

    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, 1000) ==
        expected_ts_ms(1969, 12, 31, 23, 59, 59, 1000));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, -1) ==
        expected_ts_ms(1969, 12, 31, 23, 59, 59, -1));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, -1000) ==
        expected_ts_ms(1969, 12, 31, 23, 59, 59, -1000));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, -1001) ==
        expected_ts_ms(1969, 12, 31, 23, 59, 59, -1001));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1969, 12, 31, 23, 59, 59, 1500) ==
        expected_ts_ms(1969, 12, 31, 23, 59, 59, 1500));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1970, 1, 1, 0, 0, 0, -1) ==
        expected_ts_ms(1970, 1, 1, 0, 0, 0, -1));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1970, 1, 1, 0, 0, 0, 1234567) ==
        expected_ts_ms(1970, 1, 1, 0, 0, 0, 1234567));

    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(1900, 3, 1, 0, 0, 0, 123) ==
        expected_ts_ms(1900, 3, 1, 0, 0, 0, 123));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(2000, 2, 29, 12, 34, 56, 789) ==
        expected_ts_ms(2000, 2, 29, 12, 34, 56, 789));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(2100, 3, 1, 0, 0, 0, 0) ==
        expected_ts_ms(2100, 3, 1, 0, 0, 0, 0));

    DateTimeStruct dt{1969, 12, 31, 23, 59, 59, 500};
    TIME_SHIELD_TEST_CHECK(dt_to_timestamp_ms(dt) == to_timestamp_ms(1969, 12, 31, 23, 59, 59, 500));

    return 0;
}
