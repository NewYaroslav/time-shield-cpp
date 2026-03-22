/// \file time_parser_example.cpp
/// \brief Demonstrates ISO8601, ISO week-date, and custom-format parsing helpers.

#include <time_shield/DateTime.hpp>
#include <time_shield/time_format_parser.hpp>
#include <time_shield/time_formatting.hpp>
#include <time_shield/time_parser.hpp>

#include <iostream>

int main() {
    using namespace time_shield;

    // Parse ISO8601 into calendar fields and an explicit fixed offset.
    const std::string iso8601 = "2024-11-25T14:30:00-05:30";
    DateTimeStruct iso_dt{};
    TimeZoneStruct iso_tz{};
    if (parse_iso8601(iso8601, iso_dt, iso_tz)) {
        std::cout << "ISO fields: "
                  << iso_dt.year << '-' << iso_dt.mon << '-' << iso_dt.day << ' '
                  << iso_dt.hour << ':' << iso_dt.min << ':' << iso_dt.sec << ' '
                  << to_string(iso_tz) << '\n';
    }

    // Offset-aware strings are converted to the corresponding UTC instant.
    ts_t utc_ts = 0;
    if (str_to_ts(iso8601, utc_ts)) {
        std::cout << "UTC seconds: " << utc_ts << '\n';
    }

    // Plain strings without a timezone token are interpreted as UTC.
    ts_ms_t utc_ms = 0;
    if (str_to_ts_ms("2024-11-25T20:00:00.250", utc_ms)) {
        std::cout << "UTC milliseconds: " << utc_ms << '\n';
    }

    // Floating timestamps keep the fractional part in seconds.
    fts_t utc_fts = 0.0;
    if (str_to_fts("2024-11-25T20:00:00.250Z", utc_fts)) {
        std::cout << "UTC floating seconds: " << utc_fts << '\n';
    }

    // Formatter and parser use the same custom grammar for round-trips.
    const tz_t utc_offset = -(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);
    const std::string custom = to_string("%Y-%m-%d %H:%M:%S %z", utc_ts, utc_offset);
    std::cout << "Custom formatted: " << custom << '\n';

    ts_t reparsed_ts = 0;
    if (try_parse_format_ts(custom, "%Y-%m-%d %H:%M:%S %z", reparsed_ts)) {
        std::cout << "Reparsed seconds: " << reparsed_ts << '\n';
    }

    const std::string custom_ms = to_string_ms("%Y-%m-%d %H:%M:%S.%sss %z", utc_ms, utc_offset);
    std::cout << "Custom formatted with ms: " << custom_ms << '\n';

    ts_ms_t reparsed_ms = 0;
    if (try_parse_format_ts_ms(custom_ms, "%Y-%m-%d %H:%M:%S.%sss %z", reparsed_ms)) {
        std::cout << "Parsed custom milliseconds: " << reparsed_ms << '\n';
    }

    // ISO week-date parsing accepts permissive mixed forms.
    IsoWeekDateStruct iso_week{};
    if (parse_iso_week_date("2025-W512", iso_week)) {
        std::cout << "ISO week mixed form: "
                  << iso_week.year << "-W" << iso_week.week << '-' << iso_week.weekday << '\n';
    }

    // Lowercase week marker is accepted and missing weekday defaults to Monday.
    if (DateTime::try_parse_iso_week_date("2025w51", iso_week)) {
        std::cout << "ISO week lowercase/default weekday: "
                  << iso_week.year << "-W" << iso_week.week << '-' << iso_week.weekday << '\n';
    }

    // Custom ISO week tokens round-trip through the formatter-compatible parser.
    const ts_t iso_week_ts = to_timestamp(2025, 12, 16);
    const std::string iso_week_custom = to_string("%G-%V-%u", iso_week_ts);
    std::cout << "ISO week custom formatted: " << iso_week_custom << '\n';

    DateTimeStruct iso_week_dt{};
    TimeZoneStruct iso_week_tz{};
    if (try_parse_format(iso_week_custom, "%G-%V-%u", iso_week_dt, iso_week_tz)) {
        std::cout << "ISO week reparsed calendar date: "
                  << iso_week_dt.year << '-' << iso_week_dt.mon << '-' << iso_week_dt.day << '\n';
    }

    if (try_parse_format("2025-W51", "%G-W%V", iso_week_dt, iso_week_tz)) {
        std::cout << "ISO week without weekday resolves to: "
                  << iso_week_dt.year << '-' << iso_week_dt.mon << '-' << iso_week_dt.day << '\n';
    }

    // Failed parsing simply returns false.
    const bool mismatch = try_parse_format_ts("2024/11/25", "%Y-%m-%d", reparsed_ts);
    std::cout << "Format mismatch: " << (mismatch ? "unexpected success" : "false") << '\n';

    // ISO week-based year/week tokens do not mix with Gregorian date tokens.
    const bool conflicting_tokens = try_parse_format(
        "2025-12-16 2025-51-2",
        "%Y-%m-%d %G-%V-%u",
        iso_week_dt,
        iso_week_tz);
    std::cout << "Mixed Gregorian/ISO-week tokens: "
              << (conflicting_tokens ? "unexpected success" : "false") << '\n';

    return 0;
}
