#include <time_shield.hpp>

#include <iostream>

int helper();

int main() {
    using namespace time_shield;

    TimeZoneStruct tz{};
    if (!parse_time_zone("-05:00", tz)) {
        std::cerr << "Failed to parse time zone" << std::endl;
        return 1;
    }

    DateTimeStruct dt{};
    if (!parse_iso8601("2024-07-08T12:34:56-05:00", dt, tz)) {
        std::cerr << "Failed to parse ISO8601" << std::endl;
        return 1;
    }

    const auto* weekday_name = to_cstr(Weekday::MON);
    const auto& month_name = to_str(Month::DEC);
    const auto& zone_name = to_str(TimeZone::UTC);
    const auto readable = to_human_readable(1720455296);

    std::cout << weekday_name << ' ' << month_name << ' ' << zone_name << ' ' << readable << std::endl;

    return helper();
}
