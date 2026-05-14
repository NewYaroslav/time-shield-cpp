#include <time_shield/time_conversions.hpp>

#include "test_assert.hpp"

int main() {
    using namespace time_shield;

    TIME_SHIELD_TEST_CHECK(detail::floor_div<ts_t>(-1, SEC_PER_DAY) == -1);
    TIME_SHIELD_TEST_CHECK(detail::floor_mod<ts_t>(-1, SEC_PER_DAY) == SEC_PER_DAY - 1);
    TIME_SHIELD_TEST_CHECK(detail::floor_div<int64_t>(-1001, 1000) == -2);
    TIME_SHIELD_TEST_CHECK(detail::floor_mod<int64_t>(-1001, 1000) == 999);

    const ts_t pre_epoch = to_timestamp(1969, 12, 31, 23, 59, 59);
    const ts_t pre_epoch_start = to_timestamp(1969, 12, 31, 0, 0, 0);
    const ts_t pre_epoch_prev_day = to_timestamp(1969, 12, 30, 0, 0, 0);

    TIME_SHIELD_TEST_CHECK(pre_epoch == -1);
    TIME_SHIELD_TEST_CHECK(start_of_day(pre_epoch) == pre_epoch_start);
    TIME_SHIELD_TEST_CHECK(start_of_day(pre_epoch_start) == pre_epoch_start);
    TIME_SHIELD_TEST_CHECK(start_of_day(-86400) == pre_epoch_start);
    TIME_SHIELD_TEST_CHECK(start_of_prev_day(pre_epoch_start) == pre_epoch_prev_day);
    TIME_SHIELD_TEST_CHECK(end_of_day(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(end_of_day(pre_epoch_start) == to_timestamp(1969, 12, 31, 23, 59, 59));

    TIME_SHIELD_TEST_CHECK(start_of_hour(pre_epoch) == to_timestamp(1969, 12, 31, 23, 0, 0));
    TIME_SHIELD_TEST_CHECK(end_of_hour(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(start_of_min(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 0));
    TIME_SHIELD_TEST_CHECK(end_of_min(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));

    TIME_SHIELD_TEST_CHECK(min_of_day(pre_epoch) == 1439);
    TIME_SHIELD_TEST_CHECK(hour_of_day(pre_epoch) == 23);
    TIME_SHIELD_TEST_CHECK(min_of_hour(pre_epoch) == 59);
    TIME_SHIELD_TEST_CHECK(weekday_of_ts(pre_epoch) == WED);

    TIME_SHIELD_TEST_CHECK(start_of_month(pre_epoch) == to_timestamp(1969, 12, 1, 0, 0, 0));
    TIME_SHIELD_TEST_CHECK(end_of_month(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(last_sunday_of_month(pre_epoch) == to_timestamp(1969, 12, 28, 0, 0, 0));
    TIME_SHIELD_TEST_CHECK(start_of_week(pre_epoch) == to_timestamp(1969, 12, 28, 0, 0, 0));
    TIME_SHIELD_TEST_CHECK(end_of_week(pre_epoch) == to_timestamp(1970, 1, 3, 23, 59, 59));

    TIME_SHIELD_TEST_CHECK(start_of_period(300, pre_epoch) == -300);
    TIME_SHIELD_TEST_CHECK(end_of_period(300, pre_epoch) == -1);
    TIME_SHIELD_TEST_CHECK(start_of_period(1, pre_epoch) == pre_epoch);
    TIME_SHIELD_TEST_CHECK(end_of_period(1, pre_epoch) == pre_epoch);
    TIME_SHIELD_TEST_CHECK(start_of_period(0, pre_epoch) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(end_of_period(0, pre_epoch) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(start_of_period(-300, pre_epoch) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(end_of_period(-300, pre_epoch) == ERROR_TIMESTAMP);

    const ts_ms_t pre_epoch_ms = to_timestamp_ms(1969, 12, 31, 23, 59, 59, 999);
    const ts_ms_t pre_epoch_start_ms = sec_to_ms(pre_epoch_start);

    TIME_SHIELD_TEST_CHECK(pre_epoch_ms == -1);
    TIME_SHIELD_TEST_CHECK(start_of_day_ms(pre_epoch_ms) == pre_epoch_start_ms);
    TIME_SHIELD_TEST_CHECK(start_of_day_ms(pre_epoch_start_ms - 1) == sec_to_ms(pre_epoch_prev_day));
    TIME_SHIELD_TEST_CHECK(end_of_day_ms(pre_epoch_ms) == pre_epoch_start_ms + MS_PER_DAY - 1);
    TIME_SHIELD_TEST_CHECK(start_of_hour_ms(pre_epoch_ms) == sec_to_ms(start_of_hour(pre_epoch)));
    TIME_SHIELD_TEST_CHECK(start_of_hour_ms(-1000) == sec_to_ms(to_timestamp(1969, 12, 31, 23, 0, 0)));
    TIME_SHIELD_TEST_CHECK(start_of_hour_ms(-1001) == sec_to_ms(to_timestamp(1969, 12, 31, 23, 0, 0)));
    TIME_SHIELD_TEST_CHECK(end_of_hour_ms(pre_epoch_ms) == sec_to_ms(end_of_hour(pre_epoch)) + 999);

    return 0;
}
