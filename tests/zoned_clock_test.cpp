#include <time_shield/ZonedClock.hpp>

#include <cassert>
#include <cstdlib>
#include <string>

/// \brief Tests for ZonedClock fixed-offset and named-zone behavior.
int main() {
    using namespace time_shield;

    {
        const ZonedClock fixed(static_cast<tz_t>(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN));
        assert(!fixed.has_named_zone());
        assert(fixed.zone() == UNKNOWN);
        assert(!fixed.use_ntp());
        assert(fixed.zone_name().empty());
        assert(fixed.zone_full_name() == "UTC+05:30");
        assert(fixed.offset_string() == "+05:30");
        assert(fixed.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 123)) == 5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);

        const DateTime snapshot = fixed.from_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 123));
        assert(snapshot.utc_offset() == 5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);
        assert(snapshot.to_iso8601() == "2024-01-15T17:30:00.123+05:30");
    }

    {
        const ZonedClock fixed(static_cast<tz_t>(-3 * SEC_PER_HOUR));
        assert(!fixed.has_named_zone());
        assert(fixed.zone_full_name() == "UTC-03:00");
        assert(fixed.offset_string() == "-03:00");

        const DateTime snapshot = fixed.from_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0));
        assert(snapshot.utc_offset() == -3 * SEC_PER_HOUR);
        assert(snapshot.to_iso8601() == "2024-01-15T09:00:00.000-03:00");
    }

    {
        ZonedClock created(CET);
        assert(ZonedClock::try_from_offset(static_cast<tz_t>(2 * SEC_PER_HOUR), created));
        assert(!created.has_named_zone());
        assert(created.zone() == UNKNOWN);
        assert(created.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)) == 2 * SEC_PER_HOUR);

        const ZonedClock before_failure(IST);
        created = before_failure;
        assert(!ZonedClock::try_from_offset(static_cast<tz_t>(15 * SEC_PER_HOUR), created));
        assert(created.has_named_zone());
        assert(created.zone() == IST);

        bool threw = false;
        try {
            const ZonedClock invalid(static_cast<tz_t>(15 * SEC_PER_HOUR));
            (void)invalid;
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    {
        const ZonedClock ist_clock(IST);
        const ZonedClock jst_clock(JST);
        assert(ist_clock.has_named_zone());
        assert(ist_clock.zone() == IST);
        assert(ist_clock.zone_name() == "IST");
        assert(ist_clock.zone_full_name() == "India Standard Time");
        assert(ist_clock.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)) == 5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);
        assert(jst_clock.offset_at_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)) == 9 * SEC_PER_HOUR);
        assert(ist_clock.from_utc_ms(to_ts_ms(2024, 1, 15, 12, 0, 0, 0)).to_iso8601() == "2024-01-15T17:30:00.000+05:30");
    }

    {
        const ts_ms_t winter_utc = to_ts_ms(2024, 1, 15, 12, 0, 0, 0);
        const ts_ms_t summer_utc = to_ts_ms(2024, 7, 15, 12, 0, 0, 0);

        const ZonedClock cet_clock(CET);
        const ZonedClock eet_clock(EET);
        const ZonedClock et_clock(ET);
        const ZonedClock ct_clock(CT);

        assert(cet_clock.offset_at_utc_ms(winter_utc) == SEC_PER_HOUR);
        assert(cet_clock.offset_at_utc_ms(summer_utc) == 2 * SEC_PER_HOUR);
        assert(eet_clock.offset_at_utc_ms(winter_utc) == 2 * SEC_PER_HOUR);
        assert(eet_clock.offset_at_utc_ms(summer_utc) == 3 * SEC_PER_HOUR);
        assert(et_clock.offset_at_utc_ms(winter_utc) == -5 * SEC_PER_HOUR);
        assert(et_clock.offset_at_utc_ms(summer_utc) == -4 * SEC_PER_HOUR);
        assert(ct_clock.offset_at_utc_ms(winter_utc) == -6 * SEC_PER_HOUR);
        assert(ct_clock.offset_at_utc_ms(summer_utc) == -5 * SEC_PER_HOUR);

        assert(cet_clock.from_utc_ms(winter_utc).to_iso8601() == "2024-01-15T13:00:00.000+01:00");
        assert(cet_clock.from_utc_ms(summer_utc).to_iso8601() == "2024-07-15T14:00:00.000+02:00");
        assert(et_clock.from_utc_ms(winter_utc).to_iso8601() == "2024-01-15T07:00:00.000-05:00");
        assert(et_clock.from_utc_ms(summer_utc).to_iso8601() == "2024-07-15T08:00:00.000-04:00");
    }

    {
        ZonedClock clock;
        TimeZone parsed_zone = UNKNOWN;
        assert(parse_time_zone_name("CET", parsed_zone));
        assert(parsed_zone == CET);

        assert(clock.try_set_zone(" CET "));
        assert(clock.has_named_zone());
        assert(clock.zone() == CET);
        assert(clock.zone_name() == "CET");

        assert(clock.try_set_zone("Z"));
        assert(!clock.has_named_zone());
        assert(clock.zone() == UNKNOWN);
        assert(clock.offset_now() == 0);

        assert(clock.try_set_zone("+05:30"));
        assert(!clock.has_named_zone());
        assert(clock.zone_full_name() == "UTC+05:30");

        assert(clock.try_set_zone("UTC"));
        assert(clock.has_named_zone());
        assert(clock.zone() == UTC);
        assert(clock.zone_name() == "UTC");
        assert(clock.zone_full_name() == "Coordinated Universal Time");

        assert(!clock.try_set_zone(""));
        assert(!clock.try_set_zone("utc"));
        assert(!clock.try_set_zone("Kyiv"));
        assert(!clock.try_set_zone("UTC+03:00"));
        assert(!clock.try_set_zone("unknown"));
        assert(!clock.try_set_offset(static_cast<tz_t>(15 * SEC_PER_HOUR)));
    }

    {
        const ZonedClock fixed(static_cast<tz_t>(2 * SEC_PER_HOUR));
        const std::string iso_local = fixed.to_iso8601();
        const std::string iso_utc = fixed.to_iso8601_utc();
        const std::string formatted = fixed.format("%Y-%m-%d %H:%M:%S %z");

        assert(iso_local.size() >= 6);
        assert(iso_local.substr(iso_local.size() - 6) == "+02:00");
        assert(!iso_utc.empty() && iso_utc[iso_utc.size() - 1] == 'Z');
        assert(formatted.size() >= 5);
        assert(formatted.substr(formatted.size() - 5) == "+0200");
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

        assert(std::llabs((local_ms - utc_ms) - expected_delta_ms) <= 50);
        assert(std::llabs((local_us - utc_us) - expected_delta_us) <= 50000);
        assert(std::llabs((local_sec - utc_sec) - expected_delta_sec) <= 1);
    }

    return EXIT_SUCCESS;
}
