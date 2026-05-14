#include <time_shield/time_zone_conversions.hpp>
#include <time_shield/time_conversions.hpp>
#include "test_assert.hpp"

int main() {
    using namespace time_shield;

    const ts_ms_t cet_gap = to_timestamp_ms(2024, int(MAR), 31, 2, 30, 0);
    LocalTimeResolution resolution = resolve_local_time_ms(cet_gap, CET);
    TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::nonexistent);
    TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms_strict(cet_gap, CET) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(cet_gap,
                          CET,
                          AmbiguousTimePolicy::first_occurrence,
                          NonexistentTimePolicy::error) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(cet_gap,
                          CET,
                          AmbiguousTimePolicy::first_occurrence,
                          NonexistentTimePolicy::shift_forward) ==
           to_timestamp_ms(2024, int(MAR), 31, 1, 0, 0));
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(cet_gap,
                          CET,
                          AmbiguousTimePolicy::first_occurrence,
                          NonexistentTimePolicy::shift_backward) ==
           to_timestamp_ms(2024, int(MAR), 31, 0, 59, 59, 999));
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(cet_gap,
                                CET,
                                UTC,
                                AmbiguousTimePolicy::first_occurrence,
                                NonexistentTimePolicy::error) ==
           ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(cet_gap,
                                CET,
                                UTC,
                                AmbiguousTimePolicy::first_occurrence,
                                NonexistentTimePolicy::shift_forward) ==
           to_timestamp_ms(2024, int(MAR), 31, 1, 0, 0));

    const ts_ms_t cet_fold = to_timestamp_ms(2024, int(OCT), 27, 2, 30, 0);
    resolution = resolve_local_time_ms(cet_fold, CET);
    TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::ambiguous);
    TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms ==
           to_timestamp_ms(2024, int(OCT), 27, 0, 30, 0));
    TIME_SHIELD_TEST_CHECK(resolution.second_utc_ms ==
           to_timestamp_ms(2024, int(OCT), 27, 1, 30, 0));
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms_strict(cet_fold, CET) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(cet_fold,
                          CET,
                          AmbiguousTimePolicy::first_occurrence,
                          NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(OCT), 27, 0, 30, 0));
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(cet_fold,
                          CET,
                          AmbiguousTimePolicy::second_occurrence,
                          NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(OCT), 27, 1, 30, 0));
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(cet_fold,
                          CET,
                          AmbiguousTimePolicy::error,
                          NonexistentTimePolicy::error) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(cet_fold,
                                CET,
                                UTC,
                                AmbiguousTimePolicy::first_occurrence,
                                NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(OCT), 27, 0, 30, 0));
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(cet_fold,
                                CET,
                                IST,
                                AmbiguousTimePolicy::second_occurrence,
                                NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(OCT), 27, 7, 0, 0));

    tz_t offset = 0;
    TIME_SHIELD_TEST_CHECK(zone_offset_at_utc_ms(to_timestamp_ms(2024, int(OCT), 27, 0, 30, 0),
                                 CET,
                                 offset));
    TIME_SHIELD_TEST_CHECK(offset == 2 * SEC_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(zone_offset_at_utc_ms(to_timestamp_ms(2024, int(OCT), 27, 1, 30, 0),
                                 CET,
                                 offset));
    TIME_SHIELD_TEST_CHECK(offset == SEC_PER_HOUR);

    const ts_ms_t et_gap = to_timestamp_ms(2024, int(MAR), 10, 2, 30, 0);
    resolution = resolve_local_time_ms(et_gap, ET);
    TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::nonexistent);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms_strict(et_gap, ET) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(et_gap,
                          ET,
                          AmbiguousTimePolicy::first_occurrence,
                          NonexistentTimePolicy::shift_forward) ==
           to_timestamp_ms(2024, int(MAR), 10, 7, 0, 0));
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(et_gap,
                          ET,
                          AmbiguousTimePolicy::first_occurrence,
                          NonexistentTimePolicy::shift_backward) ==
           to_timestamp_ms(2024, int(MAR), 10, 6, 59, 59, 999));
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(et_gap,
                                ET,
                                UTC,
                                AmbiguousTimePolicy::first_occurrence,
                                NonexistentTimePolicy::error) ==
           ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(et_gap,
                                ET,
                                UTC,
                                AmbiguousTimePolicy::first_occurrence,
                                NonexistentTimePolicy::shift_forward) ==
           to_timestamp_ms(2024, int(MAR), 10, 7, 0, 0));

    const ts_ms_t et_fold = to_timestamp_ms(2024, int(NOV), 3, 1, 30, 0);
    resolution = resolve_local_time_ms(et_fold, ET);
    TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::ambiguous);
    TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms ==
           to_timestamp_ms(2024, int(NOV), 3, 5, 30, 0));
    TIME_SHIELD_TEST_CHECK(resolution.second_utc_ms ==
           to_timestamp_ms(2024, int(NOV), 3, 6, 30, 0));
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(et_fold,
                          ET,
                          AmbiguousTimePolicy::first_occurrence,
                          NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(NOV), 3, 5, 30, 0));
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_ms(et_fold,
                          ET,
                          AmbiguousTimePolicy::second_occurrence,
                          NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(NOV), 3, 6, 30, 0));
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(et_fold,
                                ET,
                                UTC,
                                AmbiguousTimePolicy::first_occurrence,
                                NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(NOV), 3, 5, 30, 0));
    TIME_SHIELD_TEST_CHECK(convert_time_zone_ms(et_fold,
                                ET,
                                UTC,
                                AmbiguousTimePolicy::second_occurrence,
                                NonexistentTimePolicy::error) ==
           to_timestamp_ms(2024, int(NOV), 3, 6, 30, 0));
    TIME_SHIELD_TEST_CHECK(zone_offset_at_utc_ms(to_timestamp_ms(2024, int(NOV), 3, 5, 30, 0),
                                 ET,
                                 offset));
    TIME_SHIELD_TEST_CHECK(offset == -4 * SEC_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(zone_offset_at_utc_ms(to_timestamp_ms(2024, int(NOV), 3, 6, 30, 0),
                                 ET,
                                 offset));
    TIME_SHIELD_TEST_CHECK(offset == -5 * SEC_PER_HOUR);

    const ts_ms_t utc_local = to_timestamp_ms(2024, int(JUL), 15, 12, 0, 0);
    resolution = resolve_local_time_ms(utc_local, UTC);
    TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::valid);
    TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms == utc_local);
    TIME_SHIELD_TEST_CHECK(resolution.second_utc_ms == ERROR_TIMESTAMP);

    const ts_ms_t ist_local =
        to_timestamp_ms(2024, int(JUL), 15, 12, 0, 0, 123);
    resolution = resolve_local_time_ms(ist_local, IST);
    TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::valid);
    TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms ==
           to_timestamp_ms(2024, int(JUL), 15, 6, 30, 0, 123));

    resolution = resolve_local_time_ms(utc_local, UNKNOWN);
    TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::unsupported);
    TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms == ERROR_TIMESTAMP);

    const ts_t cet_fold_sec = to_timestamp(2024, int(OCT), 27, 2, 30, 0);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt_strict(cet_fold_sec, CET) == ERROR_TIMESTAMP);
    TIME_SHIELD_TEST_CHECK(zone_to_gmt(cet_fold_sec,
                       CET,
                       AmbiguousTimePolicy::first_occurrence,
                       NonexistentTimePolicy::error) ==
           to_timestamp(2024, int(OCT), 27, 0, 30, 0));
    TIME_SHIELD_TEST_CHECK(convert_time_zone(cet_fold_sec,
                             CET,
                             UTC,
                             AmbiguousTimePolicy::first_occurrence,
                             NonexistentTimePolicy::error) ==
           to_timestamp(2024, int(OCT), 27, 0, 30, 0));
    TIME_SHIELD_TEST_CHECK(zone_offset_at_utc(to_timestamp(2024, int(OCT), 27, 1, 30, 0),
                              CET,
                              offset));
    TIME_SHIELD_TEST_CHECK(offset == SEC_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(!zone_offset_at_utc_ms(ERROR_TIMESTAMP, CET, offset));
    TIME_SHIELD_TEST_CHECK(!zone_offset_at_utc_ms(utc_local, UNKNOWN, offset));

    return 0;
}
