#include <time_shield/time_conversions.hpp>
#include <cassert>
#include <cstdlib>
#include <cmath>

int main() {
    using namespace time_shield;

    const double epsilon = 1e-9;
    const ts_t pre_base_6h = to_timestamp(1899, 12, 29, 6, 0, 0);
    const ts_t pre_base_18h = to_timestamp(1899, 12, 29, 18, 0, 0);
    const ts_t base_6h = to_timestamp(1899, 12, 30, 6, 0, 0);

    const oadate_t oa_epoch = ts_to_oadate(static_cast<ts_t>(0));
    assert(std::fabs(oa_epoch - static_cast<oadate_t>(OLE_EPOCH)) < epsilon);

    const oadate_t oa_half_day = ts_to_oadate(static_cast<ts_t>(SEC_PER_DAY / 2));
    assert(std::fabs(oa_half_day - (static_cast<oadate_t>(OLE_EPOCH) + 0.5)) < epsilon);

    const oadate_t oa_zero = to_oadate(1899, 12, 30, 0, 0, 0, 0);
    assert(std::fabs(oa_zero) < epsilon);
    assert(std::fabs(to_oadate(1899, 12, 29, 6, 0, 0, 0) - static_cast<oadate_t>(-1.25)) < epsilon);
    assert(std::fabs(to_oadate(1899, 12, 30, 6, 0, 0, 0) - static_cast<oadate_t>(0.25)) < epsilon);

    const ts_t round_trip_ts = oadate_to_ts(oa_half_day);
    assert(round_trip_ts == static_cast<ts_t>(SEC_PER_DAY / 2));
    assert(oadate_to_ts(static_cast<oadate_t>(-1.25)) == pre_base_6h);
    assert(oadate_to_ts(static_cast<oadate_t>(-1.75)) == pre_base_18h);
    assert(oadate_to_ts(static_cast<oadate_t>(-0.25)) == base_6h);
    assert(oadate_to_ts(static_cast<oadate_t>(-0.25)) == oadate_to_ts(static_cast<oadate_t>(0.25)));
    assert(std::fabs(oadate_to_fts(static_cast<oadate_t>(-1.25)) - static_cast<fts_t>(pre_base_6h)) < epsilon);

    const ts_ms_t day_ms = static_cast<ts_ms_t>(MS_PER_DAY);
    const oadate_t oa_ms = ts_ms_to_oadate(day_ms);
    assert(std::llabs(oadate_to_ts_ms(oa_ms) - day_ms) <= 1);
    const ts_ms_t pre_base_ms = to_timestamp_ms(1899, 12, 29, 6, 0, 0, 250);
    const oadate_t pre_base_oa_ms = ts_ms_to_oadate(pre_base_ms);
    const oadate_t expected_pre_base_oa_ms = static_cast<oadate_t>(-1.25)
        - static_cast<oadate_t>(250.0) / static_cast<oadate_t>(MS_PER_DAY);
    assert(std::fabs(pre_base_oa_ms - expected_pre_base_oa_ms) < 2e-12);
    assert(std::llabs(oadate_to_ts_ms(pre_base_oa_ms) - pre_base_ms) <= 1);

    const jd_t jd_epoch = ts_to_jd(static_cast<ts_t>(0));
    assert(std::fabs(jd_epoch - 2440587.5) < epsilon);

    const mjd_t mjd_epoch = ts_to_mjd(static_cast<ts_t>(0));
    assert(std::fabs(mjd_epoch - 40587.0) < epsilon);

    const jd_t jd_year2000 = gregorian_to_jd(1U, 1U, 2000U, 12U, 0U, 0U, 0U);
    assert(std::fabs(jd_year2000 - 2451545.0) < epsilon);

    const jdn_t jdn_unix_epoch = gregorian_to_jdn(1U, 1U, 1970U);
    assert(jdn_unix_epoch == static_cast<jdn_t>(2440588));

    const double phase_epoch = moon_phase(static_cast<fts_t>(0));
    assert(std::fabs(phase_epoch - 0.7520754628736458) < 1e-12);

    const double age_epoch = moon_age_days(static_cast<fts_t>(0));
    assert(std::fabs(age_epoch - 22.209231150442246) < 1e-9);

    (void)epsilon;
    (void)pre_base_6h;
    (void)pre_base_18h;
    (void)base_6h;
    (void)oa_epoch;
    (void)oa_half_day;
    (void)oa_zero;
    (void)round_trip_ts;
    (void)day_ms;
    (void)oa_ms;
    (void)pre_base_ms;
    (void)pre_base_oa_ms;
    (void)expected_pre_base_oa_ms;
    (void)jd_epoch;
    (void)mjd_epoch;
    (void)jd_year2000;
    (void)jdn_unix_epoch;
    (void)phase_epoch;
    (void)age_epoch;

    return 0;
}
