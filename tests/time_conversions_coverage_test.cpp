#include <time_shield/time_conversions.hpp>

#include <cassert>
#include <cmath>
#include <ctime>

int main() {
    using namespace time_shield;

    // time_unit_conversions
    assert(ns_of_sec(1.25) == 250000000);
    assert(us_of_sec(1.5) == 500000);
    assert(ms_of_sec(2.5) == 500);
    assert(ms_of_ts(1234) == 234);
    assert(sec_to_ms<>(2) == 2000);
    assert(sec_to_ms<int>(3.5) == 3500);
    assert(fsec_to_ms(1.1) == 1100);
    assert(ms_to_sec<>(1500) == 1);
    assert(ms_to_fsec(2500) == 2.5);
    assert(min_to_ms<>(2) == 120000);
    assert(min_to_ms<int>(1.5) == 90000);
    assert(ms_to_min<>(60000) == 1);
    assert(min_to_sec<>(1.5) == 90);
    assert(sec_to_min<>(180) == 3);
    assert(min_to_fsec(2) == static_cast<fts_t>(SEC_PER_MIN * 2));
    assert(sec_to_fmin(180) == 3.0);
    assert(hour_to_ms<>(1) == MS_PER_HOUR);
    assert(ms_to_hour<>(MS_PER_HOUR) == 1);
    assert(hour_to_fsec(1) == static_cast<fts_t>(SEC_PER_HOUR));
    assert(sec_to_fhour(7200) == 2.0);
    assert(sec_to_hour<>(7200) == 2);
    assert(hour_to_sec<>(1) == 3600);
    assert(sec_to_hour<>(5400) == 1);

    // unix_time_conversions and aliases
    const ts_t unix_day_two_ts = unix_day_to_ts(2);
    assert(unix_day_to_timestamp(2) == unix_day_two_ts);
    assert(unixday_to_ts(2) == unix_day_two_ts);
    assert(uday_to_ts(2) == unix_day_two_ts);
    assert(start_of_day_from_unix_day(2) == unix_day_two_ts);

    const ts_ms_t unix_day_two_ms = unix_day_to_ts_ms(2);
    assert(unix_day_to_timestamp_ms(2) == unix_day_two_ms);
    assert(unixday_to_ts_ms(2) == unix_day_two_ms);
    assert(uday_to_ts_ms(2) == unix_day_two_ms);
    assert(start_of_day_from_unix_day_ms(2) == unix_day_two_ms);

    assert(end_of_day_from_unix_day(0) == SEC_PER_DAY - 1);
    assert(end_of_day_from_unix_day_ms(0) == MS_PER_DAY - 1);
    assert(start_of_next_day_from_unix_day(0) == SEC_PER_DAY);
    assert(next_day_from_unix_day(0) == SEC_PER_DAY);
    assert(next_day_unix_day(0) == SEC_PER_DAY);
    assert(next_day_unixday(0) == SEC_PER_DAY);

    assert(start_of_next_day_from_unix_day_ms(0) == MS_PER_DAY);
    assert(next_day_from_unix_day_ms(0) == MS_PER_DAY);
    assert(next_day_unix_day_ms(0) == MS_PER_DAY);
    assert(next_day_unixday_ms(0) == MS_PER_DAY);

    assert(days_since_epoch(SEC_PER_DAY) == 1);
    assert(get_unixday(SEC_PER_DAY) == 1);
    assert(unix_day(SEC_PER_DAY) == 1);
    assert(unixday(SEC_PER_DAY) == 1);
    assert(uday(SEC_PER_DAY) == 1);
    assert(get_unix_day(SEC_PER_DAY) == 1);

    assert(days_since_epoch_ms(MS_PER_DAY) == 1);
    assert(get_unixday_ms(MS_PER_DAY) == 1);
    assert(unix_day_ms(MS_PER_DAY) == 1);
    assert(unixday_ms(MS_PER_DAY) == 1);
    assert(uday_ms(MS_PER_DAY) == 1);
    assert(get_unix_day_ms(MS_PER_DAY) == 1);

    assert(days_between(0, SEC_PER_DAY * 3) == 3);

    assert(date_to_unix_day(1970, 1, 1) == 0);
    assert(years_since_epoch(static_cast<ts_t>(SEC_PER_YEAR)) == 1);

    const ts_t minute_mark = SEC_PER_MIN * 5;
    assert(min_since_epoch<>(minute_mark) == 5);
    assert(minutes_since_epoch(minute_mark) == 5);
    assert(unix_min(minute_mark) == 5);
    assert(to_unix_min(minute_mark) == 5);
    assert(umin(minute_mark) == 5);
    assert(get_unix_min(minute_mark) == 5);

    assert(sec_of_day(SEC_PER_DAY + 10) == 10);
    assert(sec_of_day_ms(MS_PER_DAY + 2000) == 2);
    assert((sec_of_day<int, int>(1, 1, 1) == SEC_PER_HOUR + SEC_PER_MIN + 1));
    assert(sec_of_min(SEC_PER_MIN + 7) == 7);
    assert(sec_of_hour(SEC_PER_HOUR + 15) == 15);

    // date_conversions and aliases
    const ts_t sample_ts = to_timestamp(2024, 6, 30, 12, 0, 0);
    DateStruct sample_date{2024, 6, 30};
    assert(to_date_time(sample_ts).year == 2024);
    assert(to_date_time_ms<DateTimeStruct>(sec_to_ms(sample_ts)).mon == 6);
    std::tm tm_info{};
    tm_info.tm_year = 124;
    tm_info.tm_mon = 5;
    tm_info.tm_mday = 30;
    tm_info.tm_hour = 12;
    tm_info.tm_min = 34;
    tm_info.tm_sec = 56;
    tm_info.tm_isdst = -1;
    assert(tm_to_timestamp(&tm_info) == to_timestamp(2024, 6, 30, 12, 34, 56));
    assert(tm_to_ts(&tm_info) == tm_to_timestamp(&tm_info));
    assert(tm_to_timestamp_ms(&tm_info) == sec_to_ms(to_timestamp(2024, 6, 30, 12, 34, 56)));
    assert(tm_to_ts_ms(&tm_info) == tm_to_timestamp_ms(&tm_info));
    assert(tm_to_ftimestamp(&tm_info) == static_cast<fts_t>(to_timestamp(2024, 6, 30, 12, 34, 56)));
    assert(tm_to_fts(&tm_info) == tm_to_ftimestamp(&tm_info));
    assert(year_of<>(sample_ts) == 2024);
    assert(year_of_ms<>(sec_to_ms(sample_ts)) == 2024);
    assert(num_days_in_year<>(2024) == DAYS_PER_LEAP_YEAR);
    assert(num_days_in_year<>(2023) == DAYS_PER_YEAR);
    assert(num_days_in_year_ts(sample_ts) == DAYS_PER_LEAP_YEAR);
    assert(start_of_year(sample_ts) == to_timestamp(2024, 1, 1));
    assert(start_of_year_ms(sec_to_ms(sample_ts)) == sec_to_ms(to_timestamp(2024, 1, 1)));
    assert(start_of_year_date(2024) == to_timestamp(2024, 1, 1));
    assert(start_of_year_date_ms(2024) == sec_to_ms(to_timestamp(2024, 1, 1)));
    assert(end_of_year(sample_ts) == to_timestamp(2024, 12, 31, 23, 59, 59));
    assert(end_of_year_ms(sec_to_ms(sample_ts)) == sec_to_ms(to_timestamp(2024, 12, 31, 23, 59, 59)));
    assert(day_of_week_date<>(2024, 6, 30) == SUN);
    assert(weekday_of_date<>(sample_date) == SUN);
    assert(weekday_from_date<>(sample_date) == SUN);
    assert(get_weekday_from_date<>(sample_date) == SUN);
    assert(wd<>(sample_date) == SUN);

    // date_time_conversions and aliases
    DateTimeStruct dt{2024, 6, 30, 12, 34, 56, 0};
    struct CustomTz { int hour; int min; bool is_positive; };
    const ts_t dt_ts = to_timestamp(dt);
    assert(dt_ts == to_timestamp(2024, 6, 30, 12, 34, 56));
    assert(dt_ts == dt_to_timestamp(dt));
    assert(dt_to_ts(dt) == dt_to_timestamp(dt));
    assert(to_timestamp_ms(2024, 6, 30, 12, 34, 56, 5) == sec_to_ms(dt_ts) + 5);
    assert(dt_to_timestamp_ms(dt) == sec_to_ms(dt_ts));
    assert(dt_to_ts_ms(dt) == dt_to_timestamp_ms(dt));
    assert(to_ts_ms(2024, 6, 30, 12, 34, 56, 5) == to_timestamp_ms(2024, 6, 30, 12, 34, 56, 5));

    assert(to_ftimestamp(2024, 6, 30, 12, 34, 56) == static_cast<fts_t>(dt_ts));
    assert(dt_to_ftimestamp(dt) == static_cast<fts_t>(dt_ts));
    assert(to_fts(2024, 6, 30, 12, 34, 56, 5) == to_ftimestamp(2024, 6, 30, 12, 34, 56, 5));
    assert(dt_to_fts(dt) == dt_to_ftimestamp(dt));

    assert(hour24_to_12(0) == 12);
    assert(h24_to_h12(13) == 12);

    const auto alias_dt = to_dt(dt_ts);
    assert(alias_dt.year == dt.year && alias_dt.mon == dt.mon && alias_dt.sec == dt.sec);
    const auto alias_dt_ms = to_dt_ms(sec_to_ms(dt_ts));
    assert(alias_dt_ms.year == dt.year && alias_dt_ms.mon == dt.mon && alias_dt_ms.sec == dt.sec);

    const ts_t day_start = start_of_day(dt_ts);
    assert(day_start == to_timestamp(2024, 6, 30));
    assert(start_of_prev_day(day_start) == to_timestamp(2024, 6, 29));
    assert(start_of_day_sec(sec_to_ms(day_start)) == day_start);
    assert(start_of_day_ms(sec_to_ms(day_start)) == sec_to_ms(day_start));
    assert(start_of_next_day(day_start) == to_timestamp(2024, 7, 1));
    assert(start_of_next_day_ms(sec_to_ms(day_start)) == sec_to_ms(to_timestamp(2024, 7, 1)));
    assert(next_day(day_start, 2) == to_timestamp(2024, 7, 2));
    assert(next_day_ms(sec_to_ms(day_start), 2) == sec_to_ms(to_timestamp(2024, 7, 2)));
    assert(end_of_day(day_start) == to_timestamp(2024, 6, 30, 23, 59, 59));
    assert(end_of_day_sec(sec_to_ms(day_start)) == to_timestamp(2024, 6, 30, 23, 59, 59));
    assert(end_of_day_ms(sec_to_ms(day_start)) == sec_to_ms(to_timestamp(2024, 6, 30, 23, 59, 59)) + 999);

    assert(day_of_year(day_start) == 182);
    assert(month_of_year<int>(day_start) == 6);
    assert(day_of_month<int>(day_start) == 30);
    assert(num_days_in_month(2024, 2) == 29);
    assert(num_days_in_month(2023, 2) == 28);
    assert(num_days_in_month_ts(sample_ts) == 30);

    assert(weekday_of_ts(day_start) == SUN);
    assert(get_weekday_from_ts(day_start) == SUN);
    assert(weekday_of_ts_ms(sec_to_ms(day_start)) == SUN);
    assert(get_weekday_from_ts_ms(sec_to_ms(day_start)) == SUN);
    assert(wd_ts(day_start) == SUN);
    assert(wd_ms(sec_to_ms(day_start)) == SUN);

    assert(start_of_month(day_start) == to_timestamp(2024, 6, 1));
    assert(end_of_month(day_start) == to_timestamp(2024, 6, 30, 23, 59, 59));
    assert(last_sunday_of_month(day_start) == to_timestamp(2024, 6, 30, 23, 59, 59));
    assert(last_sunday_month_day<>(2024, 6) == 30);

    assert(start_of_week(day_start) == to_timestamp(2024, 6, 30));
    assert(end_of_week(day_start) == to_timestamp(2024, 7, 6, 23, 59, 59));
    assert(start_of_saturday(day_start) == to_timestamp(2024, 7, 6));

    assert(start_of_hour(dt_ts) == to_timestamp(2024, 6, 30, 12, 0, 0));
    assert(start_of_hour_sec(sec_to_ms(dt_ts)) == to_timestamp(2024, 6, 30, 12, 0, 0));
    assert(start_of_hour_ms(sec_to_ms(dt_ts)) == sec_to_ms(to_timestamp(2024, 6, 30, 12, 0, 0)));
    assert(end_of_hour(dt_ts) == to_timestamp(2024, 6, 30, 12, 59, 59));
    assert(end_of_hour_sec(sec_to_ms(dt_ts)) == to_timestamp(2024, 6, 30, 12, 59, 59));
    assert(end_of_hour_ms(sec_to_ms(dt_ts)) == sec_to_ms(to_timestamp(2024, 6, 30, 12, 59, 59)) + 999);
    assert(start_of_min(dt_ts) == to_timestamp(2024, 6, 30, 12, 34, 0));
    assert(end_of_min(dt_ts) == to_timestamp(2024, 6, 30, 12, 34, 59));
    assert(min_of_day(dt_ts) == 754);
    assert(hour_of_day(dt_ts) == 12);
    assert(min_of_hour(dt_ts) == 34);
    assert(start_of_period(300, dt_ts) == to_timestamp(2024, 6, 30, 12, 30, 0));
    assert(end_of_period(300, dt_ts) == to_timestamp(2024, 6, 30, 12, 34, 59));

    const TimeZoneStruct tz_struct{3, 30, true};
    assert(time_zone_struct_to_offset(tz_struct) == SEC_PER_HOUR * 3 + SEC_PER_MIN * 30);
    assert(tz_to_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    assert(to_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    assert(to_tz_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    assert(tz_offset(tz_struct) == time_zone_struct_to_offset(tz_struct));
    assert(tz_offset_hm(3, 30) == time_zone_struct_to_offset(tz_struct));
    assert(tz_offset_hm(-5, -30) == -(SEC_PER_HOUR * 5 + SEC_PER_MIN * 30));
    assert(is_valid_tz_offset(tz_offset_hm(3, 30)));
    assert(valid_tz_offset(tz_offset_hm(3, 30)));
    const CustomTz custom_tz = to_time_zone<CustomTz>(time_zone_struct_to_offset(tz_struct));
    assert(custom_tz.hour == 3 && custom_tz.min == 30 && custom_tz.is_positive);

    // workday_conversions
    assert(first_workday_day(2024, 6) == 3);
    assert(last_workday_day(2024, 6) == 28);
    assert(count_workdays_in_month(2024, 6) == 20);
    assert(workday_index_in_month(2024, 6, 3) == 1);
    assert(is_first_workday_of_month(2024, 6, 3));
    assert(!is_first_workday_of_month(2024, 6, 4));
    assert(is_within_first_workdays_of_month(2024, 6, 5, 5));
    assert(is_last_workday_of_month(2024, 6, 28));
    assert(is_within_last_workdays_of_month(2024, 6, 24, 5));

    const ts_t workday_ts = to_timestamp(2024, 6, 3);
    const ts_ms_t workday_ts_ms = sec_to_ms(workday_ts);
    assert(is_workday(workday_ts));
    assert(is_workday_ms(workday_ts_ms));
    assert(is_first_workday_of_month(workday_ts));
    assert(is_first_workday_of_month_ms(workday_ts_ms));
    assert(is_within_first_workdays_of_month(workday_ts, 3));
    assert(is_within_first_workdays_of_month_ms(workday_ts_ms, 3));
    assert(!is_last_workday_of_month(workday_ts));
    assert(!is_last_workday_of_month_ms(workday_ts_ms));
    assert(is_last_workday_of_month(to_timestamp(2024, 6, 28)));
    assert(is_last_workday_of_month_ms(sec_to_ms(to_timestamp(2024, 6, 28))));
    assert(is_within_last_workdays_of_month(to_timestamp(2024, 6, 24), 5));
    assert(is_within_last_workdays_of_month_ms(sec_to_ms(to_timestamp(2024, 6, 24)), 5));

    assert(start_of_first_workday_month(2024, 6) == workday_ts);
    assert(start_of_first_workday_month_ms(2024, 6) == workday_ts_ms);
    assert(start_of_first_workday_month(workday_ts) == workday_ts);
    assert(start_of_first_workday_month_ms(workday_ts_ms) == workday_ts_ms);
    assert(end_of_first_workday_month(2024, 6) == end_of_day(workday_ts));
    assert(end_of_first_workday_month_ms(2024, 6) == end_of_day_ms(workday_ts_ms));
    assert(end_of_first_workday_month(workday_ts) == end_of_day(workday_ts));
    assert(end_of_first_workday_month_ms(workday_ts_ms) == end_of_day_ms(workday_ts_ms));

    const ts_t last_workday_ts = to_timestamp(2024, 6, 28);
    const ts_ms_t last_workday_ts_ms = sec_to_ms(last_workday_ts);
    assert(start_of_last_workday_month(2024, 6) == last_workday_ts);
    assert(start_of_last_workday_month_ms(2024, 6) == last_workday_ts_ms);
    assert(start_of_last_workday_month(last_workday_ts) == last_workday_ts);
    assert(start_of_last_workday_month_ms(last_workday_ts_ms) == last_workday_ts_ms);
    assert(end_of_last_workday_month(2024, 6) == end_of_day(last_workday_ts));
    assert(end_of_last_workday_month_ms(2024, 6) == end_of_day_ms(last_workday_ts_ms));
    assert(end_of_last_workday_month(last_workday_ts) == end_of_day(last_workday_ts));
    assert(end_of_last_workday_month_ms(last_workday_ts_ms) == end_of_day_ms(last_workday_ts_ms));

    return 0;
}
