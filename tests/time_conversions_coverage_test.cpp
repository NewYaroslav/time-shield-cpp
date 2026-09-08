#include <time_shield/time_conversions.hpp>
#include <time_shield/time_unit_conversions.hpp>

#include "test_assert.hpp"
#include <cmath>
#include <ctime>

static_assert(time_shield::MIN_PER_30_DAY == 30 * time_shield::MIN_PER_DAY,
              "MIN_PER_30_DAY must represent thirty days");

int main() {
    using namespace time_shield;

    // time_unit_conversions
    TIME_SHIELD_TEST_CHECK(ns_of_sec(1.25) == 250000000);
    TIME_SHIELD_TEST_CHECK(us_of_sec(1.5) == 500000);
    TIME_SHIELD_TEST_CHECK(ms_of_sec(2.5) == 500);
    TIME_SHIELD_TEST_CHECK(ms_of_ts(1234) == 234);
    TIME_SHIELD_TEST_CHECK(ms_to_sec(-1) == -1);
    TIME_SHIELD_TEST_CHECK(ms_to_sec(-1000) == -1);
    TIME_SHIELD_TEST_CHECK(ms_to_sec(-1001) == -2);
    TIME_SHIELD_TEST_CHECK(sec_to_ms<>(2) == 2000);
    TIME_SHIELD_TEST_CHECK(sec_to_ms<int>(3.5) == 3500);
    TIME_SHIELD_TEST_CHECK(fsec_to_ms(1.1) == 1100);
    TIME_SHIELD_TEST_CHECK(ms_to_sec<>(1500) == 1);
    TIME_SHIELD_TEST_CHECK(ms_to_fsec(2500) == 2.5);
    TIME_SHIELD_TEST_CHECK(min_to_ms<>(2) == 120000);
    TIME_SHIELD_TEST_CHECK(min_to_ms<int>(1.5) == 90000);
    TIME_SHIELD_TEST_CHECK(ms_to_min<>(60000) == 1);
    TIME_SHIELD_TEST_CHECK(min_to_sec<>(1.5) == 90);
    TIME_SHIELD_TEST_CHECK(sec_to_min<>(180) == 3);
    TIME_SHIELD_TEST_CHECK(ms_part(-1) == 999);
    TIME_SHIELD_TEST_CHECK(ms_part(-1000) == 0);
    TIME_SHIELD_TEST_CHECK(ms_part(-1001) == 999);
    TIME_SHIELD_TEST_CHECK(us_part(static_cast<ts_us_t>(-1)) == 999999);
    TIME_SHIELD_TEST_CHECK(ns_part(-1) == 999999999);
    TIME_SHIELD_TEST_CHECK(ms_of_sec(-1.2) == 800);
    TIME_SHIELD_TEST_CHECK(us_of_sec(-1.2) == 800000);
    TIME_SHIELD_TEST_CHECK(ns_of_sec(-1.2) == 800000000);
    TIME_SHIELD_TEST_CHECK(min_to_fsec(2) == static_cast<fts_t>(SEC_PER_MIN * 2));
    TIME_SHIELD_TEST_CHECK(sec_to_fmin(180) == 3.0);
    TIME_SHIELD_TEST_CHECK(hour_to_ms<>(1) == MS_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(ms_to_hour<>(MS_PER_HOUR) == 1);
    TIME_SHIELD_TEST_CHECK(hour_to_fsec(1) == static_cast<fts_t>(SEC_PER_HOUR));
    TIME_SHIELD_TEST_CHECK(sec_to_fhour(7200) == 2.0);
    TIME_SHIELD_TEST_CHECK(sec_to_hour<>(7200) == 2);
    TIME_SHIELD_TEST_CHECK(hour_to_sec<>(1) == 3600);
    TIME_SHIELD_TEST_CHECK(sec_to_hour<>(5400) == 1);

    // unix_time_conversions and aliases
    const ts_t unix_day_two_ts = unix_day_to_ts(2);
    TIME_SHIELD_TEST_CHECK(unix_day_to_timestamp(2) == unix_day_two_ts);
    TIME_SHIELD_TEST_CHECK(unixday_to_ts(2) == unix_day_two_ts);
    TIME_SHIELD_TEST_CHECK(uday_to_ts(2) == unix_day_two_ts);
    TIME_SHIELD_TEST_CHECK(start_of_day_from_unix_day(2) == unix_day_two_ts);

    const ts_ms_t unix_day_two_ms = unix_day_to_ts_ms(2);
    TIME_SHIELD_TEST_CHECK(unix_day_to_timestamp_ms(2) == unix_day_two_ms);
    TIME_SHIELD_TEST_CHECK(unixday_to_ts_ms(2) == unix_day_two_ms);
    TIME_SHIELD_TEST_CHECK(uday_to_ts_ms(2) == unix_day_two_ms);
    TIME_SHIELD_TEST_CHECK(start_of_day_from_unix_day_ms(2) == unix_day_two_ms);

    TIME_SHIELD_TEST_CHECK(end_of_day_from_unix_day(0) == SEC_PER_DAY - 1);
    TIME_SHIELD_TEST_CHECK(end_of_day_from_unix_day_ms(0) == MS_PER_DAY - 1);
    TIME_SHIELD_TEST_CHECK(start_of_next_day_from_unix_day(0) == SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(next_day_from_unix_day(0) == SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(next_day_unix_day(0) == SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(next_day_unixday(0) == SEC_PER_DAY);

    TIME_SHIELD_TEST_CHECK(start_of_next_day_from_unix_day_ms(0) == MS_PER_DAY);
    TIME_SHIELD_TEST_CHECK(next_day_from_unix_day_ms(0) == MS_PER_DAY);
    TIME_SHIELD_TEST_CHECK(next_day_unix_day_ms(0) == MS_PER_DAY);
    TIME_SHIELD_TEST_CHECK(next_day_unixday_ms(0) == MS_PER_DAY);

    TIME_SHIELD_TEST_CHECK(days_since_epoch(SEC_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(get_unixday(SEC_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(unix_day(SEC_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(unixday(SEC_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(uday(SEC_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(get_unix_day(SEC_PER_DAY) == 1);

    TIME_SHIELD_TEST_CHECK(days_since_epoch_ms(MS_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(get_unixday_ms(MS_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(unix_day_ms(MS_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(unixday_ms(MS_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(uday_ms(MS_PER_DAY) == 1);
    TIME_SHIELD_TEST_CHECK(get_unix_day_ms(MS_PER_DAY) == 1);

    TIME_SHIELD_TEST_CHECK(days_between(0, SEC_PER_DAY * 3) == 3);

    TIME_SHIELD_TEST_CHECK(date_to_unix_day(1970, 1, 1) == 0);
    TIME_SHIELD_TEST_CHECK(years_since_epoch(static_cast<ts_t>(SEC_PER_YEAR)) == 1);

    const ts_t minute_mark = SEC_PER_MIN * 5;
    TIME_SHIELD_TEST_CHECK(min_since_epoch<>(minute_mark) == 5);
    TIME_SHIELD_TEST_CHECK(minutes_since_epoch(minute_mark) == 5);
    TIME_SHIELD_TEST_CHECK(unix_min(minute_mark) == 5);
    TIME_SHIELD_TEST_CHECK(to_unix_min(minute_mark) == 5);
    TIME_SHIELD_TEST_CHECK(umin(minute_mark) == 5);
    TIME_SHIELD_TEST_CHECK(get_unix_min(minute_mark) == 5);

    TIME_SHIELD_TEST_CHECK(sec_of_day(SEC_PER_DAY + 10) == 10);
    TIME_SHIELD_TEST_CHECK(sec_of_day_ms(MS_PER_DAY + 2000) == 2);
    TIME_SHIELD_TEST_CHECK((sec_of_day<int, int>(1, 1, 1) == SEC_PER_HOUR + SEC_PER_MIN + 1));
    TIME_SHIELD_TEST_CHECK(sec_of_min(SEC_PER_MIN + 7) == 7);
    TIME_SHIELD_TEST_CHECK(sec_of_hour(SEC_PER_HOUR + 15) == 15);

    // date_conversions and aliases
    {
        const ts_ms_t points[] = {
            static_cast<ts_ms_t>(-2001), static_cast<ts_ms_t>(-2000), static_cast<ts_ms_t>(-1999),
            static_cast<ts_ms_t>(-1001), static_cast<ts_ms_t>(-1000), static_cast<ts_ms_t>(-999),
            static_cast<ts_ms_t>(-2),    static_cast<ts_ms_t>(-1),    static_cast<ts_ms_t>(0),
            static_cast<ts_ms_t>(1),     static_cast<ts_ms_t>(999),   static_cast<ts_ms_t>(1000),
            static_cast<ts_ms_t>(1001)
        };
    
        for (size_t i = 0; i < sizeof(points)/sizeof(points[0]); ++i) {
            const ts_ms_t t = points[i];
    
            // Day boundaries in ms:
            const ts_ms_t d0 = start_of_day_ms(t);
            const ts_ms_t d1 = end_of_day_ms(t);
            TIME_SHIELD_TEST_CHECK(d0 <= t && t <= d1);
            TIME_SHIELD_TEST_CHECK(ms_part(d0) == 0);
            TIME_SHIELD_TEST_CHECK(ms_part(d1) == (MS_PER_SEC - 1));
    
            // Year boundaries in ms:
            const ts_ms_t y0 = start_of_year_ms(t);
            const ts_ms_t y1 = end_of_year_ms(t);
            TIME_SHIELD_TEST_CHECK(y0 <= t && t <= y1);
            TIME_SHIELD_TEST_CHECK(ms_part(y0) == 0);
            TIME_SHIELD_TEST_CHECK(ms_part(y1) == (MS_PER_SEC - 1));
        }
    }
    const ts_t sample_ts = to_timestamp(2024, 6, 30, 12, 0, 0);
    DateStruct sample_date{2024, 6, 30};
    TIME_SHIELD_TEST_CHECK(to_date_time(sample_ts).year == 2024);
    TIME_SHIELD_TEST_CHECK(to_date_time_ms<DateTimeStruct>(sec_to_ms(sample_ts)).mon == 6);
    std::tm tm_info{};
    tm_info.tm_year = 124;
    tm_info.tm_mon = 5;
    tm_info.tm_mday = 30;
    tm_info.tm_hour = 12;
    tm_info.tm_min = 34;
    tm_info.tm_sec = 56;
    tm_info.tm_isdst = -1;
    TIME_SHIELD_TEST_CHECK(tm_to_timestamp(&tm_info) == to_timestamp(2024, 6, 30, 12, 34, 56));
    TIME_SHIELD_TEST_CHECK(tm_to_ts(&tm_info) == tm_to_timestamp(&tm_info));
    TIME_SHIELD_TEST_CHECK(tm_to_timestamp_ms(&tm_info) == sec_to_ms(to_timestamp(2024, 6, 30, 12, 34, 56)));
    TIME_SHIELD_TEST_CHECK(tm_to_ts_ms(&tm_info) == tm_to_timestamp_ms(&tm_info));
    TIME_SHIELD_TEST_CHECK(tm_to_ftimestamp(&tm_info) == static_cast<fts_t>(to_timestamp(2024, 6, 30, 12, 34, 56)));
    TIME_SHIELD_TEST_CHECK(tm_to_fts(&tm_info) == tm_to_ftimestamp(&tm_info));
    TIME_SHIELD_TEST_CHECK(year_of<>(sample_ts) == 2024);
    TIME_SHIELD_TEST_CHECK(year_of_ms<>(sec_to_ms(sample_ts)) == 2024);
    TIME_SHIELD_TEST_CHECK(num_days_in_year<>(2024) == DAYS_PER_LEAP_YEAR);
    TIME_SHIELD_TEST_CHECK(num_days_in_year<>(2023) == DAYS_PER_YEAR);
    TIME_SHIELD_TEST_CHECK(num_days_in_year_ts(sample_ts) == DAYS_PER_LEAP_YEAR);
    TIME_SHIELD_TEST_CHECK(start_of_year(sample_ts) == to_timestamp(2024, 1, 1));
    TIME_SHIELD_TEST_CHECK(start_of_year_ms(sec_to_ms(sample_ts)) == sec_to_ms(to_timestamp(2024, 1, 1)));
    TIME_SHIELD_TEST_CHECK(start_of_year_date(2024) == to_timestamp(2024, 1, 1));
    TIME_SHIELD_TEST_CHECK(start_of_year_date_ms(2024) == sec_to_ms(to_timestamp(2024, 1, 1)));
    TIME_SHIELD_TEST_CHECK(end_of_year(sample_ts) == to_timestamp(2024, 12, 31, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(end_of_year_ms(sec_to_ms(sample_ts)) == sec_to_ms(to_timestamp(2024, 12, 31, 23, 59, 59)) + (MS_PER_SEC - 1));
    {
        const ts_ms_t t = -1;
        const ts_ms_t y0 = start_of_year_ms(t);
        const ts_ms_t y1 = end_of_year_ms(t);
    
        // Calendar invariants for ms boundaries:
        TIME_SHIELD_TEST_CHECK(y0 <= t && t <= y1);
        TIME_SHIELD_TEST_CHECK(ms_part(y0) == 0);
        TIME_SHIELD_TEST_CHECK(ms_part(y1) == (MS_PER_SEC - 1));
    }
    const ts_t before_epoch = to_timestamp(1969, 12, 31, 23, 59, 59);
    TIME_SHIELD_TEST_CHECK(start_of_year(before_epoch) == to_timestamp(1969, 1, 1));
    TIME_SHIELD_TEST_CHECK(end_of_year(before_epoch) == to_timestamp(1969, 12, 31, 23, 59, 59));
    const ts_t epoch_start = to_timestamp(1970, 1, 1, 0, 0, 0);
    TIME_SHIELD_TEST_CHECK(start_of_year(epoch_start) == to_timestamp(1970, 1, 1));
    TIME_SHIELD_TEST_CHECK(end_of_year(epoch_start) == to_timestamp(1970, 12, 31, 23, 59, 59));
    const ts_t leap_day_2000 = to_timestamp(2000, 2, 29);
    TIME_SHIELD_TEST_CHECK(start_of_year(leap_day_2000) == to_timestamp(2000, 1, 1));
    TIME_SHIELD_TEST_CHECK(end_of_year(leap_day_2000) == to_timestamp(2000, 12, 31, 23, 59, 59));
    const ts_t march_1900 = to_timestamp(1900, 3, 1);
    TIME_SHIELD_TEST_CHECK(start_of_year(march_1900) == to_timestamp(1900, 1, 1));
    TIME_SHIELD_TEST_CHECK(end_of_year(march_1900) == to_timestamp(1900, 12, 31, 23, 59, 59));
    const ts_t march_2100 = to_timestamp(2100, 3, 1);
    TIME_SHIELD_TEST_CHECK(start_of_year(march_2100) == to_timestamp(2100, 1, 1));
    TIME_SHIELD_TEST_CHECK(end_of_year(march_2100) == to_timestamp(2100, 12, 31, 23, 59, 59));
    const ts_t min_ts = MIN_TIMESTAMP;
    const ts_t max_ts = MAX_TIMESTAMP;
    const ts_t min_year_start = start_of_year(min_ts);
    const ts_t min_year_end = end_of_year(min_ts);
    TIME_SHIELD_TEST_CHECK(min_year_start <= min_ts);
    TIME_SHIELD_TEST_CHECK(min_year_end >= min_ts);
    TIME_SHIELD_TEST_CHECK(start_of_year(min_year_end) == min_year_start);
    TIME_SHIELD_TEST_CHECK(end_of_year(min_year_start) == min_year_end);
    const ts_t max_year_start = start_of_year(max_ts);
    const ts_t max_year_end = end_of_year(max_ts);
    TIME_SHIELD_TEST_CHECK(max_year_start <= max_ts);
    TIME_SHIELD_TEST_CHECK(max_year_end >= max_ts);
    TIME_SHIELD_TEST_CHECK(year_of(max_year_start) == year_of(max_ts));
    TIME_SHIELD_TEST_CHECK(is_valid_date(MAX_YEAR, 1, 1));
    TIME_SHIELD_TEST_CHECK(is_valid_date(MIN_YEAR, 1, 1));
    TIME_SHIELD_TEST_CHECK(year_of(max_ts) <= MAX_YEAR);
    TIME_SHIELD_TEST_CHECK(year_of(min_ts) >= MIN_YEAR);

    TIME_SHIELD_TEST_CHECK(day_of_week_date<>(2024, 6, 30) == SUN);
    TIME_SHIELD_TEST_CHECK(weekday_of_date<>(sample_date) == SUN);
    TIME_SHIELD_TEST_CHECK(weekday_from_date<>(sample_date) == SUN);
    TIME_SHIELD_TEST_CHECK(get_weekday_from_date<>(sample_date) == SUN);
    TIME_SHIELD_TEST_CHECK(wd<>(sample_date) == SUN);

    // date_time_conversions and aliases
    DateTimeStruct dt{2024, 6, 30, 12, 34, 56, 0};
    struct CustomTz { int hour; int min; bool is_positive; };
    const ts_t dt_ts = to_timestamp(dt);
    TIME_SHIELD_TEST_CHECK(dt_ts == to_timestamp(2024, 6, 30, 12, 34, 56));
    TIME_SHIELD_TEST_CHECK(dt_ts == dt_to_timestamp(dt));
    TIME_SHIELD_TEST_CHECK(dt_to_ts(dt) == dt_to_timestamp(dt));
    TIME_SHIELD_TEST_CHECK(to_timestamp_ms(2024, 6, 30, 12, 34, 56, 5) == sec_to_ms(dt_ts) + 5);
    TIME_SHIELD_TEST_CHECK(dt_to_timestamp_ms(dt) == sec_to_ms(dt_ts));
    TIME_SHIELD_TEST_CHECK(dt_to_ts_ms(dt) == dt_to_timestamp_ms(dt));
    TIME_SHIELD_TEST_CHECK(to_ts_ms(2024, 6, 30, 12, 34, 56, 5) == to_timestamp_ms(2024, 6, 30, 12, 34, 56, 5));

    TIME_SHIELD_TEST_CHECK(to_ftimestamp(2024, 6, 30, 12, 34, 56) == static_cast<fts_t>(dt_ts));
    TIME_SHIELD_TEST_CHECK(dt_to_ftimestamp(dt) == static_cast<fts_t>(dt_ts));
    TIME_SHIELD_TEST_CHECK(to_fts(2024, 6, 30, 12, 34, 56, 5) == to_ftimestamp(2024, 6, 30, 12, 34, 56, 5));
    TIME_SHIELD_TEST_CHECK(dt_to_fts(dt) == dt_to_ftimestamp(dt));

    TIME_SHIELD_TEST_CHECK(hour24_to_12(0) == 12);
    TIME_SHIELD_TEST_CHECK(h24_to_h12(13) == 12);

    const auto alias_dt = to_dt(dt_ts);
    TIME_SHIELD_TEST_CHECK(alias_dt.year == dt.year && alias_dt.mon == dt.mon && alias_dt.sec == dt.sec);
    const auto alias_dt_ms = to_dt_ms(sec_to_ms(dt_ts));
    TIME_SHIELD_TEST_CHECK(alias_dt_ms.year == dt.year && alias_dt_ms.mon == dt.mon && alias_dt_ms.sec == dt.sec);

    const ts_t day_start = start_of_day(dt_ts);
    TIME_SHIELD_TEST_CHECK(day_start == to_timestamp(2024, 6, 30));
    TIME_SHIELD_TEST_CHECK(start_of_prev_day(day_start) == to_timestamp(2024, 6, 29));
    TIME_SHIELD_TEST_CHECK(start_of_day_sec(sec_to_ms(day_start)) == day_start);
    TIME_SHIELD_TEST_CHECK(start_of_day_ms(sec_to_ms(day_start)) == sec_to_ms(day_start));
    TIME_SHIELD_TEST_CHECK(start_of_next_day(day_start) == to_timestamp(2024, 7, 1));
    TIME_SHIELD_TEST_CHECK(start_of_next_day_ms(sec_to_ms(day_start)) == sec_to_ms(to_timestamp(2024, 7, 1)));
    TIME_SHIELD_TEST_CHECK(next_day(day_start, 2) == to_timestamp(2024, 7, 2));
    TIME_SHIELD_TEST_CHECK(next_day_ms(sec_to_ms(day_start), 2) == sec_to_ms(to_timestamp(2024, 7, 2)));
    TIME_SHIELD_TEST_CHECK(end_of_day(day_start) == to_timestamp(2024, 6, 30, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(end_of_day_sec(sec_to_ms(day_start)) == to_timestamp(2024, 6, 30, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(end_of_day_ms(sec_to_ms(day_start)) == sec_to_ms(to_timestamp(2024, 6, 30, 23, 59, 59)) + 999);
    {
        const ts_ms_t t = sec_to_ms(day_start);
        const ts_ms_t d0 = start_of_day_ms(t);
        const ts_ms_t d1 = end_of_day_ms(t);
        TIME_SHIELD_TEST_CHECK(d0 == t);
        TIME_SHIELD_TEST_CHECK(d1 == t + MS_PER_DAY - 1);
    }

    TIME_SHIELD_TEST_CHECK(day_of_year(day_start) == 182);
    TIME_SHIELD_TEST_CHECK(month_of_year<int>(day_start) == 6);
    TIME_SHIELD_TEST_CHECK(day_of_month<int>(day_start) == 30);
    TIME_SHIELD_TEST_CHECK(num_days_in_month(2024, 2) == 29);
    TIME_SHIELD_TEST_CHECK(num_days_in_month(2023, 2) == 28);
    TIME_SHIELD_TEST_CHECK(num_days_in_month_ts(sample_ts) == 30);

    TIME_SHIELD_TEST_CHECK(weekday_of_ts(day_start) == SUN);
    TIME_SHIELD_TEST_CHECK(get_weekday_from_ts(day_start) == SUN);
    TIME_SHIELD_TEST_CHECK(weekday_of_ts_ms(sec_to_ms(day_start)) == SUN);
    TIME_SHIELD_TEST_CHECK(get_weekday_from_ts_ms(sec_to_ms(day_start)) == SUN);
    TIME_SHIELD_TEST_CHECK(wd_ts(day_start) == SUN);
    TIME_SHIELD_TEST_CHECK(wd_ms(sec_to_ms(day_start)) == SUN);

    TIME_SHIELD_TEST_CHECK(start_of_month(day_start) == to_timestamp(2024, 6, 1));
    TIME_SHIELD_TEST_CHECK(end_of_month(day_start) == to_timestamp(2024, 6, 30, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(last_sunday_of_month(day_start) == to_timestamp(2024, 6, 30, 0, 0, 0));
    TIME_SHIELD_TEST_CHECK(last_sunday_of_month(to_timestamp(2024, 6, 15, 12, 0, 0)) == to_timestamp(2024, 6, 30, 0, 0, 0));
    TIME_SHIELD_TEST_CHECK(last_sunday_month_day<>(2024, 6) == 30);

    TIME_SHIELD_TEST_CHECK(start_of_week(day_start) == to_timestamp(2024, 6, 30));
    TIME_SHIELD_TEST_CHECK(end_of_week(day_start) == to_timestamp(2024, 7, 6, 23, 59, 59));
    TIME_SHIELD_TEST_CHECK(start_of_saturday(day_start) == to_timestamp(2024, 7, 6));

    TIME_SHIELD_TEST_CHECK(start_of_hour(dt_ts) == to_timestamp(2024, 6, 30, 12, 0, 0));
    TIME_SHIELD_TEST_CHECK(start_of_hour_sec(sec_to_ms(dt_ts)) == to_timestamp(2024, 6, 30, 12, 0, 0));
    TIME_SHIELD_TEST_CHECK(start_of_hour_ms(sec_to_ms(dt_ts)) == sec_to_ms(to_timestamp(2024, 6, 30, 12, 0, 0)));
    TIME_SHIELD_TEST_CHECK(end_of_hour(dt_ts) == to_timestamp(2024, 6, 30, 12, 59, 59));
    TIME_SHIELD_TEST_CHECK(end_of_hour_sec(sec_to_ms(dt_ts)) == to_timestamp(2024, 6, 30, 12, 59, 59));
    TIME_SHIELD_TEST_CHECK(end_of_hour_ms(sec_to_ms(dt_ts)) == sec_to_ms(to_timestamp(2024, 6, 30, 12, 59, 59)) + 999);
    TIME_SHIELD_TEST_CHECK(start_of_min(dt_ts) == to_timestamp(2024, 6, 30, 12, 34, 0));
    TIME_SHIELD_TEST_CHECK(end_of_min(dt_ts) == to_timestamp(2024, 6, 30, 12, 34, 59));
    TIME_SHIELD_TEST_CHECK(min_of_day(dt_ts) == 754);
    TIME_SHIELD_TEST_CHECK(hour_of_day(dt_ts) == 12);
    TIME_SHIELD_TEST_CHECK(min_of_hour(dt_ts) == 34);
    TIME_SHIELD_TEST_CHECK(start_of_period(1, dt_ts) == dt_ts);
    TIME_SHIELD_TEST_CHECK(start_of_period(300, dt_ts) == to_timestamp(2024, 6, 30, 12, 30, 0));
    TIME_SHIELD_TEST_CHECK(start_of_period(0, dt_ts) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(start_of_period(-300, dt_ts) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(end_of_period(1, dt_ts) == dt_ts);
    TIME_SHIELD_TEST_CHECK(end_of_period(300, dt_ts) == to_timestamp(2024, 6, 30, 12, 34, 59));
    TIME_SHIELD_TEST_CHECK(end_of_period(0, dt_ts) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(end_of_period(-300, dt_ts) == ERROR_TIMESTAMP);

    const TimeZoneStruct tz_struct{3, 30, true};
    TIME_SHIELD_TEST_CHECK(time_zone_struct_to_offset(tz_struct) == SEC_PER_HOUR * 3 + SEC_PER_MIN * 30);
    TIME_SHIELD_TEST_CHECK(tz_to_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    TIME_SHIELD_TEST_CHECK(to_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    TIME_SHIELD_TEST_CHECK(to_tz_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    TIME_SHIELD_TEST_CHECK(tz_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    TIME_SHIELD_TEST_CHECK(tz_offset_hm(3, 30) == time_zone_struct_to_offset(tz_struct));
    TIME_SHIELD_TEST_CHECK(tz_offset_hm(-5, -30) == -(SEC_PER_HOUR * 5 + SEC_PER_MIN * 30));
    TIME_SHIELD_TEST_CHECK(is_valid_tz_offset(tz_offset_hm(3, 30)));
    TIME_SHIELD_TEST_CHECK(valid_tz_offset(tz_offset_hm(3, 30)));
    const CustomTz custom_tz = to_time_zone<CustomTz>(time_zone_struct_to_offset(tz_struct));
    TIME_SHIELD_TEST_CHECK(custom_tz.hour == 3 && custom_tz.min == 30 && custom_tz.is_positive);

    // workday_conversions
    TIME_SHIELD_TEST_CHECK(first_workday_day(2024, 6) == 3);
    TIME_SHIELD_TEST_CHECK(last_workday_day(2024, 6) == 28);
    TIME_SHIELD_TEST_CHECK(count_workdays_in_month(2024, 6) == 20);
    TIME_SHIELD_TEST_CHECK(workday_index_in_month(2024, 6, 3) == 1);
    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month(2024, 6, 3));
    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month(2024, 6, 4));
    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month(2024, 6, 5, 5));
    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month(2024, 6, 28));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(2024, 6, 24, 5));

    const ts_t workday_ts = to_timestamp(2024, 6, 3);
    const ts_ms_t workday_ts_ms = sec_to_ms(workday_ts);
    TIME_SHIELD_TEST_CHECK(is_workday(workday_ts));
    TIME_SHIELD_TEST_CHECK(is_workday_ms(workday_ts_ms));
    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month(workday_ts));
    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month_ms(workday_ts_ms));
    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month(workday_ts, 3));
    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month_ms(workday_ts_ms, 3));
    TIME_SHIELD_TEST_CHECK(!is_last_workday_of_month(workday_ts));
    TIME_SHIELD_TEST_CHECK(!is_last_workday_of_month_ms(workday_ts_ms));
    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month(to_timestamp(2024, 6, 28)));
    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month_ms(sec_to_ms(to_timestamp(2024, 6, 28))));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(to_timestamp(2024, 6, 24), 5));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month_ms(sec_to_ms(to_timestamp(2024, 6, 24)), 5));

    TIME_SHIELD_TEST_CHECK(start_of_first_workday_month(2024, 6) == workday_ts);
    TIME_SHIELD_TEST_CHECK(start_of_first_workday_month_ms(2024, 6) == workday_ts_ms);
    TIME_SHIELD_TEST_CHECK(start_of_first_workday_month(workday_ts) == workday_ts);
    TIME_SHIELD_TEST_CHECK(start_of_first_workday_month_ms(workday_ts_ms) == workday_ts_ms);
    TIME_SHIELD_TEST_CHECK(end_of_first_workday_month(2024, 6) == end_of_day(workday_ts));
    TIME_SHIELD_TEST_CHECK(end_of_first_workday_month_ms(2024, 6) == end_of_day_ms(workday_ts_ms));
    TIME_SHIELD_TEST_CHECK(end_of_first_workday_month(workday_ts) == end_of_day(workday_ts));
    TIME_SHIELD_TEST_CHECK(end_of_first_workday_month_ms(workday_ts_ms) == end_of_day_ms(workday_ts_ms));

    const ts_t last_workday_ts = to_timestamp(2024, 6, 28);
    const ts_ms_t last_workday_ts_ms = sec_to_ms(last_workday_ts);
    TIME_SHIELD_TEST_CHECK(start_of_last_workday_month(2024, 6) == last_workday_ts);
    TIME_SHIELD_TEST_CHECK(start_of_last_workday_month_ms(2024, 6) == last_workday_ts_ms);
    TIME_SHIELD_TEST_CHECK(start_of_last_workday_month(last_workday_ts) == last_workday_ts);
    TIME_SHIELD_TEST_CHECK(start_of_last_workday_month_ms(last_workday_ts_ms) == last_workday_ts_ms);
    TIME_SHIELD_TEST_CHECK(end_of_last_workday_month(2024, 6) == end_of_day(last_workday_ts));
    TIME_SHIELD_TEST_CHECK(end_of_last_workday_month_ms(2024, 6) == end_of_day_ms(last_workday_ts_ms));
    TIME_SHIELD_TEST_CHECK(end_of_last_workday_month(last_workday_ts) == end_of_day(last_workday_ts));
    TIME_SHIELD_TEST_CHECK(end_of_last_workday_month_ms(last_workday_ts_ms) == end_of_day_ms(last_workday_ts_ms));

    (void)unix_day_two_ts;
    (void)unix_day_two_ms;
    (void)minute_mark;
    (void)sample_ts;
    (void)day_start;
    (void)workday_ts;
    (void)workday_ts_ms;
    (void)last_workday_ts;
    (void)last_workday_ts_ms;

    return 0;
}
