/// \file time_zone_conversions_example.cpp
/// \brief Demonstrates functions from time_shield::time_zone_conversions.
///
/// This example converts CET and EET timestamps to GMT and prints the
/// human-readable representations.

#include <iostream>

#if defined(_WIN32)
#include <time_shield.hpp>

int main() {
    using namespace time_shield;

    // Create a timestamp for 2024-06-21 12:00:00 CET
    ts_t cet_ts = to_ts(2024, Month::JUN, 21, 12, 0, 0);
    ts_t gmt_from_cet = cet_to_gmt(cet_ts);

    // Create a timestamp for 2024-06-21 12:00:00 EET
    ts_t eet_ts = to_ts(2024, Month::JUN, 21, 12, 0, 0);
    ts_t gmt_from_eet = eet_to_gmt(eet_ts);

    std::cout << "CET time: " << to_iso8601(cet_ts) << '\n';
    std::cout << "GMT from CET: " << to_iso8601(gmt_from_cet) << '\n';

    std::cout << "EET time: " << to_iso8601(eet_ts) << '\n';
    std::cout << "GMT from EET: " << to_iso8601(gmt_from_eet) << '\n';
    
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
#else
int main() {
    std::cout << "time_zone_conversions.hpp requires Windows." << std::endl;
    return 0;
}
#endif
