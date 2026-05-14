#include <time_shield/iso_week_conversions.hpp>
#include <time_shield/time_conversions.hpp>
#include <time_shield/time_parser.hpp>

#include "test_assert.hpp"
#include <array>
#include <stdexcept>
#include <string>

/// \brief Checks for ISO week-date conversions, formatting, and parsing.
int main() {
    using namespace time_shield;

    struct IsoCase {
        int year;
        int month;
        int day;
        int iso_year;
        int iso_week;
        int iso_weekday;
    };

    const std::array<IsoCase, 9> calendar_cases = {{
        {2005, 1, 1, 2004, 53, 6},
        {2014, 12, 29, 2015, 1, 1},
        {2015, 1, 1, 2015, 1, 4},
        {2016, 1, 3, 2015, 53, 7},
        {2020, 12, 31, 2020, 53, 4},
        {2021, 1, 1, 2020, 53, 5},
        {2021, 12, 31, 2021, 52, 5},
        {2022, 1, 1, 2021, 52, 6},
        {2025, 12, 16, 2025, 51, 2},
    }};

    for (const auto& item : calendar_cases) {
        const auto iso = to_iso_week_date(item.year, item.month, item.day);
        TIME_SHIELD_TEST_CHECK(iso.year == item.iso_year);
        TIME_SHIELD_TEST_CHECK(iso.week == item.iso_week);
        TIME_SHIELD_TEST_CHECK(iso.weekday == item.iso_weekday);

        const auto date_back = iso_week_date_to_date(iso);
        TIME_SHIELD_TEST_CHECK(date_back.year == item.year);
        TIME_SHIELD_TEST_CHECK(date_back.mon == item.month);
        TIME_SHIELD_TEST_CHECK(date_back.day == item.day);

        const auto ts_value = to_timestamp(item.year, item.month, item.day);
        const auto iso_from_ts = to_iso_week_date(ts_value);
        TIME_SHIELD_TEST_CHECK(iso_from_ts.year == item.iso_year);
        TIME_SHIELD_TEST_CHECK(iso_from_ts.week == item.iso_week);
        TIME_SHIELD_TEST_CHECK(iso_from_ts.weekday == item.iso_weekday);
    }

    TIME_SHIELD_TEST_CHECK(iso_weeks_in_year(2004) == 53);
    TIME_SHIELD_TEST_CHECK(iso_weeks_in_year(2005) == 52);
    TIME_SHIELD_TEST_CHECK(iso_weeks_in_year(2015) == 53);
    TIME_SHIELD_TEST_CHECK(iso_weeks_in_year(2020) == 53);
    TIME_SHIELD_TEST_CHECK(iso_weeks_in_year(2021) == 52);
    TIME_SHIELD_TEST_CHECK(iso_weeks_in_year(2025) == 52);

    IsoWeekDateStruct parsed{};
    TIME_SHIELD_TEST_CHECK(parse_iso_week_date("2025-W51-2", parsed));
    TIME_SHIELD_TEST_CHECK(parsed.year == 2025 && parsed.week == 51 && parsed.weekday == 2);
    TIME_SHIELD_TEST_CHECK(format_iso_week_date(parsed) == "2025-W51-2");

    TIME_SHIELD_TEST_CHECK(parse_iso_week_date("2025W512", parsed));
    TIME_SHIELD_TEST_CHECK(format_iso_week_date(parsed) == "2025-W51-2");

    TIME_SHIELD_TEST_CHECK(parse_iso_week_date("2025-W512", parsed));
    TIME_SHIELD_TEST_CHECK(parsed.year == 2025 && parsed.week == 51 && parsed.weekday == 2);

    TIME_SHIELD_TEST_CHECK(parse_iso_week_date("2025W51-2", parsed));
    TIME_SHIELD_TEST_CHECK(parsed.year == 2025 && parsed.week == 51 && parsed.weekday == 2);

    TIME_SHIELD_TEST_CHECK(parse_iso_week_date("2025-w51-2", parsed));
    TIME_SHIELD_TEST_CHECK(parsed.year == 2025 && parsed.week == 51 && parsed.weekday == 2);

    TIME_SHIELD_TEST_CHECK(parse_iso_week_date("2025-W51", parsed));
    TIME_SHIELD_TEST_CHECK(parsed.weekday == 1);
    TIME_SHIELD_TEST_CHECK(format_iso_week_date(parsed, true, false) == "2025-W51");

    const std::array<char, 10> iso_chars{{'2', '0', '2', '5', '-', 'W', '5', '1', '-', '2'}};
    TIME_SHIELD_TEST_CHECK(parse_iso_week_date(iso_chars.data(), iso_chars.size(), parsed));
    TIME_SHIELD_TEST_CHECK(parsed.year == 2025 && parsed.week == 51 && parsed.weekday == 2);

    TIME_SHIELD_TEST_CHECK(!parse_iso_week_date("2025-W00-1", parsed));
    TIME_SHIELD_TEST_CHECK(!parse_iso_week_date("2025-W54-1", parsed));
    TIME_SHIELD_TEST_CHECK(!parse_iso_week_date("2025-W51-0", parsed));
    TIME_SHIELD_TEST_CHECK(!parse_iso_week_date("2025-W51-8", parsed));
    TIME_SHIELD_TEST_CHECK(!parse_iso_week_date("2025-W51-X", parsed));

    const IsoWeekDateStruct round_trip_iso = create_iso_week_date_struct(2020, 53, 4);
    const auto round_trip_date = iso_week_date_to_date(round_trip_iso);
    const auto iso_again = to_iso_week_date(round_trip_date.year, round_trip_date.mon, round_trip_date.day);
    TIME_SHIELD_TEST_CHECK(iso_again.year == round_trip_iso.year);
    TIME_SHIELD_TEST_CHECK(iso_again.week == round_trip_iso.week);
    TIME_SHIELD_TEST_CHECK(iso_again.weekday == round_trip_iso.weekday);

    {
        const IsoWeekDateStruct no_weekday = create_iso_week_date_struct(2025, 51, 0);
        TIME_SHIELD_TEST_CHECK(format_iso_week_date(no_weekday, true, false) == "2025-W51");

        bool threw = false;
        try {
            (void)format_iso_week_date(no_weekday);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        TIME_SHIELD_TEST_CHECK(threw);
    }

    {
        bool threw = false;
        try {
            (void)iso_week_date_to_date(create_iso_week_date_struct(2025, 54, 1));
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        TIME_SHIELD_TEST_CHECK(threw);
    }

    DateTimeStruct parsed_dt{};
    TimeZoneStruct parsed_tz{};

    TIME_SHIELD_TEST_CHECK(parse_iso8601("2025-W51-2", parsed_dt, parsed_tz));
    TIME_SHIELD_TEST_CHECK(parsed_dt.year == 2025 && parsed_dt.mon == 12 && parsed_dt.day == 16);
    TIME_SHIELD_TEST_CHECK(parsed_dt.hour == 0 && parsed_dt.min == 0 && parsed_dt.sec == 0 && parsed_dt.ms == 0);

    TIME_SHIELD_TEST_CHECK(parse_iso8601("2025W512T10:15:30Z", parsed_dt, parsed_tz));
    TIME_SHIELD_TEST_CHECK(parsed_dt.year == 2025 && parsed_dt.mon == 12 && parsed_dt.day == 16);
    TIME_SHIELD_TEST_CHECK(parsed_dt.hour == 10 && parsed_dt.min == 15 && parsed_dt.sec == 30 && parsed_dt.ms == 0);
    TIME_SHIELD_TEST_CHECK(parsed_tz.hour == 0 && parsed_tz.min == 0 && parsed_tz.is_positive);

    TIME_SHIELD_TEST_CHECK(parse_iso8601("2025-W51T23:45:00-02:30", parsed_dt, parsed_tz));
    TIME_SHIELD_TEST_CHECK(parsed_dt.year == 2025 && parsed_dt.mon == 12 && parsed_dt.day == 15);
    TIME_SHIELD_TEST_CHECK(parsed_dt.hour == 23 && parsed_dt.min == 45 && parsed_dt.sec == 0);
    TIME_SHIELD_TEST_CHECK(parsed_tz.hour == 2 && parsed_tz.min == 30 && !parsed_tz.is_positive);

    return 0;
}
