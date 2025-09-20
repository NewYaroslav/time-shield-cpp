#include <time_shield.hpp>

#include <string>

namespace {
int accumulate_sizes(const std::string& a, const std::string& b, const std::string& c) {
    return static_cast<int>(a.size() + b.size() + c.size());
}
}

int helper() {
    using namespace time_shield;

    TimeZoneStruct tz{};
    parse_time_zone("+02:30", tz);

    DateTimeStruct dt{};
    parse_iso8601("2024-01-02T03:04:05+02:30", dt, tz);

    const auto tz_string = to_string(tz);
    const auto* weekday_name = to_cstr(Weekday::FRI);
    const auto& month_name = to_str(Month::JAN);
    const auto& zone_name = to_str(TimeZone::GMT);
    const auto readable = to_human_readable(1704155045);
    const auto readable_ms = to_human_readable_ms(1704155045123);

    const auto result = accumulate_sizes(tz_string, readable, readable_ms) +
                        static_cast<int>(month_name.size() + zone_name.size() +
                                         std::char_traits<char>::length(weekday_name)) +
                        dt.day + tz.min;
    static_cast<void>(result);
    return 0;
}
