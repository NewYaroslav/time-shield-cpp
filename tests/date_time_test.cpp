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

    return 0;
}
