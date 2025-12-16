// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_DATE_TIME_HPP_INCLUDED
#define TIME_SHIELD_DATE_TIME_HPP_INCLUDED

/// \file DateTime.hpp
/// \brief Value-type wrapper for timestamps with fixed UTC offset.

#include "config.hpp"
#include "constants.hpp"
#include "date_conversions.hpp"
#include "date_time_conversions.hpp"
#include "date_time_struct.hpp"
#include "iso_week_conversions.hpp"
#include "time_formatting.hpp"
#include "time_parser.hpp"
#include "time_struct.hpp"
#include "time_utils.hpp"
#include "time_zone_struct.hpp"
#include "types.hpp"
#include "validation.hpp"

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#ifdef TIME_SHIELD_CPP17
#include <string_view>
#endif

namespace time_shield {

    /// \brief Represents a moment in time with optional fixed UTC offset.
    ///
    /// Equality and ordering compare the UTC instant only and ignore the stored offset.
    class DateTime {
    public:
        /// \brief Default constructor sets epoch with zero offset.
        DateTime() noexcept
            : m_utc_ms(0)
            , m_offset(0) {}

        /// \brief Create instance from UTC milliseconds.
        /// \param utc_ms Timestamp in milliseconds since Unix epoch (UTC).
        /// \param offset Fixed UTC offset in seconds.
        /// \return Constructed DateTime.
        static DateTime from_unix_ms(ts_ms_t utc_ms, tz_t offset = 0) noexcept {
            return DateTime(utc_ms, offset);
        }

        /// \brief Create instance from UTC seconds.
        /// \param utc_s Timestamp in seconds since Unix epoch (UTC).
        /// \param offset Fixed UTC offset in seconds.
        /// \return Constructed DateTime.
        static DateTime from_unix_s(ts_t utc_s, tz_t offset = 0) noexcept {
            return DateTime(sec_to_ms(utc_s), offset);
        }

        /// \brief Construct instance for current UTC time.
        /// \param offset Fixed UTC offset in seconds.
        /// \return DateTime set to now.
        static DateTime now_utc(tz_t offset = 0) noexcept {
            return DateTime(ts_ms(), offset);
        }

        /// \brief Build from calendar components interpreted in provided offset.
        static DateTime from_components(
                year_t year,
                int month,
                int day,
                int hour = 0,
                int min = 0,
                int sec = 0,
                int ms = 0,
                tz_t offset = 0) {
            const ts_ms_t local_ms = to_timestamp_ms(year, month, day, hour, min, sec, ms);
            const ts_ms_t utc_ms = local_ms - offset_to_ms(offset);
            return DateTime(utc_ms, offset);
        }

        /// \brief Try to build from calendar components interpreted in provided offset.
        /// \param year Year component.
        /// \param month Month component.
        /// \param day Day component.
        /// \param hour Hour component.
        /// \param min Minute component.
        /// \param sec Second component.
        /// \param ms Millisecond component.
        /// \param offset Fixed UTC offset in seconds.
        /// \param out Output DateTime on success.
        /// \return True when components form a valid date-time and offset.
        static bool try_from_components(
                year_t year,
                int month,
                int day,
                int hour,
                int min,
                int sec,
                int ms,
                tz_t offset,
                DateTime& out) noexcept {
            if (!is_valid_date_time(year, month, day, hour, min, sec, ms)) {
                return false;
            }
            const TimeZoneStruct tz = to_time_zone_struct(offset);
            if (!is_valid_time_zone_offset(tz)) {
                return false;
            }
            const ts_ms_t local_ms = to_timestamp_ms(year, month, day, hour, min, sec, ms);
            out = DateTime(local_ms - offset_to_ms(offset), offset);
            return true;
        }

        /// \brief Build from DateTimeStruct interpreted in provided offset.
        static DateTime from_date_time_struct(const DateTimeStruct& local_dt, tz_t offset = 0) {
            const ts_ms_t local_ms = dt_to_timestamp_ms(local_dt);
            const ts_ms_t utc_ms = local_ms - offset_to_ms(offset);
            return DateTime(utc_ms, offset);
        }

