#include <time_shield/time_conversions.hpp>

#include <cassert>

int main() {
    using namespace time_shield;

    const ts_t pre_epoch = to_timestamp(1969, 12, 31, 23, 59, 59);
    const ts_t pre_epoch_start = to_timestamp(1969, 12, 31, 0, 0, 0);
    const ts_t pre_epoch_prev_day = to_timestamp(1969, 12, 30, 0, 0, 0);

    assert(pre_epoch == -1);
    assert(start_of_day(pre_epoch) == pre_epoch_start);
    assert(start_of_day(pre_epoch_start) == pre_epoch_start);
    assert(start_of_day(-86400) == pre_epoch_start);
    assert(start_of_prev_day(pre_epoch_start) == pre_epoch_prev_day);
    assert(end_of_day(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));
    assert(end_of_day(pre_epoch_start) == to_timestamp(1969, 12, 31, 23, 59, 59));

    assert(start_of_hour(pre_epoch) == to_timestamp(1969, 12, 31, 23, 0, 0));
    assert(end_of_hour(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));
    assert(start_of_min(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 0));
    assert(end_of_min(pre_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));

    assert(min_of_day(pre_epoch) == 1439);
    assert(hour_of_day(pre_epoch) == 23);
    assert(min_of_hour(pre_epoch) == 59);
    assert(weekday_of_ts(pre_epoch) == WED);

    assert(start_of_period(300, pre_epoch) == -300);
    assert(end_of_period(300, pre_epoch) == -1);

    const ts_ms_t pre_epoch_ms = to_timestamp_ms(1969, 12, 31, 23, 59, 59, 999);
    const ts_ms_t pre_epoch_start_ms = sec_to_ms(pre_epoch_start);

    assert(pre_epoch_ms == -1);
    assert(start_of_day_ms(pre_epoch_ms) == pre_epoch_start_ms);
    assert(start_of_day_ms(pre_epoch_start_ms - 1) == sec_to_ms(pre_epoch_prev_day));
    assert(end_of_day_ms(pre_epoch_ms) == pre_epoch_start_ms + MS_PER_DAY - 1);
    assert(start_of_hour_ms(pre_epoch_ms) == sec_to_ms(start_of_hour(pre_epoch)));
    assert(start_of_hour_ms(-1000) == sec_to_ms(to_timestamp(1969, 12, 31, 23, 0, 0)));
    assert(start_of_hour_ms(-1001) == sec_to_ms(to_timestamp(1969, 12, 31, 23, 0, 0)));
    assert(end_of_hour_ms(pre_epoch_ms) == sec_to_ms(end_of_hour(pre_epoch)) + 999);

    return 0;
}
