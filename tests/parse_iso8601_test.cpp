#include <time_shield/time_parser.hpp>

#include "test_assert.hpp"
#include <string>

int main() {
    using namespace time_shield;

    auto check_dt = [](const DateTimeStruct& dt,
                       int year,
                       int month,
                       int day,
                       int hour,
                       int minute,
                       int second,
                       int millisecond) {
        TIME_SHIELD_TEST_CHECK(dt.year == year);
        TIME_SHIELD_TEST_CHECK(dt.mon == month);
        TIME_SHIELD_TEST_CHECK(dt.day == day);
        TIME_SHIELD_TEST_CHECK(dt.hour == hour);
        TIME_SHIELD_TEST_CHECK(dt.min == minute);
        TIME_SHIELD_TEST_CHECK(dt.sec == second);
        TIME_SHIELD_TEST_CHECK(dt.ms == millisecond);
        (void)dt;
        (void)year;
        (void)month;
        (void)day;
        (void)hour;
        (void)minute;
        (void)second;
        (void)millisecond;
    };

    auto check_tz = [](const TimeZoneStruct& tz, bool is_positive, int hour, int minute) {
        TIME_SHIELD_TEST_CHECK(tz.is_positive == is_positive);
        TIME_SHIELD_TEST_CHECK(tz.hour == hour);
        TIME_SHIELD_TEST_CHECK(tz.min == minute);
        (void)tz;
        (void)is_positive;
        (void)hour;
        (void)minute;
    };

    DateTimeStruct dt{};
    TimeZoneStruct tz{};

    auto parse_and_check = [&](const std::string& input,
                               int year,
                               int month,
                               int day,
                               int hour,
                               int minute,
                               int second,
                               int millisecond,
                               bool is_positive_tz,
                               int tz_hour,
                               int tz_minute) {
        dt = create_date_time_struct(0);
        tz = create_time_zone_struct(0, 0);
        tz.is_positive = true;

        const bool is_parsed = parse_iso8601(input, dt, tz);
        TIME_SHIELD_TEST_CHECK(is_parsed);
        (void)is_parsed;

        check_dt(dt, year, month, day, hour, minute, second, millisecond);
        check_tz(tz, is_positive_tz, tz_hour, tz_minute);
    };

    // Date only.
    parse_and_check("2024-07-08", 2024, 7, 8, 0, 0, 0, 0, true, 0, 0);

    // Date with time to minutes.
    parse_and_check("2024-07-08T12:34", 2024, 7, 8, 12, 34, 0, 0, true, 0, 0);

    // Date/time to seconds.
    parse_and_check("2024-07-08T12:34:56", 2024, 7, 8, 12, 34, 56, 0, true, 0, 0);

    // Date/time with fractional seconds.
    parse_and_check("2024-07-08T12:34:56.789", 2024, 7, 8, 12, 34, 56, 789, true, 0, 0);

    // Zulu timezone.
    parse_and_check("2024-07-08T12:34:56Z", 2024, 7, 8, 12, 34, 56, 0, true, 0, 0);

    // Positive timezone offset.
    parse_and_check("2024-07-08T12:34:56+05:30", 2024, 7, 8, 12, 34, 56, 0, true, 5, 30);

    // Negative timezone offset.
    parse_and_check("2024-07-08T12:34:56-02:15", 2024, 7, 8, 12, 34, 56, 0, false, 2, 15);

    // Whitespace separator and trailing spaces.
    parse_and_check("2024-07-08 12:34:56   ", 2024, 7, 8, 12, 34, 56, 0, true, 0, 0);

    // Alternate date separators.
    parse_and_check("2024/07/08T12:34", 2024, 7, 8, 12, 34, 0, 0, true, 0, 0);
    parse_and_check("2024.07.08T12:34:00-03:00", 2024, 7, 8, 12, 34, 0, 0, false, 3, 0);

    // Leading spaces.
    parse_and_check("   2024-07-08T12:34:56Z", 2024, 7, 8, 12, 34, 56, 0, true, 0, 0);

    return 0;
}
