#include <time_shield/time_parser.hpp>
#include <time_shield/time_zone_offset_conversions.hpp>

#include <cassert>
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
        assert(parse_time_zone_name("UTC", zone));
        assert(zone == UTC);
        assert(parse_time_zone_name(" CET ", zone));
        assert(zone == CET);
        assert(parse_time_zone_name("JST", zone));
        assert(zone == JST);
        assert(parse_tz_name("  ET\t", zone));
        assert(zone == ET);
        assert(parse_tz_name("WITA", zone));
        assert(zone == WITA);
    }

    {
        TimeZone zone = CET;
        assert(!parse_time_zone_name("", zone));
        assert(zone == UNKNOWN);
        assert(!parse_time_zone_name("unknown", zone));
        assert(zone == UNKNOWN);
        assert(!parse_time_zone_name("utc", zone));
        assert(zone == UNKNOWN);
        assert(!parse_time_zone_name("Kyiv", zone));
        assert(zone == UNKNOWN);
        assert(!parse_time_zone_name("UTC+03:00", zone));
        assert(zone == UNKNOWN);
        assert(!parse_time_zone_name("+03:00", zone));
        assert(zone == UNKNOWN);
        assert(!parse_time_zone_name("UNKNOWN", zone));
        assert(zone == UNKNOWN);
        assert(!parse_tz_name(static_cast<const char*>(nullptr), zone));
        assert(zone == UNKNOWN);
    }

    {
        TimeZoneStruct tz = create_time_zone_struct(0, 0, true);
        assert(parse_time_zone("+23:59", tz));
        assert(tz.hour == 23);
        assert(tz.min == 59);
        assert(tz.is_positive);
        assert(!is_valid_tz_offset(to_offset(tz)));
    }

    {
        TimeZoneStruct tz = create_time_zone_struct(9, 9, true);
        assert(!parse_time_zone("UTC", tz));
        assert(tz.hour == 9);
        assert(tz.min == 9);
        assert(tz.is_positive);

        assert(!parse_time_zone("CET", tz));
        assert(tz.hour == 9);
        assert(tz.min == 9);
        assert(tz.is_positive);
    }

#if __cplusplus >= 201703L
    {
        TimeZone zone = UNKNOWN;
        const std::string_view value = "  JST ";
        assert(parse_time_zone_name(value, zone));
        assert(zone == JST);
        assert(parse_tz_name(std::string_view("UTC"), zone));
        assert(zone == UTC);
    }
#endif

    return EXIT_SUCCESS;
}
