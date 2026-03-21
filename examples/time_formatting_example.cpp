/// \file time_formatting_example.cpp
/// \brief Demonstrates functions from time_shield::time_formatting.

#include <time_shield/time_formatting.hpp>

#include <chrono>
#include <iostream>

int main() {
    using namespace time_shield;

    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const ts_t now_sec = static_cast<ts_t>(std::chrono::system_clock::to_time_t(now));
    const ts_ms_t now_ms = static_cast<ts_ms_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
    const tz_t utc_offset = 2 * SEC_PER_HOUR + 30 * SEC_PER_MIN;

    std::cout << "ISO8601 UTC:            " << to_iso8601(now_sec) << '\n';
    std::cout << "ISO8601 UTC (ms):       " << to_iso8601_ms(now_ms) << '\n';
    std::cout << "ISO8601 with offset:    " << to_iso8601(now_sec, utc_offset) << '\n';
    std::cout << "Custom UTC format:      "
              << to_string("%Y-%m-%d %H:%M:%S", now_sec) << '\n';
    std::cout << "Custom local format:    "
              << to_string("%Y-%m-%d %H:%M:%S %z", now_sec, utc_offset) << '\n';
    std::cout << "Filename-safe string:   " << to_windows_filename(now_sec) << '\n';

    return 0;
}
