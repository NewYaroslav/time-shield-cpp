/// \file time_formatting_example.cpp
/// \brief Demonstrates functions from time_shield::time_formatting.
///
/// This example formats current timestamps using various utilities
/// provided by time_formatting.hpp.

#include <iostream>
#include <chrono>

#if defined(_WIN32)
#include <time_shield/time_formatting.hpp>

int main() {
    using namespace time_shield;

    auto now = std::chrono::system_clock::now();
    const ts_t now_sec = static_cast<ts_t>(std::chrono::system_clock::to_time_t(now));
    const ts_ms_t now_ms = static_cast<ts_ms_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());

    std::cout << "ISO8601:                " << to_iso8601(now_sec) << '\n';
    std::cout << "ISO8601 with ms:        " << to_iso8601_ms(now_ms) << '\n';
    std::cout << "Custom format:          "
              << to_string("%Y-%m-%d %H:%M:%S", now_sec) << '\n';
    std::cout << "Windows filename:       " << to_windows_filename(now_sec) << '\n';
    std::cout << "Windows filename (ms):  " << to_windows_filename_ms(now_ms) << '\n';
    std::cout << "MQL5 date/time:         " << to_mql5_date_time(now_sec) << '\n';
    std::cout << "Human readable:         " << to_human_readable(now_sec) << '\n';
    std::cout << "Human readable (ms):    " << to_human_readable_ms(now_ms) << '\n';

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
#else
int main() {
    std::cout << "time_formatting.hpp requires Windows." << std::endl;
    return 0;
}
#endif
