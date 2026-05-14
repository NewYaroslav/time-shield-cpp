#include <time_shield/time_parser.hpp>
#include <time_shield/time_zone_offset_conversions.hpp>

#include "test_assert.hpp"
#include <cstdlib>
#include <string>
#if __cplusplus >= 201703L
#   include <string_view>
#endif

/// \brief Tests strict named-zone parsing beside numeric parse_time_zone.
int main() {
    using namespace time_shield;

    {
        TimeZone zone = UNKNOWN;
        TIME_SHIELD_TEST_CHECK(parse_time_zone_name("UTC", zone));
        TIME_SHIELD_TEST_CHECK(zone == UTC);
        TIME_SHIELD_TEST_CHECK(parse_time_zone_name(" CET ", zone));
        TIME_SHIELD_TEST_CHECK(zone == CET);
        TIME_SHIELD_TEST_CHECK(parse_time_zone_name("JST", zone));
        TIME_SHIELD_TEST_CHECK(zone == JST);
        TIME_SHIELD_TEST_CHECK(parse_tz_name("  ET\t", zone));
        TIME_SHIELD_TEST_CHECK(zone == ET);
        TIME_SHIELD_TEST_CHECK(parse_tz_name("WITA", zone));
        TIME_SHIELD_TEST_CHECK(zone == WITA);
    }

    {
        TimeZone zone = CET;
        TIME_SHIELD_TEST_CHECK(!parse_time_zone_name("", zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!parse_time_zone_name("unknown", zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!parse_time_zone_name("utc", zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!parse_time_zone_name("Kyiv", zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!parse_time_zone_name("UTC+03:00", zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!parse_time_zone_name("+03:00", zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!parse_time_zone_name("UNKNOWN", zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!parse_tz_name(static_cast<const char*>(nullptr), zone));
        TIME_SHIELD_TEST_CHECK(zone == UNKNOWN);
    }

    {
        TimeZoneStruct tz = create_time_zone_struct(0, 0, true);
        TIME_SHIELD_TEST_CHECK(parse_time_zone("+23:59", tz));
        TIME_SHIELD_TEST_CHECK(tz.hour == 23);
        TIME_SHIELD_TEST_CHECK(tz.min == 59);
        TIME_SHIELD_TEST_CHECK(tz.is_positive);
        TIME_SHIELD_TEST_CHECK(!is_valid_tz_offset(to_offset(tz)));
    }

    {
        TimeZoneStruct tz = create_time_zone_struct(9, 9, true);
        TIME_SHIELD_TEST_CHECK(!parse_time_zone("UTC", tz));
        TIME_SHIELD_TEST_CHECK(tz.hour == 9);
        TIME_SHIELD_TEST_CHECK(tz.min == 9);
        TIME_SHIELD_TEST_CHECK(tz.is_positive);

        TIME_SHIELD_TEST_CHECK(!parse_time_zone("CET", tz));
        TIME_SHIELD_TEST_CHECK(tz.hour == 9);
        TIME_SHIELD_TEST_CHECK(tz.min == 9);
        TIME_SHIELD_TEST_CHECK(tz.is_positive);
    }

#if __cplusplus >= 201703L
    {
        TimeZone zone = UNKNOWN;
        const std::string_view value = "  JST ";
        TIME_SHIELD_TEST_CHECK(parse_time_zone_name(value, zone));
        TIME_SHIELD_TEST_CHECK(zone == JST);
        TIME_SHIELD_TEST_CHECK(parse_tz_name(std::string_view("UTC"), zone));
        TIME_SHIELD_TEST_CHECK(zone == UTC);
    }
#endif

    return EXIT_SUCCESS;
}
