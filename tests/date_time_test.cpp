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
        const tz_t offset = 2 * SEC_PER_HOUR;
        const DateTime dt = DateTime::from_components(2024, 3, 15, 10, 20, 30, 400, offset);
        const ts_ms_t offset_ms = sec_to_ms(offset);
        const ts_t local_ts = to_timestamp(2024, 3, 15, 10, 20, 30);

        const DateTime local_start_month = dt.start_of_month();
        const DateTime local_end_month = dt.end_of_month();
        const DateTime local_start_year = dt.start_of_year();
        const DateTime local_end_year = dt.end_of_year();

        expect(local_start_month.unix_ms() == sec_to_ms(start_of_month(local_ts)) - offset_ms, "Local start_of_month should match raw helper");
        expect(local_end_month.unix_ms() == sec_to_ms(end_of_month(local_ts)) + 999 - offset_ms, "Local end_of_month should match raw helper");
        expect(local_start_year.unix_ms() == sec_to_ms(start_of_year(local_ts)) - offset_ms, "Local start_of_year should match raw helper");
        expect(local_end_year.unix_ms() == sec_to_ms(end_of_year(local_ts)) + 999 - offset_ms, "Local end_of_year should match raw helper");

        expect(local_start_month.year() == 2024 && local_start_month.month() == 3 && local_start_month.day() == 1, "Local start_of_month should reset to first day");
        expect(local_start_month.hour() == 0 && local_start_month.minute() == 0 && local_start_month.second() == 0 && local_start_month.millisecond() == 0, "Local start_of_month should reset time of day");
        expect(local_end_month.year() == 2024 && local_end_month.month() == 3 && local_end_month.day() == 31, "Local end_of_month should target last day");
        expect(local_end_month.hour() == 23 && local_end_month.minute() == 59 && local_end_month.second() == 59 && local_end_month.millisecond() == 999, "Local end_of_month should reach end of day");
        expect(local_start_year.year() == 2024 && local_start_year.month() == 1 && local_start_year.day() == 1, "Local start_of_year should reset to first day of year");
        expect(local_end_year.year() == 2024 && local_end_year.month() == 12 && local_end_year.day() == 31, "Local end_of_year should target last day of year");
        expect(local_end_year.hour() == 23 && local_end_year.minute() == 59 && local_end_year.second() == 59 && local_end_year.millisecond() == 999, "Local end_of_year should reach end of day");
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
        expect(!DateTime::try_parse_iso8601("2024-01-02T03:04:05+23:59", parsed), "Unsupported semantic offset should fail to parse");
    }

    {
        DateTime result;
        expect(DateTime::try_from_components(2024, 2, 29, 23, 59, 59, 999, 0, result), "Valid leap day components should parse");
        expect(!DateTime::try_from_components(2023, 2, 29, 0, 0, 0, 0, 0, result), "Invalid leap day should fail");
        expect(!DateTime::try_from_components(2024, 1, 1, 25, 0, 0, 0, 0, result), "Out-of-range hour should fail");
        expect(!DateTime::try_from_components(2024, 1, 1, 0, 0, 0, 0, 15 * SEC_PER_HOUR, result), "Out-of-range semantic offset should fail");
    }

    {
        DateTime result = DateTime::from_components(2024, 1, 1, 12, 0, 0, 0, SEC_PER_HOUR);
        const DateTimeStruct valid_dt = create_date_time_struct(2024, 1, 1, 12, 0, 0, 0);
        const DateTimeStruct invalid_dt = create_date_time_struct(2024, 2, 30, 12, 0, 0, 0);

        expect(DateTime::try_from_date_time_struct(valid_dt, 14 * SEC_PER_HOUR, result), "Valid semantic offset should build from DateTimeStruct");
        expect(!DateTime::try_from_date_time_struct(valid_dt, 15 * SEC_PER_HOUR, result), "Unsupported semantic offset should fail for DateTimeStruct");
        expect(!DateTime::try_from_date_time_struct(invalid_dt, 0, result), "Invalid DateTimeStruct should fail");
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

    {
        const DateTime dt = DateTime::from_components(2024, 1, 1, 1, 30, 15, 250, 2 * SEC_PER_HOUR);
        const DateTimeStruct utc = dt.to_date_time_struct_utc();
        const IsoWeekDateStruct utc_iso = dt.utc_iso_week_date();

        expect(dt.utc_year() == utc.year, "UTC year getter should match UTC structure");
        expect(dt.utc_month() == utc.mon, "UTC month getter should match UTC structure");
        expect(dt.utc_day() == utc.day, "UTC day getter should match UTC structure");
        expect(dt.utc_hour() == utc.hour, "UTC hour getter should match UTC structure");
        expect(dt.utc_minute() == utc.min, "UTC minute getter should match UTC structure");
        expect(dt.utc_second() == utc.sec, "UTC second getter should match UTC structure");
        expect(dt.utc_millisecond() == utc.ms, "UTC millisecond getter should match UTC structure");
        expect(dt.utc_weekday() == weekday_of_date<Weekday>(dt.utc_date()), "UTC weekday should match UTC date");
        expect(dt.utc_iso_weekday() == iso_weekday_of_date(utc.year, utc.mon, utc.day), "UTC ISO weekday should match conversion helper");
        expect(utc_iso.year == 2023 && utc_iso.week == 52 && utc_iso.weekday == 7, "UTC ISO week date should match expected boundary value");
        expect(!dt.utc_is_workday(), "UTC weekend helper should report non-workday");
        expect(dt.utc_is_weekend(), "UTC weekend helper should report weekend");

        const DateTime start_day = dt.start_of_utc_day();
        const DateTime end_day = dt.end_of_utc_day();
        const DateTime start_month = dt.start_of_utc_month();
        const DateTime end_month = dt.end_of_utc_month();
        const DateTime start_year = dt.start_of_utc_year();
        const DateTime end_year = dt.end_of_utc_year();

        expect(start_day.to_iso8601_utc() == "2023-12-31T00:00:00.000Z", "UTC start_of_day should use UTC boundary");
        expect(end_day.to_iso8601_utc() == "2023-12-31T23:59:59.999Z", "UTC end_of_day should use UTC boundary");
        expect(start_month.to_iso8601_utc() == "2023-12-01T00:00:00.000Z", "UTC start_of_month should use UTC boundary");
        expect(end_month.to_iso8601_utc() == "2023-12-31T23:59:59.999Z", "UTC end_of_month should use UTC boundary");
        expect(start_year.to_iso8601_utc() == "2023-01-01T00:00:00.000Z", "UTC start_of_year should use UTC boundary");
        expect(end_year.to_iso8601_utc() == "2023-12-31T23:59:59.999Z", "UTC end_of_year should use UTC boundary");
        expect(start_day.utc_offset() == dt.utc_offset(), "UTC boundary helpers should preserve stored offset");
    }

    return all_ok ? 0 : 1;
}
