#include <time_shield/time_zone_conversions.hpp>
#include <time_shield/time_conversions.hpp>
#include <cassert>

/// \brief Tests CET/EET conversions to UTC including DST transitions.
///
/// DST rules follow the European Union schedule: since 2002 the shift
/// occurs at 01:00 UTC on the last Sunday of March (start) and last
/// Sunday of October (end). Older rules used different switch hours; the
/// conversion helpers document and support both behaviours.
int main() {
    using namespace time_shield;

    const int SWITCH_HOUR = 2;

    auto first_sunday_month_day = [](int year, int month) {
        return 1 + (DAYS_PER_WEEK - day_of_week_date(year, month, 1)) % DAYS_PER_WEEK;
    };

    auto second_sunday_month_day = [&](int year, int month) {
        return first_sunday_month_day(year, month) + 7;
    };

    ts_t cet_winter = to_timestamp(2023, 1, 1, 12, 0, 0);
    ts_t gmt_winter = cet_to_gmt(cet_winter);
    assert(gmt_winter == to_timestamp(2023, 1, 1, 11, 0, 0));

    ts_t cet_summer = to_timestamp(2023, 7, 1, 12, 0, 0);
    ts_t gmt_summer = cet_to_gmt(cet_summer);
    assert(gmt_summer == to_timestamp(2023, 7, 1, 10, 0, 0));

    ts_t cet_before = to_timestamp(2023, 3, 26, 1, 30, 0);
    ts_t gmt_before = cet_to_gmt(cet_before);
    assert(gmt_before == to_timestamp(2023, 3, 26, 0, 30, 0));

    ts_t cet_after = to_timestamp(2023, 3, 26, 3, 30, 0);
    ts_t gmt_after = cet_to_gmt(cet_after);
    assert(gmt_after == to_timestamp(2023, 3, 26, 1, 30, 0));

    ts_t eet_winter = to_timestamp(2023, 1, 1, 12, 0, 0);
    ts_t gmt_eet_winter = eet_to_gmt(eet_winter);
    assert(gmt_eet_winter == to_timestamp(2023, 1, 1, 10, 0, 0));

    ts_t eet_summer = to_timestamp(2023, 7, 1, 12, 0, 0);
    ts_t gmt_eet_summer = eet_to_gmt(eet_summer);
    assert(gmt_eet_summer == to_timestamp(2023, 7, 1, 9, 0, 0));

    int spring_day_2024 = second_sunday_month_day(2024, MAR);
    ts_t et_before_spring = to_timestamp(2024, int(MAR), spring_day_2024, 1, 59, 59);
    ts_t gmt_before_spring = et_to_gmt(et_before_spring);
    assert(gmt_before_spring == to_timestamp(2024, int(MAR), spring_day_2024, 6, 59, 59));

    ts_t gmt_spring_switch = to_timestamp(2024, int(MAR), spring_day_2024, 7, 0, 0);
    ts_t et_after_spring = gmt_to_et(gmt_spring_switch);
    assert(et_after_spring == to_timestamp(2024, int(MAR), spring_day_2024, 3, 0, 0));

    int fall_day_2024 = first_sunday_month_day(2024, NOV);
    ts_t gmt_fall_before = to_timestamp(2024, int(NOV), fall_day_2024, 6, 59, 59);
    ts_t et_fall_before = gmt_to_et(gmt_fall_before);
    assert(gmt_fall_before - et_fall_before == SEC_PER_HOUR * 4);

    ts_t gmt_fall_after = to_timestamp(2024, int(NOV), fall_day_2024, 7, 0, 0);
    ts_t et_fall_after = gmt_to_et(gmt_fall_after);
    assert(gmt_fall_after - et_fall_after == SEC_PER_HOUR * 5);

    int spring_day_2006 = first_sunday_month_day(2006, APR);
    ts_t et_before_2006 = to_timestamp(2006, int(APR), spring_day_2006, 1, 59, 59);
    ts_t gmt_before_2006 = et_to_gmt(et_before_2006);
    assert(gmt_before_2006 == to_timestamp(2006, int(APR), spring_day_2006, 6, 59, 59));

    ts_t et_after_2006 = to_timestamp(2006, int(APR), spring_day_2006, SWITCH_HOUR, 0, 0);
    ts_t gmt_after_2006 = et_to_gmt(et_after_2006);
    assert(gmt_after_2006 == to_timestamp(2006, int(APR), spring_day_2006, 6, 0, 0));

    int fall_day_2006 = last_sunday_month_day(2006, OCT);
    ts_t et_fall_2006_before = to_timestamp(2006, int(OCT), fall_day_2006, 1, 59, 59);
    ts_t gmt_fall_2006_before = et_to_gmt(et_fall_2006_before);
    assert(gmt_fall_2006_before == to_timestamp(2006, int(OCT), fall_day_2006, 5, 59, 59));

    ts_t et_fall_2006_after = to_timestamp(2006, int(OCT), fall_day_2006, SWITCH_HOUR, 0, 0);
    ts_t gmt_fall_2006_after = et_to_gmt(et_fall_2006_after);
    assert(gmt_fall_2006_after == to_timestamp(2006, int(OCT), fall_day_2006, 7, 0, 0));

    ts_t et_round_trip_winter = to_timestamp(2024, 1, 15, 12, 0, 0);
    ts_t et_round_trip_summer = to_timestamp(2024, 7, 15, 12, 0, 0);
    assert(et_round_trip_winter == gmt_to_et(et_to_gmt(et_round_trip_winter)));
    assert(et_round_trip_summer == gmt_to_et(et_to_gmt(et_round_trip_summer)));

    ts_t gmt_round_trip_winter = to_timestamp(2024, 1, 15, 17, 0, 0);
    ts_t gmt_round_trip_summer = to_timestamp(2024, 7, 15, 16, 0, 0);
    assert(gmt_round_trip_winter == et_to_gmt(gmt_to_et(gmt_round_trip_winter)));
    assert(gmt_round_trip_summer == et_to_gmt(gmt_to_et(gmt_round_trip_summer)));

    for(int year : {2021, 2022, 2023, 2024}) {
        int day = last_sunday_month_day(year, OCT);

        ts_t cet_end_before = to_timestamp(year, int(OCT), day, 1, 30, 0);
        ts_t gmt_end_before = cet_to_gmt(cet_end_before);
        assert(gmt_end_before == to_timestamp(year, int(OCT), day - 1, 23, 30, 0));

        ts_t cet_end_after = to_timestamp(year, int(OCT), day, 3, 30, 0);
        ts_t gmt_end_after = cet_to_gmt(cet_end_after);
        assert(gmt_end_after == to_timestamp(year, int(OCT), day, 2, 30, 0));

        ts_t eet_end_before = to_timestamp(year, int(OCT), day, 2, 30, 0);
        ts_t gmt_eet_end_before = eet_to_gmt(eet_end_before);
        assert(gmt_eet_end_before == to_timestamp(year, int(OCT), day - 1, 23, 30, 0));

        ts_t eet_end_after = to_timestamp(year, int(OCT), day, 4, 30, 0);
        ts_t gmt_eet_end_after = eet_to_gmt(eet_end_after);
        assert(gmt_eet_end_after == to_timestamp(year, int(OCT), day, 2, 30, 0));

        (void)day;
        (void)cet_end_before;
        (void)gmt_end_before;
        (void)cet_end_after;
        (void)gmt_end_after;
        (void)eet_end_before;
        (void)gmt_eet_end_before;
        (void)eet_end_after;
        (void)gmt_eet_end_after;
    }

    (void)cet_winter;
    (void)gmt_winter;
    (void)cet_summer;
    (void)gmt_summer;
    (void)cet_before;
    (void)gmt_before;
    (void)cet_after;
    (void)gmt_after;
    (void)eet_winter;
    (void)gmt_eet_winter;
    (void)eet_summer;
    (void)gmt_eet_summer;
    (void)spring_day_2024;
    (void)et_before_spring;
    (void)gmt_before_spring;
    (void)gmt_spring_switch;
    (void)et_after_spring;
    (void)fall_day_2024;
    (void)gmt_fall_before;
    (void)et_fall_before;
    (void)gmt_fall_after;
    (void)et_fall_after;
    (void)spring_day_2006;
    (void)et_before_2006;
    (void)gmt_before_2006;
    (void)et_after_2006;
    (void)gmt_after_2006;
    (void)fall_day_2006;
    (void)et_fall_2006_before;
    (void)gmt_fall_2006_before;
    (void)et_fall_2006_after;
    (void)gmt_fall_2006_after;
    (void)et_round_trip_winter;
    (void)et_round_trip_summer;
    (void)gmt_round_trip_winter;
    (void)gmt_round_trip_summer;

    return 0;
}
