#include <time_shield/time_zone_conversions.hpp>
#include <time_shield/time_conversions.hpp>
#include <time_shield/time_unit_conversions.hpp>
#include "test_assert.hpp"
#include <string>
#include <vector>

/// \brief Tests generic zone-to-zone conversions and new Asia/EMEA fixed zones.
int main() {
    using namespace time_shield;

    struct FixedZoneCase {
        TimeZone zone;
        ts_t local;
        ts_t expected_gmt;
    };

    const std::vector<FixedZoneCase> fixed_zone_cases = {
        {IST,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 6, 30, 0)},
        {MYT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 4, 0, 0)},
        {WIB,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 5, 0, 0)},
        {WITA, to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 4, 0, 0)},
        {WIT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 3, 0, 0)},
        {KZT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 7, 0, 0)},
        {TRT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 9, 0, 0)},
        {BYT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 9, 0, 0)},
        {SGT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 4, 0, 0)},
        {ICT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 5, 0, 0)},
        {PHT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 4, 0, 0)},
        {GST,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 8, 0, 0)},
        {HKT,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 4, 0, 0)},
        {JST,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 3, 0, 0)},
        {KST,  to_timestamp(2024, 1, 15, 12, 0, 0), to_timestamp(2024, 1, 15, 3, 0, 0)},
    };

    for(const FixedZoneCase& item : fixed_zone_cases) {
        TIME_SHIELD_TEST_CHECK(zone_to_gmt(item.local, item.zone) == item.expected_gmt);
        TIME_SHIELD_TEST_CHECK(gmt_to_zone(item.expected_gmt, item.zone) == item.local);
        TIME_SHIELD_TEST_CHECK(convert_time_zone(item.local, item.zone, GMT) == item.expected_gmt);
        TIME_SHIELD_TEST_CHECK(convert_time_zone(item.local, item.zone, item.zone) == item.local);
        TIME_SHIELD_TEST_CHECK(gmt_to_zone(zone_to_gmt(item.local, item.zone), item.zone) == item.local);
    }

    const ts_t ist_local = to_timestamp(2024, 7, 15, 9, 0, 0);
    const ts_t myt_from_ist = convert_time_zone(ist_local, IST, MYT);
    TIME_SHIELD_TEST_CHECK(myt_from_ist == to_timestamp(2024, 7, 15, 11, 30, 0));
    TIME_SHIELD_TEST_CHECK(myt_from_ist == gmt_to_zone(zone_to_gmt(ist_local, IST), MYT));

    const ts_t wib_local = to_timestamp(2024, 7, 15, 14, 45, 0);
    const ts_t jst_from_wib = convert_time_zone(wib_local, WIB, JST);
    TIME_SHIELD_TEST_CHECK(jst_from_wib == to_timestamp(2024, 7, 15, 16, 45, 0));

    const ts_t gst_local = to_timestamp(2024, 7, 15, 8, 0, 0);
    const ts_t ist_from_gst = convert_time_zone(gst_local, GST, IST);
    TIME_SHIELD_TEST_CHECK(ist_from_gst == to_timestamp(2024, 7, 15, 9, 30, 0));

    const ts_t et_local = to_timestamp(2024, 7, 15, 9, 30, 0);
    const ts_t ist_from_et = convert_time_zone(et_local, ET, IST);
    TIME_SHIELD_TEST_CHECK(ist_from_et == to_timestamp(2024, 7, 15, 19, 0, 0));

    const ts_t ct_local = to_timestamp(2024, 7, 15, 9, 30, 0);
    const ts_t kst_from_ct = convert_time_zone(ct_local, CT, KST);
    TIME_SHIELD_TEST_CHECK(kst_from_ct == to_timestamp(2024, 7, 15, 23, 30, 0));

    const ts_t kyiv_local = to_timestamp(2024, 7, 15, 12, 0, 0);
    TIME_SHIELD_TEST_CHECK(kyiv_to_gmt(kyiv_local) == eet_to_gmt(kyiv_local));
    TIME_SHIELD_TEST_CHECK(gmt_to_kyiv(to_timestamp(2024, 7, 15, 9, 0, 0)) == gmt_to_eet(to_timestamp(2024, 7, 15, 9, 0, 0)));

    const ts_ms_t ist_local_ms = sec_to_ms<ts_ms_t>(to_timestamp(2024, 7, 15, 9, 0, 0)) + 123;
    const ts_ms_t myt_from_ist_ms = convert_time_zone_ms(ist_local_ms, IST, MYT);
    TIME_SHIELD_TEST_CHECK(myt_from_ist_ms == sec_to_ms<ts_ms_t>(to_timestamp(2024, 7, 15, 11, 30, 0)) + 123);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(ist_local_ms, IST) == sec_to_ms<ts_ms_t>(to_timestamp(2024, 7, 15, 3, 30, 0)) + 123);
    TIME_SHIELD_TEST_CHECK(gmt_to_zone_ms(sec_to_ms<ts_ms_t>(to_timestamp(2024, 7, 15, 3, 30, 0)) + 123, IST) == ist_local_ms);

    TIME_SHIELD_TEST_CHECK(convert_time_zone(to_timestamp(2024, 7, 15, 12, 0, 0), UNKNOWN, IST) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt(to_timestamp(2024, 7, 15, 12, 0, 0), UNKNOWN) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(gmt_to_zone(to_timestamp(2024, 7, 15, 12, 0, 0), UNKNOWN) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(sec_to_ms<ts_ms_t>(to_timestamp(2024, 7, 15, 12, 0, 0)), UNKNOWN) == ERROR_TIMESTAMP);

    TIME_SHIELD_TEST_CHECK(std::string(to_cstr(IST)) == "IST");
    TIME_SHIELD_TEST_CHECK(std::string(to_cstr(WIB)) == "WIB");
    TIME_SHIELD_TEST_CHECK(std::string(to_str(KZT)) == "KZT");
    TIME_SHIELD_TEST_CHECK(std::string(to_str(HKT, FULL_NAME)) == "Hong Kong Time");

    return 0;
}
