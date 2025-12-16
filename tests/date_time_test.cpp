#include <time_shield.hpp>

#include <iostream>
#include <string>

/// \brief Tests for DateTime wrapper utilities.
int main() {
    using namespace time_shield;

    bool all_ok = true;
    auto expect = [&](bool condition, const char* message) {
        if (!condition) {
            std::cerr << message << std::endl;
            all_ok = false;
        }
    };

    {
        DateTime parsed;
        const std::string input = "2025-12-16T10:20:30.123+02:30";
        if (DateTime::try_parse_iso8601(input, parsed)) {
            expect(parsed.to_iso8601() == input, "Parsed ISO8601 should round-trip");
        } else {
            expect(false, "Failed to parse expected ISO8601 string");
        }
    }

    {
        const DateTime utc = DateTime::from_components(2025, 1, 1, 0, 0, 0, 0, 0);
        const DateTime berlin = DateTime::from_components(2025, 1, 1, 1, 0, 0, 0, SEC_PER_HOUR);
        expect(utc.unix_ms() == berlin.unix_ms(), "UTC and offset-adjusted instant must match");
        expect(utc == berlin, "Equality should compare UTC instant");
    }

    {
        const DateTime shifted_epoch = DateTime::from_components(1970, 1, 1, 0, 0, 0, 0, SEC_PER_HOUR);
        expect(shifted_epoch.unix_ms() == -sec_to_ms(SEC_PER_HOUR), "Offset should shift epoch");
    }

    {
        const DateTime sample = DateTime::from_components(2024, 5, 15, 12, 0, 0, 0, SEC_PER_HOUR);
        const IsoWeekDateStruct iso = sample.iso_week_date();
        const DateTime restored = DateTime::from_iso_week_date(iso, 12, 0, 0, 0, SEC_PER_HOUR);
        expect(restored.year() == sample.year(), "ISO week-year conversion should preserve year");
        expect(restored.month() == sample.month(), "ISO week-year conversion should preserve month");
        expect(restored.day() == sample.day(), "ISO week-year conversion should preserve day");
    }

    {
        const DateTime dt = DateTime::from_components(2025, 3, 14, 15, 9, 26, 500, 2 * SEC_PER_HOUR);
        const DateTime start = dt.start_of_day();
        expect(start.hour() == 0, "Start of day should reset hour");
        expect(start.minute() == 0, "Start of day should reset minute");
        expect(start.second() == 0, "Start of day should reset second");
        expect(start.millisecond() == 0, "Start of day should reset millisecond");
        expect(start.unix_ms() <= dt.unix_ms(), "Start of day should not exceed original instant");
    }

    {
        DateTime parsed_z;
        DateTime parsed_plus;
        DateTime parsed_negative;
        expect(DateTime::try_parse_iso8601("2024-01-02T03:04:05Z", parsed_z), "Should parse Zulu offset");
        expect(DateTime::try_parse_iso8601("2024-01-02T03:04:05+00:00", parsed_plus), "Should parse +00:00 offset");
        expect(DateTime::try_parse_iso8601("2024-01-02T21:34:05-05:30", parsed_negative), "Should parse negative offset");
        expect(parsed_z.unix_ms() == parsed_plus.unix_ms(), "Equivalent zero-offset instants should match");
        expect(parsed_z.utc_offset() == 0, "Zulu offset should be zero");
        expect(parsed_negative.utc_offset() == -(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN), "Parsed offset should match -05:30");
    }

    {
        DateTime parsed;
        if (DateTime::try_parse_iso8601("2024-07-01T12:00:00-05:30", parsed)) {
            const DateTime reparsed = DateTime::parse_iso8601(parsed.to_iso8601());
            expect(parsed == reparsed, "Round-trip parse should preserve instant");
            expect(parsed.utc_offset() == reparsed.utc_offset(), "Round-trip parse should preserve offset");
        } else {
            expect(false, "Should parse offset date");
        }
    }

    {
        DateTime parsed;
        if (DateTime::try_parse_iso8601("2024-02-29", parsed)) {
            expect(parsed.hour() == 0, "Date-only parse should default hour");
            expect(parsed.minute() == 0, "Date-only parse should default minute");
            expect(parsed.second() == 0, "Date-only parse should default second");
        } else {
            expect(false, "Leap-day parse should succeed");
        }
    }

    {
        DateTime parsed;
        expect(!DateTime::try_parse_iso8601("invalid-date", parsed), "Invalid ISO should fail to parse");
    }

    {
        DateTime result;
        expect(DateTime::try_from_components(2024, 2, 29, 23, 59, 59, 999, 0, result), "Valid leap day components should parse");
        expect(!DateTime::try_from_components(2023, 2, 29, 0, 0, 0, 0, 0, result), "Invalid leap day should fail");
        expect(!DateTime::try_from_components(2024, 1, 1, 25, 0, 0, 0, 0, result), "Out-of-range hour should fail");
        expect(!DateTime::try_from_components(2024, 1, 1, 0, 0, 0, 0, 30 * SEC_PER_HOUR, result), "Out-of-range offset should fail");
    }

    {
        const DateTime iso_boundary = DateTime::from_components(2018, 12, 31);
        const IsoWeekDateStruct iso = iso_boundary.iso_week_date();
        const DateTime restored = DateTime::from_iso_week_date(iso);
        expect(restored.year() == 2018, "ISO boundary conversion should preserve year");
        expect(restored.month() == 12, "ISO boundary conversion should preserve month");
        expect(restored.day() == 31, "ISO boundary conversion should preserve day");
    }

    {
        const DateTime utc = DateTime::from_components(2025, 6, 1, 10, 0, 0, 0, 0);
        const DateTime shifted = utc.with_offset(SEC_PER_HOUR);
        expect(utc == shifted, "Offset change should keep instant equal");
        expect(!utc.same_local(shifted), "Offset change should alter local representation");
    }

    return all_ok ? 0 : 1;
}
