#include <time_shield/time_formatting.hpp>
#include <time_shield/time_format_parser.hpp>
#include <time_shield/time_parser.hpp>

#include "test_assert.hpp"
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
        TIME_SHIELD_TEST_CHECK(is_ok);
        TIME_SHIELD_TEST_CHECK(dt.year == 2024);
        TIME_SHIELD_TEST_CHECK(dt.mon == 7);
        TIME_SHIELD_TEST_CHECK(dt.day == 8);
        TIME_SHIELD_TEST_CHECK(dt.hour == 12);
        TIME_SHIELD_TEST_CHECK(dt.min == 34);
        TIME_SHIELD_TEST_CHECK(dt.sec == 56);
        TIME_SHIELD_TEST_CHECK(time_zone_struct_to_offset(tz) == 0);
        (void)is_ok;
        (void)dt;
        (void)tz;
    }

    {
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        TIME_SHIELD_TEST_CHECK(try_parse_format(std::string("2025-51-2"), std::string("%G-%V-%u"), dt, tz));
        TIME_SHIELD_TEST_CHECK(dt.year == 2025);
        TIME_SHIELD_TEST_CHECK(dt.mon == 12);
        TIME_SHIELD_TEST_CHECK(dt.day == 16);
        TIME_SHIELD_TEST_CHECK(time_zone_struct_to_offset(tz) == 0);
    }

    {
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        TIME_SHIELD_TEST_CHECK(try_parse_format(std::string("2025-W51"), std::string("%G-W%V"), dt, tz));
        TIME_SHIELD_TEST_CHECK(dt.year == 2025);
        TIME_SHIELD_TEST_CHECK(dt.mon == 12);
        TIME_SHIELD_TEST_CHECK(dt.day == 15);
        TIME_SHIELD_TEST_CHECK(dt.hour == 0);
        TIME_SHIELD_TEST_CHECK(dt.min == 0);
        TIME_SHIELD_TEST_CHECK(dt.sec == 0);
    }

    {
        ts_t ts_value = 0;
        TIME_SHIELD_TEST_CHECK(try_parse_format_ts(
            std::string("2024-01-02T04:04:05+0100"),
            std::string("%Y-%m-%dT%H:%M:%S%z"),
            ts_value));
        TIME_SHIELD_TEST_CHECK(ts_value == to_timestamp(2024, 1, 2, 3, 4, 5));
        (void)ts_value;
    }

    {
        const ts_t utc_ts = to_timestamp(2024, 1, 2, 3, 4, 5);
        const std::string text = to_iso8601(utc_ts, SEC_PER_HOUR);
        ts_t parsed = 0;
        TIME_SHIELD_TEST_CHECK(try_parse_format_ts(text, std::string("%Y-%m-%dT%H:%M:%S%z"), parsed));
        TIME_SHIELD_TEST_CHECK(parsed == utc_ts);
        (void)parsed;
    }

    {
        ts_t ts_value = 0;
        TIME_SHIELD_TEST_CHECK(try_parse_format_ts(
            std::string("2024-03-20T07:04:05-05:30"),
            std::string("%Y-%m-%dT%H:%M:%S%z"),
            ts_value));
        TIME_SHIELD_TEST_CHECK(ts_value == to_timestamp(2024, 3, 20, 12, 34, 5));
        (void)ts_value;
    }

    {
        const ts_ms_t utc_ms = ts_ms(2024, 3, 20, 12, 34, 56, 789);
        const std::string text = to_string_ms("%a %b %e %H:%M:%S %Y", utc_ms, 0);
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        TIME_SHIELD_TEST_CHECK(try_parse_format(text, std::string("%c"), dt, tz));
        TIME_SHIELD_TEST_CHECK(dt.year == 2024);
        TIME_SHIELD_TEST_CHECK(dt.mon == 3);
        TIME_SHIELD_TEST_CHECK(dt.day == 20);
        TIME_SHIELD_TEST_CHECK(dt.hour == 12);
        TIME_SHIELD_TEST_CHECK(dt.min == 34);
        TIME_SHIELD_TEST_CHECK(dt.sec == 56);
        (void)dt;
        (void)tz;
    }

    {
        const ts_ms_t utc_ms = ts_ms(2024, 3, 20, 12, 34, 56, 789);
        const std::string text = to_string_ms("%Y-%m-%d %H:%M:%S.%sss %z", utc_ms, -(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN));
        ts_ms_t parsed = 0;
        TIME_SHIELD_TEST_CHECK(try_parse_format_ts_ms(text, std::string("%Y-%m-%d %H:%M:%S.%sss %z"), parsed));
        TIME_SHIELD_TEST_CHECK(parsed == utc_ms);
        (void)parsed;
    }

    {
        DateTimeStruct dt{};
        TimeZoneStruct tz{};
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-07-08X12:34:56"), std::string("%Y-%m-%d %H:%M:%S"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-02-30 12:34:56"), std::string("%Y-%m-%d %H:%M:%S"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-07-08 12:34:56 +01"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-07-08 12:34:56 +010"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-07-08 12:34:56 +01:0"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-07-08 12:34:56 +01:000"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-07-08 12:34:56 +2500"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2024-07-08 12:34:56 +25:00"), std::string("%Y-%m-%d %H:%M:%S %z"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2025-12-16 2025-51-2"), std::string("%Y-%m-%d %G-%V-%u"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2025-00-1"), std::string("%G-%V-%u"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2025-54-1"), std::string("%G-%V-%u"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2025-51-0"), std::string("%G-%V-%u"), dt, tz));
        TIME_SHIELD_TEST_CHECK(!try_parse_format(std::string("2025-51-8"), std::string("%G-%V-%u"), dt, tz));
        (void)dt;
        (void)tz;
    }

    return 0;
}
