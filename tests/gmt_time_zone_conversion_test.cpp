#include <time_shield/time_zone_conversions.hpp>
#include <time_shield/time_conversions.hpp>
#include <cassert>

/// \brief Round-trip tests for GMT and CET/EET conversions around DST boundaries.
int main() {
    using namespace time_shield;

    for(int year : {2021, 2022, 2023, 2024}) {
        int start_day = last_sunday_month_day(year, MAR);

        ts_t cet_start_before = to_timestamp(year, int(MAR), start_day, 1, 59, 0);
        ts_t gmt_from_cet_before = cet_to_gmt(cet_start_before);
        assert(gmt_to_cet(gmt_from_cet_before) == cet_start_before);

        ts_t cet_start_after = to_timestamp(year, int(MAR), start_day, 3, 0, 0);
        ts_t gmt_from_cet_after = cet_to_gmt(cet_start_after);
        assert(gmt_to_cet(gmt_from_cet_after) == cet_start_after);

        ts_t eet_start_before = to_timestamp(year, int(MAR), start_day, 2, 59, 0);
        ts_t gmt_from_eet_before = eet_to_gmt(eet_start_before);
        assert(gmt_to_eet(gmt_from_eet_before) == eet_start_before);

        ts_t eet_start_after = to_timestamp(year, int(MAR), start_day, 4, 0, 0);
        ts_t gmt_from_eet_after = eet_to_gmt(eet_start_after);
        assert(gmt_to_eet(gmt_from_eet_after) == eet_start_after);

        int end_day = last_sunday_month_day(year, OCT);

        ts_t cet_end_before = to_timestamp(year, int(OCT), end_day, 1, 59, 0);
        ts_t gmt_from_cet_end_before = cet_to_gmt(cet_end_before);
        assert(gmt_to_cet(gmt_from_cet_end_before) == cet_end_before);

        ts_t cet_end_after = to_timestamp(year, int(OCT), end_day, 3, 0, 0);
        ts_t gmt_from_cet_end_after = cet_to_gmt(cet_end_after);
        assert(gmt_to_cet(gmt_from_cet_end_after) == cet_end_after);

        ts_t eet_end_before = to_timestamp(year, int(OCT), end_day, 3, 59, 0);
        ts_t gmt_from_eet_end_before = eet_to_gmt(eet_end_before);
        assert(gmt_to_eet(gmt_from_eet_end_before) == eet_end_before);

        ts_t eet_end_after = to_timestamp(year, int(OCT), end_day, 4, 0, 0);
        ts_t gmt_from_eet_end_after = eet_to_gmt(eet_end_after);
        assert(gmt_to_eet(gmt_from_eet_end_after) == eet_end_after);

        (void)start_day;
        (void)cet_start_before;
        (void)gmt_from_cet_before;
        (void)cet_start_after;
        (void)gmt_from_cet_after;
        (void)eet_start_before;
        (void)gmt_from_eet_before;
        (void)eet_start_after;
        (void)gmt_from_eet_after;
        (void)end_day;
        (void)cet_end_before;
        (void)gmt_from_cet_end_before;
        (void)cet_end_after;
        (void)gmt_from_cet_end_after;
        (void)eet_end_before;
        (void)gmt_from_eet_end_before;
        (void)eet_end_after;
        (void)gmt_from_eet_end_after;
    }

    return 0;
}
