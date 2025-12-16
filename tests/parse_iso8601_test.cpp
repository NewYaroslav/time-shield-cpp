#include <time_shield/time_parser.hpp>

#include <cassert>
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
        assert(dt.year == year);
        assert(dt.mon == month);
        assert(dt.day == day);
        assert(dt.hour == hour);
        assert(dt.min == minute);
        assert(dt.sec == second);
        assert(dt.ms == millisecond);
    };

    auto check_tz = [](const TimeZoneStruct& tz, bool is_positive, int hour, int minute) {
        assert(tz.is_positive == is_positive);
        assert(tz.hour == hour);
        assert(tz.min == minute);
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
        assert(is_parsed);

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