        /// \brief Try to build from DateTimeStruct interpreted in provided offset.
        /// \param local_dt Local date-time structure.
        /// \param offset Fixed UTC offset in seconds.
        /// \param out Output DateTime on success.
        /// \return True when structure and offset are valid.
        static bool try_from_date_time_struct(
                const DateTimeStruct& local_dt,
                tz_t offset,
                DateTime& out) noexcept {
            if (!is_valid_date_time(local_dt)) {
                return false;
            }
            const TimeZoneStruct tz = to_time_zone_struct(offset);
            if (!is_valid_time_zone_offset(tz)) {
                return false;
            }
            const ts_ms_t local_ms = dt_to_timestamp_ms(local_dt);
            out = DateTime(local_ms - offset_to_ms(offset), offset);
            return true;
        }

        /// \brief Convert to date-time structure using stored offset.
        DateTimeStruct to_date_time_struct_local() const {
            return to_date_time_ms<DateTimeStruct>(local_ms());
        }

        /// \brief Convert to UTC date-time structure.
        DateTimeStruct to_date_time_struct_utc() const {
            return to_date_time_ms<DateTimeStruct>(m_utc_ms);
        }

        /// \brief Build instance from ISO week date interpreted in provided offset.
        static DateTime from_iso_week_date(
                const IsoWeekDateStruct& iso,
                int hour = 0,
                int min = 0,
                int sec = 0,
                int ms = 0,
                tz_t offset = 0) {
            const DateStruct date = iso_week_date_to_date(iso);
            return from_components(date.year, date.mon, date.day, hour, min, sec, ms, offset);
        }

        /// \brief Try to parse ISO8601 string to DateTime.
        /// \param str Input ISO8601 string.
        /// \param out Output DateTime when parsing succeeds.
        /// \return True on success.
        static bool try_parse_iso8601(const std::string& str, DateTime& out) noexcept {
            return try_parse_iso8601_buffer(str.data(), str.size(), out);
        }

        #ifdef TIME_SHIELD_CPP17
        /// \brief Try to parse ISO8601 string_view to DateTime.
        /// \param str Input ISO8601 string_view.
        /// \param out Output DateTime when parsing succeeds.
        /// \return True on success.
        static bool try_parse_iso8601(std::string_view str, DateTime& out) noexcept {
            return try_parse_iso8601_buffer(str.data(), str.size(), out);
        }
        #endif

        /// \brief Try to parse ISO8601 C-string to DateTime.
        /// \param str Null-terminated ISO8601 string.
        /// \param out Output DateTime when parsing succeeds.
        /// \return True on success.
        static bool try_parse_iso8601(const char* str, DateTime& out) noexcept {
            if (str == nullptr) {
                return false;
            }
            return try_parse_iso8601_buffer(str, std::strlen(str), out);
        }

        /// \brief Parse ISO8601 string, throws on failure.
        /// \param str Input ISO8601 string.
        /// \return Parsed DateTime.
        static DateTime parse_iso8601(const std::string& str) {
            return parse_iso8601_buffer(str.data(), str.size());
        }

        #ifdef TIME_SHIELD_CPP17
        /// \brief Parse ISO8601 string_view, throws on failure.
        /// \param str Input ISO8601 view.
        /// \return Parsed DateTime.
        static DateTime parse_iso8601(std::string_view str) {
            return parse_iso8601_buffer(str.data(), str.size());
        }
        #endif

        /// \brief Parse ISO8601 C-string, throws on failure.
        /// \param str Null-terminated ISO8601 string.
        /// \return Parsed DateTime.
        static DateTime parse_iso8601(const char* str) {
            if (str == nullptr) {
                throw std::invalid_argument("Invalid ISO8601 datetime");
            }
            return parse_iso8601_buffer(str, std::strlen(str));
        }

