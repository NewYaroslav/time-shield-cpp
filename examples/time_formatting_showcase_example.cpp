/// \file time_formatting_showcase_example.cpp
/// \brief Demonstrates a broader set of formatter helpers.

#include <time_shield/time_formatting.hpp>

#include <iostream>

int main() {
    using namespace time_shield;

    const ts_t utc_ts = to_timestamp(2024, 6, 21, 12, 34, 56);
    const ts_ms_t utc_ms = ts_ms(2024, 6, 21, 12, 34, 56, 789);
    const tz_t utc_offset = 2 * SEC_PER_HOUR + 30 * SEC_PER_MIN;

    // ISO8601 helpers.
    std::cout << "ISO8601 local default:  " << to_iso8601(utc_ts) << '\n';
    std::cout << "ISO8601 UTC:            " << to_iso8601_utc(utc_ts) << '\n';
    std::cout << "ISO8601 date:           " << to_iso8601_date(utc_ts) << '\n';
    std::cout << "ISO8601 time:           " << to_iso8601_time(utc_ts) << '\n';
    std::cout << "ISO8601 time UTC:       " << to_iso8601_time_utc(utc_ts) << '\n';
    std::cout << "ISO8601 with offset:    " << to_iso8601(utc_ts, utc_offset) << '\n';
    std::cout << "ISO8601 ms local:       " << to_iso8601_ms(utc_ms) << '\n';
    std::cout << "ISO8601 ms UTC:         " << to_iso8601_utc_ms(utc_ms) << '\n';
    std::cout << "ISO8601 ms with offset: " << to_iso8601_ms(utc_ms, utc_offset) << '\n';

    // Custom formatting helpers.
    std::cout << "Custom UTC format:      "
              << to_string("%Y-%m-%d %H:%M:%S", utc_ts) << '\n';
    std::cout << "Custom local format:    "
              << to_string("%Y-%m-%d %H:%M:%S %z", utc_ts, utc_offset) << '\n';
    std::cout << "Custom UTC format (ms): "
              << to_string_ms("%Y-%m-%d %H:%M:%S.%sss", utc_ms) << '\n';
    std::cout << "Custom local fmt (ms):  "
              << to_string_ms("%Y-%m-%d %H:%M:%S.%sss %z", utc_ms, utc_offset) << '\n';

    // Specialized output helpers.
    std::cout << "MQL5 date/time:         " << to_mql5_date_time(utc_ts) << '\n';
    std::cout << "Filename-safe string:   " << to_windows_filename(utc_ts) << '\n';
    std::cout << "Filename-safe ms:       " << to_windows_filename_ms(utc_ms) << '\n';
    std::cout << "Human readable:         " << to_human_readable(utc_ts) << '\n';
    std::cout << "Human readable (ms):    " << to_human_readable_ms(utc_ms) << '\n';

    return 0;
}
