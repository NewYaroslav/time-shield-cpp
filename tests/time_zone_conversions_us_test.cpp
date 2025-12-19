#include <time_shield/time_zone_conversions.hpp>
#include <time_shield/time_conversions.hpp>
#include <cassert>

/// \brief Tests US ET/CT conversions to UTC including DST transitions.
int main() {
    using namespace time_shield;

    auto first_sunday_month_day = [](int year, int month) {
        return static_cast<int>(
            1 + (DAYS_PER_WEEK - day_of_week_date(year, month, 1)) % DAYS_PER_WEEK);
    };

    auto second_sunday_month_day = [&](int year, int month) {
        return first_sunday_month_day(year, month) + 7;
    };

    ts_t ct_standard = to_timestamp(2024, 1, 15, 12, 0, 0);
    ts_t gmt_ct_standard = ct_to_gmt(ct_standard);
    assert(gmt_ct_standard == to_timestamp(2024, 1, 15, 18, 0, 0));

    ts_t et_standard = to_timestamp(2024, 1, 15, 12, 0, 0);
    ts_t gmt_et_standard = et_to_gmt(et_standard);
    assert(gmt_et_standard == to_timestamp(2024, 1, 15, 17, 0, 0));

    ts_t ct_summer = to_timestamp(2024, 7, 15, 12, 0, 0);
    ts_t gmt_ct_summer = ct_to_gmt(ct_summer);
    assert(gmt_ct_summer == to_timestamp(2024, 7, 15, 17, 0, 0));

    ts_t et_summer = to_timestamp(2024, 7, 15, 12, 0, 0);
    ts_t gmt_et_summer = et_to_gmt(et_summer);
    assert(gmt_et_summer == to_timestamp(2024, 7, 15, 16, 0, 0));

    ts_t ct_wrap_check = to_timestamp(2024, 6, 10, 9, 0, 0);
    assert(ct_to_gmt(ct_wrap_check) == et_to_gmt(ct_wrap_check + SEC_PER_HOUR));

    ts_t gmt_wrap_check = to_timestamp(2024, 6, 10, 15, 0, 0);
    assert(gmt_to_ct(gmt_wrap_check) == gmt_to_et(gmt_wrap_check) - SEC_PER_HOUR);

    int spring_day_2024 = static_cast<int>(second_sunday_month_day(2024, MAR));
    ts_t et_before_spring = to_timestamp(2024, int(MAR), spring_day_2024, 1, 59, 59);
    ts_t gmt_et_before_spring = et_to_gmt(et_before_spring);
    assert(gmt_et_before_spring == to_timestamp(2024, int(MAR), spring_day_2024, 6, 59, 59));

    ts_t et_after_spring = to_timestamp(2024, int(MAR), spring_day_2024, 3, 0, 0);
    ts_t gmt_et_after_spring = et_to_gmt(et_after_spring);
    assert(gmt_et_after_spring == to_timestamp(2024, int(MAR), spring_day_2024, 7, 0, 0));

    ts_t ct_before_spring = to_timestamp(2024, int(MAR), spring_day_2024, 1, 59, 59);
    ts_t gmt_before_spring = ct_to_gmt(ct_before_spring);
    assert(gmt_before_spring == to_timestamp(2024, int(MAR), spring_day_2024, 6, 59, 59));

    ts_t ct_after_spring = to_timestamp(2024, int(MAR), spring_day_2024, 3, 0, 0);
    ts_t gmt_after_spring = ct_to_gmt(ct_after_spring);
    assert(gmt_after_spring == to_timestamp(2024, int(MAR), spring_day_2024, 8, 0, 0));

    int fall_day_2024 = static_cast<int>(first_sunday_month_day(2024, NOV));
    ts_t et_repeat_hour = to_timestamp(2024, int(NOV), fall_day_2024, 1, 30, 0);
    ts_t gmt_et_repeat_hour = et_to_gmt(et_repeat_hour);
    assert(gmt_et_repeat_hour == to_timestamp(2024, int(NOV), fall_day_2024, 5, 30, 0));

    ts_t ct_repeat_hour = to_timestamp(2024, int(NOV), fall_day_2024, 1, 30, 0);
    ts_t gmt_repeat_hour = ct_to_gmt(ct_repeat_hour);
    assert(gmt_repeat_hour == to_timestamp(2024, int(NOV), fall_day_2024, 7, 30, 0));

    ts_t ct_after_fall = to_timestamp(2024, int(NOV), fall_day_2024, 2, 0, 0);
    ts_t gmt_after_fall = ct_to_gmt(ct_after_fall);
    assert(gmt_after_fall == to_timestamp(2024, int(NOV), fall_day_2024, 8, 0, 0));

    (void)gmt_ct_standard;
    (void)gmt_et_standard;
    (void)gmt_ct_summer;
    (void)gmt_et_summer;
    (void)ct_wrap_check;
    (void)gmt_wrap_check;
    (void)spring_day_2024;
    (void)et_before_spring;
    (void)gmt_et_before_spring;
    (void)et_after_spring;
    (void)gmt_et_after_spring;
    (void)ct_before_spring;
    (void)gmt_before_spring;
    (void)ct_after_spring;
    (void)gmt_after_spring;
    (void)fall_day_2024;
    (void)et_repeat_hour;
    (void)gmt_et_repeat_hour;
    (void)ct_repeat_hour;
    (void)gmt_repeat_hour;
    (void)ct_after_fall;
    (void)gmt_after_fall;

    return 0;
}