        /// \brief Try to parse ISO week-date string.
        /// \param str Input ISO week-date string.
        /// \param iso Output ISO week-date structure.
        /// \return True on success.
        static bool try_parse_iso_week_date(const std::string& str, IsoWeekDateStruct& iso) noexcept {
            return parse_iso_week_date(str.data(), str.size(), iso);
        }

        #ifdef TIME_SHIELD_CPP17
        /// \brief Try to parse ISO week-date string_view.
        static bool try_parse_iso_week_date(std::string_view str, IsoWeekDateStruct& iso) noexcept {
            return parse_iso_week_date(str.data(), str.size(), iso);
        }
        #endif

        /// \brief Try to parse ISO week-date C-string.
        static bool try_parse_iso_week_date(const char* str, IsoWeekDateStruct& iso) noexcept {
            if (str == nullptr) {
                return false;
            }
            return parse_iso_week_date(str, std::strlen(str), iso);
        }

        /// \brief Format to ISO8601 string with stored offset.
        std::string to_iso8601() const {
            return to_iso8601_ms(local_ms(), m_offset);
        }

        /// \brief Format to ISO8601 string in UTC.
        std::string to_iso8601_utc() const {
            return to_iso8601_utc_ms(m_utc_ms);
        }

        /// \brief Format using custom pattern.
        std::string format(const std::string& fmt) const {
            return to_string_ms(fmt, local_ms(), m_offset);
        }

        #ifdef TIME_SHIELD_CPP17
        /// \brief Format using custom string_view pattern.
        std::string format(std::string_view fmt) const {
            return to_string_ms(std::string(fmt), local_ms(), m_offset);
        }
        #endif

        /// \brief Format using C-string pattern.
        std::string format(const char* fmt) const {
            if (fmt == nullptr) {
                return std::string();
            }
            return to_string_ms(std::string(fmt), local_ms(), m_offset);
        }

        /// \brief Format to MQL5 date-time string.
        std::string to_mql5_date_time() const {
            return time_shield::to_mql5_date_time(ms_to_sec<ts_t>(local_ms()));
        }

        /// \brief Access UTC milliseconds.
        ts_ms_t unix_ms() const noexcept {
            return m_utc_ms;
        }

        /// \brief Access UTC seconds.
        ts_t unix_s() const noexcept {
            return ms_to_sec<ts_t>(m_utc_ms);
        }

        /// \brief Access stored UTC offset.
        tz_t utc_offset() const noexcept {
            return m_offset;
        }

        /// \brief Get timezone structure from offset.
        TimeZoneStruct time_zone() const {
            return to_time_zone_struct(m_offset);
        }

        /// \brief Local year component.
        year_t year() const {
            return to_date_time_struct_local().year;
        }

        /// \brief Local month component.
        int month() const {
            return to_date_time_struct_local().mon;
        }

        /// \brief Local day component.
        int day() const {
            return to_date_time_struct_local().day;
        }

        /// \brief Local hour component.
        int hour() const {
            return to_date_time_struct_local().hour;
        }

        /// \brief Local minute component.
        int minute() const {
            return to_date_time_struct_local().min;
        }

        /// \brief Local second component.
        int second() const {
            return to_date_time_struct_local().sec;
        }

        /// \brief Local millisecond component.
        int millisecond() const {
            return to_date_time_struct_local().ms;
        }

        /// \brief Local date components.
        DateStruct date() const {
            const DateTimeStruct local_dt = to_date_time_struct_local();
            return create_date_struct(local_dt.year, local_dt.mon, local_dt.day);
        }

        /// \brief Local time-of-day components.
        TimeStruct time_of_day() const {
            const DateTimeStruct local_dt = to_date_time_struct_local();
            return create_time_struct(
                    static_cast<int16_t>(local_dt.hour),
                    static_cast<int16_t>(local_dt.min),
                    static_cast<int16_t>(local_dt.sec),
                    static_cast<int16_t>(local_dt.ms));
        }

        /// \brief UTC date components.
        DateStruct utc_date() const {
            const DateTimeStruct utc_dt = to_date_time_struct_utc();
            return create_date_struct(utc_dt.year, utc_dt.mon, utc_dt.day);
        }

