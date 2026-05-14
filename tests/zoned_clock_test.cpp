#include <time_shield/ZonedClock.hpp>

#include "test_assert.hpp"
#include <cstdlib>
#include <string>

/// \brief Tests for ZonedClock fixed-offset and named-zone behavior.
int main() {
    using namespace time_shield;

    {
        const ZonedClock fixed(static_cast<tz_t>(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN));
        TIME_SHIELD_TEST_CHECK(!fixed.has_named_zone());
        TIME_SHIELD_TEST_CHECK(fixed.zone() == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(!fixed.use_ntp());
        TIME_SHIELD_TEST_CHECK(fixed.zone_name().empty());
        TIME_SHIELD_TEST_CHECK(fixed.zone_full_name() == "UTC+05:30");
        TIME_SHIELD_TEST_CHECK(fixed.offset_string() == "+05:30");
        TIME_SHIELD_TEST_CHECK(fixed.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 123)) == 5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);

        const DateTime snapshot = fixed.from_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 123));
        TIME_SHIELD_TEST_CHECK(snapshot.utc_offset() == 5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);
        TIME_SHIELD_TEST_CHECK(snapshot.to_iso8601() == "2024-01-15T17:30:00.123+05:30");
    }

    {
        const ZonedClock fixed(static_cast<tz_t>(-3 * SEC_PER_HOUR));
        TIME_SHIELD_TEST_CHECK(!fixed.has_named_zone());
        TIME_SHIELD_TEST_CHECK(fixed.zone_full_name() == "UTC-03:00");
        TIME_SHIELD_TEST_CHECK(fixed.offset_string() == "-03:00");

        const DateTime snapshot = fixed.from_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0));
        TIME_SHIELD_TEST_CHECK(snapshot.utc_offset() == -3 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(snapshot.to_iso8601() == "2024-01-15T09:00:00.000-03:00");
    }

    {
        ZonedClock created(CET);
        TIME_SHIELD_TEST_CHECK(ZonedClock::try_from_offset(static_cast<tz_t>(2 * SEC_PER_HOUR), created));
        TIME_SHIELD_TEST_CHECK(!created.has_named_zone());
        TIME_SHIELD_TEST_CHECK(created.zone() == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(created.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)) == 2 * SEC_PER_HOUR);

        const ZonedClock before_failure(IST);
        created = before_failure;
        TIME_SHIELD_TEST_CHECK(!ZonedClock::try_from_offset(static_cast<tz_t>(15 * SEC_PER_HOUR), created));
        TIME_SHIELD_TEST_CHECK(created.has_named_zone());
        TIME_SHIELD_TEST_CHECK(created.zone() == IST);

        bool threw = false;
        try {
            const ZonedClock invalid(static_cast<tz_t>(15 * SEC_PER_HOUR));
            (void)invalid;
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        TIME_SHIELD_TEST_CHECK(threw);
    }

    {
        const ZonedClock ist_clock(IST);
        const ZonedClock jst_clock(JST);
        TIME_SHIELD_TEST_CHECK(ist_clock.has_named_zone());
        TIME_SHIELD_TEST_CHECK(ist_clock.zone() == IST);
        TIME_SHIELD_TEST_CHECK(ist_clock.zone_name() == "IST");
        TIME_SHIELD_TEST_CHECK(ist_clock.zone_full_name() == "India Standard Time");
        TIME_SHIELD_TEST_CHECK(ist_clock.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)) == 5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);
        TIME_SHIELD_TEST_CHECK(jst_clock.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)) == 9 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(ist_clock.from_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)).to_iso8601() == "2024-01-15T17:30:00.000+05:30");
    }

    {
        const ts_ms_t winter_utc = to_ts_ms(2024, 1, 15, 12, 0, 0, 0);
        const ts_ms_t summer_utc = to_ts_ms(2024, 7, 15, 12, 0, 0, 0);

        const ZonedClock cet_clock(CET);
        const ZonedClock eet_clock(EET);
        const ZonedClock et_clock(ET);
        const ZonedClock ct_clock(CT);

        TIME_SHIELD_TEST_CHECK(cet_clock.offset_at_utc_ms(winter_utc) == SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(cet_clock.offset_at_utc_ms(summer_utc) == 2 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(eet_clock.offset_at_utc_ms(winter_utc) == 2 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(eet_clock.offset_at_utc_ms(summer_utc) == 3 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(et_clock.offset_at_utc_ms(winter_utc) == -5 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(et_clock.offset_at_utc_ms(summer_utc) == -4 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(ct_clock.offset_at_utc_ms(winter_utc) == -6 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(ct_clock.offset_at_utc_ms(summer_utc) == -5 * SEC_PER_HOUR);

        TIME_SHIELD_TEST_CHECK(cet_clock.from_utc_ms(winter_utc).to_iso8601() == "2024-01-15T13:00:00.000+01:00");
        TIME_SHIELD_TEST_CHECK(cet_clock.from_utc_ms(summer_utc).to_iso8601() == "2024-07-15T14:00:00.000+02:00");
        TIME_SHIELD_TEST_CHECK(et_clock.from_utc_ms(winter_utc).to_iso8601() == "2024-01-15T07:00:00.000-05:00");
        TIME_SHIELD_TEST_CHECK(et_clock.from_utc_ms(summer_utc).to_iso8601() == "2024-07-15T08:00:00.000-04:00");
    }

    {
        const ZonedClock cet_clock(CET);
        const ts_ms_t cet_fold_local = to_ts_ms(2024, 10, 27, 2, 30, 0, 0);
        LocalTimeResolution resolution =
            cet_clock.resolve_local_time_ms(cet_fold_local);
        TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::ambiguous);
        TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms == to_ts_ms(2024, 10, 27, 0, 30, 0, 0));
        TIME_SHIELD_TEST_CHECK(resolution.second_utc_ms == to_ts_ms(2024, 10, 27, 1, 30, 0, 0));
        TIME_SHIELD_TEST_CHECK(cet_clock.to_utc_ms(cet_fold_local,
                                   AmbiguousTimePolicy::first_occurrence,
                                   NonexistentTimePolicy::error) ==
               to_ts_ms(2024, 10, 27, 0, 30, 0, 0));
        TIME_SHIELD_TEST_CHECK(cet_clock.to_utc_ms(cet_fold_local,
                                   AmbiguousTimePolicy::second_occurrence,
                                   NonexistentTimePolicy::error) ==
               to_ts_ms(2024, 10, 27, 1, 30, 0, 0));
        TIME_SHIELD_TEST_CHECK(cet_clock.to_utc_ms(cet_fold_local) == ERROR_TIMESTAMP);

        const ts_ms_t cet_gap_local = to_ts_ms(2024, 3, 31, 2, 30, 0, 0);
        resolution = cet_clock.resolve_local_time_ms(cet_gap_local);
        TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::nonexistent);
        TIME_SHIELD_TEST_CHECK(cet_clock.to_utc_ms(cet_gap_local,
                                   AmbiguousTimePolicy::first_occurrence,
                                   NonexistentTimePolicy::shift_forward) ==
               to_ts_ms(2024, 3, 31, 1, 0, 0, 0));

        tz_t offset = 0;
        TIME_SHIELD_TEST_CHECK(cet_clock.try_offset_at_utc_ms(to_ts_ms(2024, 10, 27, 0, 30, 0, 0),
                                              offset));
        TIME_SHIELD_TEST_CHECK(offset == 2 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(cet_clock.try_offset_at_utc_ms(to_ts_ms(2024, 10, 27, 1, 30, 0, 0),
                                              offset));
        TIME_SHIELD_TEST_CHECK(offset == SEC_PER_HOUR);

        const ZonedClock et_clock(ET);
        TIME_SHIELD_TEST_CHECK(et_clock.try_offset_at_utc_ms(to_ts_ms(2024, 11, 3, 5, 30, 0, 0),
                                             offset));
        TIME_SHIELD_TEST_CHECK(offset == -4 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(et_clock.try_offset_at_utc_ms(to_ts_ms(2024, 11, 3, 6, 30, 0, 0),
                                             offset));
        TIME_SHIELD_TEST_CHECK(offset == -5 * SEC_PER_HOUR);
    }

    {
        const ZonedClock fixed(static_cast<tz_t>(3 * SEC_PER_HOUR));
        const ts_ms_t local_ms = to_ts_ms(2024, 5, 1, 12, 0, 0, 500);
        LocalTimeResolution resolution = fixed.resolve_local_time_ms(local_ms);
        TIME_SHIELD_TEST_CHECK(resolution.status == LocalTimeStatus::valid);
        TIME_SHIELD_TEST_CHECK(resolution.first_utc_ms == to_ts_ms(2024, 5, 1, 9, 0, 0, 500));
        TIME_SHIELD_TEST_CHECK(fixed.to_utc_ms(local_ms) == resolution.first_utc_ms);

        tz_t offset = 0;
        TIME_SHIELD_TEST_CHECK(fixed.try_offset_at_utc_ms(to_ts_ms(2024, 5, 1, 9, 0, 0, 0),
                                          offset));
        TIME_SHIELD_TEST_CHECK(offset == 3 * SEC_PER_HOUR);
        TIME_SHIELD_TEST_CHECK(!fixed.try_offset_at_utc_ms(ERROR_TIMESTAMP, offset));
    }

    {
        ZonedClock clock;
        TimeZone parsed_zone = UNKNOWN;
        TIME_SHIELD_TEST_CHECK(parse_time_zone_name("CET", parsed_zone));
        TIME_SHIELD_TEST_CHECK(parsed_zone == CET);

        TIME_SHIELD_TEST_CHECK(clock.try_set_zone(" CET "));
        TIME_SHIELD_TEST_CHECK(clock.has_named_zone());
        TIME_SHIELD_TEST_CHECK(clock.zone() == CET);
        TIME_SHIELD_TEST_CHECK(clock.zone_name() == "CET");

        TIME_SHIELD_TEST_CHECK(clock.try_set_zone("Z"));
        TIME_SHIELD_TEST_CHECK(!clock.has_named_zone());
        TIME_SHIELD_TEST_CHECK(clock.zone() == UNKNOWN);
        TIME_SHIELD_TEST_CHECK(clock.offset_now() == 0);

        TIME_SHIELD_TEST_CHECK(clock.try_set_zone("+05:30"));
        TIME_SHIELD_TEST_CHECK(!clock.has_named_zone());
        TIME_SHIELD_TEST_CHECK(clock.zone_full_name() == "UTC+05:30");

        TIME_SHIELD_TEST_CHECK(clock.try_set_zone("UTC"));
        TIME_SHIELD_TEST_CHECK(clock.has_named_zone());
        TIME_SHIELD_TEST_CHECK(clock.zone() == UTC);
        TIME_SHIELD_TEST_CHECK(clock.zone_name() == "UTC");
        TIME_SHIELD_TEST_CHECK(clock.zone_full_name() == "Coordinated Universal Time");

        TIME_SHIELD_TEST_CHECK(!clock.try_set_zone(""));
        TIME_SHIELD_TEST_CHECK(!clock.try_set_zone("utc"));
        TIME_SHIELD_TEST_CHECK(!clock.try_set_zone("Kyiv"));
        TIME_SHIELD_TEST_CHECK(!clock.try_set_zone("UTC+03:00"));
        TIME_SHIELD_TEST_CHECK(!clock.try_set_zone("unknown"));
        TIME_SHIELD_TEST_CHECK(!clock.try_set_offset(static_cast<tz_t>(15 * SEC_PER_HOUR)));
    }

    {
        const ZonedClock fixed(static_cast<tz_t>(2 * SEC_PER_HOUR));
        const std::string iso_local = fixed.to_iso8601();
        const std::string iso_utc = fixed.to_iso8601_utc();
        const std::string formatted = fixed.format("%Y-%m-%d %H:%M:%S %z");

        TIME_SHIELD_TEST_CHECK(iso_local.size() >= 6);
        TIME_SHIELD_TEST_CHECK(iso_local.substr(iso_local.size() - 6) == "+02:00");
        TIME_SHIELD_TEST_CHECK(!iso_utc.empty() && iso_utc[iso_utc.size() - 1] == 'Z');
        TIME_SHIELD_TEST_CHECK(formatted.size() >= 5);
        TIME_SHIELD_TEST_CHECK(formatted.substr(formatted.size() - 5) == "+0200");
    }

    {
        const ZonedClock fixed(static_cast<tz_t>(SEC_PER_HOUR));
        const ts_ms_t utc_ms = fixed.utc_time_ms();
        const ts_ms_t local_ms = fixed.local_time_ms();
        const ts_us_t utc_us = fixed.utc_time_us();
        const ts_us_t local_us = fixed.local_time_us();
        const ts_t utc_sec = fixed.utc_time_sec();
        const ts_t local_sec = fixed.local_time_sec();

        const ts_ms_t expected_delta_ms = static_cast<ts_ms_t>(SEC_PER_HOUR) * MS_PER_SEC;
        const ts_us_t expected_delta_us = static_cast<ts_us_t>(SEC_PER_HOUR) * US_PER_SEC;
        const ts_t expected_delta_sec = SEC_PER_HOUR;

        TIME_SHIELD_TEST_CHECK(std::llabs((local_ms - utc_ms) - expected_delta_ms) <= 50);
        TIME_SHIELD_TEST_CHECK(std::llabs((local_us - utc_us) - expected_delta_us) <= 50000);
        TIME_SHIELD_TEST_CHECK(std::llabs((local_sec - utc_sec) - expected_delta_sec) <= 1);
    }

    return EXIT_SUCCESS;
}
