#include <time_shield.hpp>

#include <cassert>
#include <string>

/// \brief Tests for DateTime wrapper utilities.
int main() {
    using namespace time_shield;

    {
        DateTime parsed;
        const std::string input = "2025-12-16T10:20:30.123+02:30";
        assert(DateTime::try_parse_iso8601(input, parsed));
        assert(parsed.to_iso8601() == input);
    }

    {
        const DateTime utc = DateTime::from_components(2025, 1, 1, 0, 0, 0, 0, 0);
        const DateTime berlin = DateTime::from_components(2025, 1, 1, 1, 0, 0, 0, SEC_PER_HOUR);
        assert(utc.unix_ms() == berlin.unix_ms());
        assert(utc == berlin);
    }

    {
        const DateTime shifted_epoch = DateTime::from_components(1970, 1, 1, 0, 0, 0, 0, SEC_PER_HOUR);
        assert(shifted_epoch.unix_ms() == -sec_to_ms(SEC_PER_HOUR));
    }

    {
        const DateTime sample = DateTime::from_components(2024, 5, 15, 12, 0, 0, 0, SEC_PER_HOUR);
        const IsoWeekDateStruct iso = sample.iso_week_date();
        const DateTime restored = DateTime::from_iso_week_date(iso, 12, 0, 0, 0, SEC_PER_HOUR);
        assert(restored.year() == sample.year());
        assert(restored.month() == sample.month());
        assert(restored.day() == sample.day());
    }

    {
        const DateTime dt = DateTime::from_components(2025, 3, 14, 15, 9, 26, 500, 2 * SEC_PER_HOUR);
        const DateTime start = dt.start_of_day();
        assert(start.hour() == 0);
        assert(start.minute() == 0);
        assert(start.second() == 0);
        assert(start.millisecond() == 0);
        assert(start.unix_ms() <= dt.unix_ms());
    }

    {
        DateTime parsed_z;
        DateTime parsed_plus;
        DateTime parsed_negative;
        assert(DateTime::try_parse_iso8601("2024-01-02T03:04:05Z", parsed_z));
        assert(DateTime::try_parse_iso8601("2024-01-02T03:04:05+00:00", parsed_plus));
        assert(DateTime::try_parse_iso8601("2024-01-02T21:34:05-05:30", parsed_negative));
        assert(parsed_z.unix_ms() == parsed_plus.unix_ms());
        assert(parsed_z.utc_offset() == 0);
        assert(parsed_negative.utc_offset() == -(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN));
    }

    {
        DateTime parsed;
        assert(DateTime::try_parse_iso8601("2024-07-01T12:00:00-05:30", parsed));
        const DateTime reparsed = DateTime::parse_iso8601(parsed.to_iso8601());
        assert(parsed == reparsed);
        assert(parsed.utc_offset() == reparsed.utc_offset());
    }

    {
        DateTime parsed;
        assert(DateTime::try_parse_iso8601("2024-02-29", parsed));
        assert(parsed.hour() == 0);
        assert(parsed.minute() == 0);
        assert(parsed.second() == 0);
    }

    {
        DateTime parsed;
        assert(!DateTime::try_parse_iso8601("invalid-date", parsed));
    }

    {
        DateTime result;
        assert(DateTime::try_from_components(2024, 2, 29, 23, 59, 59, 999, 0, result));
        assert(!DateTime::try_from_components(2023, 2, 29, 0, 0, 0, 0, 0, result));
        assert(!DateTime::try_from_components(2024, 1, 1, 25, 0, 0, 0, 0, result));
        assert(!DateTime::try_from_components(2024, 1, 1, 0, 0, 0, 0, 30 * SEC_PER_HOUR, result));
    }

    {
        const DateTime iso_boundary = DateTime::from_components(2018, 12, 31);
        const IsoWeekDateStruct iso = iso_boundary.iso_week_date();
        const DateTime restored = DateTime::from_iso_week_date(iso);
        assert(restored.year() == 2018);
        assert(restored.month() == 12);
        assert(restored.day() == 31);
    }

    {
        const DateTime utc = DateTime::from_components(2025, 6, 1, 10, 0, 0, 0, 0);
        const DateTime shifted = utc.with_offset(SEC_PER_HOUR);
        assert(utc == shifted);
        assert(!utc.same_local(shifted));
    }

    return 0;
}