        /// \brief UTC time-of-day components.
        TimeStruct utc_time_of_day() const {
            const DateTimeStruct utc_dt = to_date_time_struct_utc();
            return create_time_struct(
                    static_cast<int16_t>(utc_dt.hour),
                    static_cast<int16_t>(utc_dt.min),
                    static_cast<int16_t>(utc_dt.sec),
                    static_cast<int16_t>(utc_dt.ms));
        }

        /// \brief Local weekday.
        Weekday weekday() const {
            const DateStruct local_date = date();
            return weekday_of_date<Weekday>(local_date);
        }

        /// \brief Local ISO weekday number (1..7).
        int iso_weekday() const {
            const DateStruct local_date = date();
            return iso_weekday_of_date(local_date.year, local_date.mon, local_date.day);
        }

        /// \brief Local ISO week date.
        IsoWeekDateStruct iso_week_date() const {
            const DateStruct local_date = date();
            return to_iso_week_date(local_date.year, local_date.mon, local_date.day);
        }

        /// \brief Check if local date is a workday.
        bool is_workday() const noexcept {
            return is_workday_ms(local_ms());
        }

        /// \brief Check if local date is a weekend.
        bool is_weekend() const noexcept {
            return time_shield::is_weekend(ms_to_sec<ts_t>(local_ms()));
        }

        /// \brief Compare equality by UTC instant.
        bool operator==(const DateTime& other) const noexcept {
            return m_utc_ms == other.m_utc_ms;
        }

        /// \brief Compare inequality by UTC instant.
        bool operator!=(const DateTime& other) const noexcept {
            return !(*this == other);
        }

        /// \brief Less-than comparison by UTC instant.
        bool operator<(const DateTime& other) const noexcept {
            return m_utc_ms < other.m_utc_ms;
        }

        /// \brief Less-than-or-equal comparison by UTC instant.
        bool operator<=(const DateTime& other) const noexcept {
            return m_utc_ms <= other.m_utc_ms;
        }

        /// \brief Greater-than comparison by UTC instant.
        bool operator>(const DateTime& other) const noexcept {
            return m_utc_ms > other.m_utc_ms;
        }

        /// \brief Greater-than-or-equal comparison by UTC instant.
        bool operator>=(const DateTime& other) const noexcept {
            return m_utc_ms >= other.m_utc_ms;
        }

        /// \brief Check if local representations match including offset.
        bool same_local(const DateTime& other) const noexcept {
            return local_ms() == other.local_ms() && m_offset == other.m_offset;
        }

        /// \brief Add milliseconds to UTC instant.
        DateTime add_ms(int64_t delta_ms) const noexcept {
            return DateTime(m_utc_ms + delta_ms, m_offset);
        }

        /// \brief Add seconds to UTC instant.
        DateTime add_seconds(int64_t seconds) const noexcept {
            return add_ms(sec_to_ms(seconds));
        }

        /// \brief Add minutes to UTC instant.
        DateTime add_minutes(int64_t minutes) const noexcept {
            return add_ms(sec_to_ms(minutes * SEC_PER_MIN));
        }

        /// \brief Add hours to UTC instant.
        DateTime add_hours(int64_t hours) const noexcept {
            return add_ms(sec_to_ms(hours * SEC_PER_HOUR));
        }

        /// \brief Add days to UTC instant.
        DateTime add_days(int64_t days) const noexcept {
            return add_ms(days * MS_PER_DAY);
        }

        /// \brief Difference in milliseconds to another DateTime.
        int64_t diff_ms(const DateTime& other) const noexcept {
            return m_utc_ms - other.m_utc_ms;
        }

        /// \brief Difference in seconds to another DateTime.
        double diff_seconds(const DateTime& other) const noexcept {
            return static_cast<double>(diff_ms(other)) / static_cast<double>(MS_PER_SEC);
        }

        /// \brief Return copy with new offset preserving instant.
        DateTime with_offset(tz_t new_offset) const noexcept {
            return DateTime(m_utc_ms, new_offset);
        }

