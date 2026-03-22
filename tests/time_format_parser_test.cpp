#include <time_shield/time_formatting.hpp>
#include <time_shield/time_format_parser.hpp>
#include <time_shield/time_parser.hpp>

#include <cassert>
#include <string>

/// \brief Coverage for formatter-compatible parsing helpers.
int main() {
    using namespace time_shield;

    {
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        const bool is_ok = try_parse_format(
            "2024-07-08 12:34:56",
            std::string("%Y-%m-%d %H:%M:%S"),
            dt,
            tz);
        assert(is_ok);
        assert(dt.year == 2024);
        assert(dt.mon == 7);
        assert(dt.day == 8);
        assert(dt.hour == 12);
        assert(dt.min == 34);
        assert(dt.sec == 56);
        assert(time_zone_struct_to_offset(tz) == 0);
        (void)is_ok;
        (void)dt;
        (void)tz;
    }

    {
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        assert(try_parse_format(std::string("2025-51-2"), std::string("%G-%V-%u"), dt, tz));
        assert(dt.year == 2025);
        assert(dt.mon == 12);
        assert(dt.day == 16);
        assert(time_zone_struct_to_offset(tz) == 0);
    }

    {
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        assert(try_parse_format(std::string("2025-W51"), std::string("%G-W%V"), dt, tz));
        assert(dt.year == 2025);
        assert(dt.mon == 12);
        assert(dt.day == 15);
        assert(dt.hour == 0);
        assert(dt.min == 0);
        assert(dt.sec == 0);
    }

    {
        ts_t ts_value = 0;
        assert(try_parse_format_ts(
            std::string("2024-01-02T04:04:05+0100"),
            std::string("%Y-%m-%dT%H:%M:%S%z"),
            ts_value));
        assert(ts_value == to_timestamp(2024, 1, 2, 3, 4, 5));
        (void)ts_value;
    }

    {
        const ts_t utc_ts = to_timestamp(2024, 1, 2, 3, 4, 5);
        const std::string text = to_iso8601(utc_ts, SEC_PER_HOUR);
        ts_t parsed = 0;
        assert(try_parse_format_ts(text, std::string("%Y-%m-%dT%H:%M:%S%z"), parsed));
        assert(parsed == utc_ts);
        (void)parsed;
    }

    {
        ts_t ts_value = 0;
        assert(try_parse_format_ts(
            std::string("2024-03-20T07:04:05-05:30"),
            std::string("%Y-%m-%dT%H:%M:%S%z"),
            ts_value));
        assert(ts_value == to_timestamp(2024, 3, 20, 12, 34, 5));
        (void)ts_value;
    }

    {
        const ts_ms_t utc_ms = ts_ms(2024, 3, 20, 12, 34, 56, 789);
        const std::string text = to_string_ms("%a %b %e %H:%M:%S %Y", utc_ms, 0);
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        assert(try_parse_format(text, std::string("%c"), dt, tz));
        assert(dt.year == 2024);
        assert(dt.mon == 3);
        assert(dt.day == 20);
        assert(dt.hour == 12);
        assert(dt.min == 34);
        assert(dt.sec == 56);
        (void)dt;
        (void)tz;
    }

    {
        const ts_ms_t utc_ms = ts_ms(2024, 3, 20, 12, 34, 56, 789);
        const std::string text = to_string_ms("%Y-%m-%d %H:%M:%S.%sss %z", utc_ms, -(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN));
        ts_ms_t parsed = 0;
        assert(try_parse_format_ts_ms(text, std::string("%Y-%m-%d %H:%M:%S.%sss %z"), parsed));
        assert(parsed == utc_ms);
        (void)parsed;
    }

    {
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        assert(!try_parse_format(std::string("2024-07-08X12:34:56"), std::string("%Y-%m-%d %H:%M:%S"), dt, tz));
        assert(!try_parse_format(std::string("2024-02-30 12:34:56"), std::string("%Y-%m-%d %H:%M:%S"), dt, tz));
        assert(!try_parse_format(std::string("2024-07-08 12:34:56 +01"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        assert(!try_parse_format(std::string("2024-07-08 12:34:56 +010"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        assert(!try_parse_format(std::string("2024-07-08 12:34:56 +01:0"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        assert(!try_parse_format(std::string("2024-07-08 12:34:56 +01:000"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        assert(!try_parse_format(std::string("2024-07-08 12:34:56 +2500"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        assert(!try_parse_format(std::string("2024-07-08 12:34:56 +25:00"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        assert(!try_parse_format(std::string("2025-12-16 2025-51-2"), std::string("%Y-%m-%d %G-%V-%u"), dt, tz));
        assert(!try_parse_format(std::string("2025-00-1"), std::string("%G-%V-%u"), dt, tz));
        assert(!try_parse_format(std::string("2025-54-1"), std::string("%G-%V-%u"), dt, tz));
        assert(!try_parse_format(std::string("2025-51-0"), std::string("%G-%V-%u"), dt, tz));
        assert(!try_parse_format(std::string("2025-51-8"), std::string("%G-%V-%u"), dt, tz));
        (void)dt;
        (void)tz;
    }

    return 0;
}
