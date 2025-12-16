#include <time_shield.hpp>

#include <iostream>
#include <string>

/// \brief Demonstrates DateTime parsing, formatting, arithmetic, and ISO week-date usage.
int main() {
    using namespace time_shield;

    const std::string input = "2025-12-16T10:20:30.123+02:30";
    DateTime dt;
    if (!DateTime::try_parse_iso8601(input, dt)) {
        std::cout << "Failed to parse input" << std::endl;
        return 1;
    }

    std::cout << "unix_ms: " << dt.unix_ms() << '\n';
    std::cout << "iso8601 utc: " << dt.to_iso8601_utc() << '\n';
    std::cout << "formatted: " << dt.format("%F %T") << '\n';

    const DateTime tomorrow = dt.add_days(1);
    std::cout << "tomorrow: " << tomorrow.to_iso8601() << '\n';
    std::cout << "start_of_day: " << dt.start_of_day().to_iso8601() << '\n';

    const IsoWeekDateStruct iso_week = dt.iso_week_date();
    std::cout << "ISO week-date: " << format_iso_week_date(iso_week) << '\n';

    return 0;
}
