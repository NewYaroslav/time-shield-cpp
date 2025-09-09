#include <time_shield/time_zone_conversions.hpp>
#include <time_shield/time_conversions.hpp>
#include <cassert>

/// \brief Tests CET/EET conversions to UTC including DST transitions.
///
/// DST rules follow the European Union schedule: since 2002 the shift
/// occurs at 01:00 UTC on the last Sunday of March (start) and last
/// Sunday of October (end). Older rules used different switch hours; the
/// conversion helpers document and support both behaviours.
int main() {
    using namespace time_shield;

    ts_t cet_winter = to_timestamp(2023, 1, 1, 12, 0, 0);
    ts_t gmt_winter = cet_to_gmt(cet_winter);
    assert(gmt_winter == to_timestamp(2023, 1, 1, 11, 0, 0));

    ts_t cet_summer = to_timestamp(2023, 7, 1, 12, 0, 0);
    ts_t gmt_summer = cet_to_gmt(cet_summer);
    assert(gmt_summer == to_timestamp(2023, 7, 1, 10, 0, 0));

    ts_t cet_before = to_timestamp(2023, 3, 26, 1, 30, 0);
    ts_t gmt_before = cet_to_gmt(cet_before);
    assert(gmt_before == to_timestamp(2023, 3, 26, 0, 30, 0));

    ts_t cet_after = to_timestamp(2023, 3, 26, 3, 30, 0);
    ts_t gmt_after = cet_to_gmt(cet_after);
    assert(gmt_after == to_timestamp(2023, 3, 26, 1, 30, 0));

    ts_t eet_winter = to_timestamp(2023, 1, 1, 12, 0, 0);
    ts_t gmt_eet_winter = eet_to_gmt(eet_winter);
    assert(gmt_eet_winter == to_timestamp(2023, 1, 1, 10, 0, 0));

    ts_t eet_summer = to_timestamp(2023, 7, 1, 12, 0, 0);
    ts_t gmt_eet_summer = eet_to_gmt(eet_summer);
    assert(gmt_eet_summer == to_timestamp(2023, 7, 1, 9, 0, 0));

    return 0;
}