        /// \brief Return copy with zero offset.
        DateTime to_utc() const noexcept {
            return with_offset(0);
        }

        /// \brief Start of local day.
        DateTime start_of_day() const {
            const DateTimeStruct local_dt = to_date_time_struct_local();
            const ts_ms_t local_start_ms = to_timestamp_ms(local_dt.year, local_dt.mon, local_dt.day);
            return from_unix_ms(local_start_ms - offset_to_ms(m_offset), m_offset);
        }

        /// \brief End of local day.
        DateTime end_of_day() const {
            const DateTimeStruct local_dt = to_date_time_struct_local();
            const ts_ms_t local_end_ms = to_timestamp_ms(
                    local_dt.year,
                    local_dt.mon,
                    local_dt.day,
                    23,
                    59,
                    59,
                    static_cast<int>(MS_PER_SEC - 1));
            return from_unix_ms(local_end_ms - offset_to_ms(m_offset), m_offset);
        }

        /// \brief Start of local month.
        DateTime start_of_month() const {
            const DateTimeStruct local_dt = to_date_time_struct_local();
            const ts_ms_t local_start_ms = to_timestamp_ms(local_dt.year, local_dt.mon, 1);
            return from_unix_ms(local_start_ms - offset_to_ms(m_offset), m_offset);
        }

        /// \brief End of local month.
        DateTime end_of_month() const {
            const DateTimeStruct local_dt = to_date_time_struct_local();
            const int days = num_days_in_month(local_dt.year, local_dt.mon);
            const ts_ms_t local_end_ms = to_timestamp_ms(
                    local_dt.year,
                    local_dt.mon,
                    days,
                    23,
                    59,
                    59,
                    static_cast<int>(MS_PER_SEC - 1));
            return from_unix_ms(local_end_ms - offset_to_ms(m_offset), m_offset);
        }

        /// \brief Start of local year.
        DateTime start_of_year() const {
            const year_t local_year = year();
            const ts_ms_t local_start_ms = to_timestamp_ms(local_year, 1, 1);
            return from_unix_ms(local_start_ms - offset_to_ms(m_offset), m_offset);
        }

        /// \brief End of local year.
        DateTime end_of_year() const {
            const year_t local_year = year();
            const ts_ms_t local_end_ms = to_timestamp_ms(
                    local_year,
                    12,
                    31,
                    23,
                    59,
                    59,
                    static_cast<int>(MS_PER_SEC - 1));
            return from_unix_ms(local_end_ms - offset_to_ms(m_offset), m_offset);
        }

    private:
        static bool try_parse_iso8601_buffer(const char* data, std::size_t size, DateTime& out) noexcept {
            if (data == nullptr) {
                return false;
            }
            DateTimeStruct dt = create_date_time_struct(0);
            TimeZoneStruct tz = create_time_zone_struct(0, 0, true);
            if (!time_shield::parse_iso8601(data, size, dt, tz)) {
                return false;
            }
            const tz_t offset = time_zone_struct_to_offset(tz);
            const ts_ms_t utc_ms = dt_to_timestamp_ms(dt) - offset_to_ms(offset);
            out = from_unix_ms(utc_ms, offset);
            return true;
        }

        static DateTime parse_iso8601_buffer(const char* data, std::size_t size) {
            DateTime result;
            if (!try_parse_iso8601_buffer(data, size, result)) {
                throw std::invalid_argument("Invalid ISO8601 datetime");
            }
            return result;
        }

        DateTime(ts_ms_t utc_ms, tz_t offset) noexcept
            : m_utc_ms(utc_ms)
            , m_offset(offset) {}

        static constexpr ts_ms_t offset_to_ms(tz_t offset) noexcept {
            return static_cast<ts_ms_t>(offset) * MS_PER_SEC;
        }

        ts_ms_t local_ms() const noexcept {
            return m_utc_ms + offset_to_ms(m_offset);
        }

        ts_ms_t m_utc_ms;
        tz_t m_offset;
    };

} // namespace time_shield

#endif // TIME_SHIELD_DATE_TIME_HPP_INCLUDED
