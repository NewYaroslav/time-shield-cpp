/// \file time_zone_conversions_example.cpp
/// \brief Demonstrates named and generic time zone conversion helpers.

#include <iostream>

#include <time_shield/time_formatting.hpp>
#include <time_shield/time_zone_conversions.hpp>

int main() {
    using namespace time_shield;

    // Fixed-offset example: convert GMT to India Standard Time.
    ts_t gmt = to_timestamp(2024, int(Month::JUN), 21, 12, 0, 0);
    ts_t ist = gmt_to_ist(gmt);

    // Regional alias example: Kyiv uses the EET/EEST conversion rules.
    ts_t kyiv = gmt_to_kyiv(gmt);

    // Generic matrix example: convert IST local time directly to Malaysia time.
    ts_t ist_local = to_timestamp(2024, int(Month::JUN), 21, 17, 30, 0);
    ts_t myt_local = convert_time_zone(ist_local, TimeZone::IST, TimeZone::MYT);

    std::cout << "GMT: " << to_iso8601(gmt) << '\n';
    std::cout << "IST from GMT: " << to_iso8601(ist) << '\n';
    std::cout << "Kyiv from GMT: " << to_iso8601(kyiv) << '\n';
    std::cout << "MYT from IST local: " << to_iso8601(myt_local) << '\n';

    return 0;
}
