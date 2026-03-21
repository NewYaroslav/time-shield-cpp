#include <time_shield.hpp>

int main() {
    const time_shield::ts_t ts = 0;
    const time_shield::ts_ms_t ts_ms = 1234;

    const time_shield::Weekday weekday = time_shield::weekday_of_ts(ts);
    const time_shield::Weekday weekday_ms = time_shield::weekday_of_ts_ms(ts_ms);
    const time_shield::Weekday weekday_alias = time_shield::day_of_week<>(ts);
    const time_shield::Weekday weekday_alias_2 = time_shield::get_weekday_from_ts<>(ts);

    const time_shield::ts_ms_t from_components = time_shield::to_timestamp_ms(2024, 1, 2, 3, 4, 5, 6);
    const time_shield::fts_t floating_alias = time_shield::to_fts(2024, 1, 2, 3, 4, 5, 6);

    time_shield::DateTimeStruct dt{};
    dt.year = 2024;
    dt.mon = 1;
    dt.day = 2;
    dt.hour = 3;
    dt.min = 4;
    dt.sec = 5;
    dt.ms = 6;

    const time_shield::Weekday weekday_from_date = time_shield::day_of_week<>(dt);

    time_shield::TimeZone parsed_zone = time_shield::TimeZone::UNKNOWN;
    const bool has_zone = time_shield::parse_time_zone_name("UTC", parsed_zone);

    const time_shield::jdn_t jdn = time_shield::gregorian_ymd_to_jdn(1970, 1, 1);
    const time_shield::jd_t jd = time_shield::gregorian_ymd_to_jd(2000, 1, 1, 12, 0);

    const int sink = static_cast<int>(weekday)
        + static_cast<int>(weekday_ms)
        + static_cast<int>(weekday_alias)
        + static_cast<int>(weekday_alias_2)
        + static_cast<int>(weekday_from_date)
        + static_cast<int>(from_components != 0)
        + static_cast<int>(floating_alias != 0.0)
        + static_cast<int>(has_zone)
        + static_cast<int>(parsed_zone == time_shield::TimeZone::UTC)
        + static_cast<int>(jdn != 0)
        + static_cast<int>(jd != 0.0);
    return sink == -1 ? 1 : 0;
}
