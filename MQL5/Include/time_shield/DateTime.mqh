//+------------------------------------------------------------------+
//|                                                     DateTime.mqh |
//|                                Time Shield - MQL5 DateTime Type |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_DATE_TIME_MQH__
#define __TIME_SHIELD_DATE_TIME_MQH__

/// \file DateTime.mqh
/// \ingroup mql5
/// \brief Lightweight date-time wrapper for MQL5 with fixed UTC offset.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include "time_conversions.mqh"
#include "time_formatting.mqh"
#include "time_parser.mqh"
#include "time_zone_struct.mqh"
#include "validation.mqh"

namespace time_shield {

    /// \brief Represents a UTC timestamp with an optional fixed offset.
    class DateTime {
    public:
        /// \brief Default constructor initializes epoch with zero offset.
        DateTime(): m_utc_ms(0), m_offset(0) {}

        /// \brief Create instance from UTC milliseconds.
        /// \param utc_ms Timestamp in milliseconds since the Unix epoch (UTC).
        /// \param offset Fixed UTC offset in seconds.
        /// \return Constructed DateTime.
        static DateTime from_unix_ms(const long utc_ms, const int offset = 0) {
            return DateTime(utc_ms, offset);
        }

        /// \brief Construct instance for current UTC time.
        /// \param offset Fixed UTC offset in seconds.
        /// \return DateTime set to now.
        static DateTime now_utc(const int offset = 0) {
            return DateTime(ts_ms(), offset);
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
                const long year,
                const int month,
                const int day,
                const int hour,
                const int min,
                const int sec,
                const int ms,
                const int offset,
                DateTime &out) {
            if (!is_valid_date_time(year, month, day, hour, min, sec, ms))
                return false;

            TimeZoneStruct tz = to_time_zone_struct(offset);
            if (!is_valid_time_zone_offset(tz))
                return false;

            const long local_ms = to_timestamp_ms(year, month, day, hour, min, sec, ms);
            out = DateTime(local_ms - offset_to_ms(offset), offset);
            return true;
        }

        /// \brief Try to build from DateTimeStruct interpreted in provided offset.
        /// \param local_dt Local date-time structure.
        /// \param offset Fixed UTC offset in seconds.
        /// \param out Output DateTime on success.
        /// \return True when structure and offset are valid.
        static bool try_from_date_time_struct(
                const DateTimeStruct &local_dt,
                const int offset,
                DateTime &out) {
            if (!is_valid_date_time(local_dt))
                return false;

            TimeZoneStruct tz = to_time_zone_struct(offset);
            if (!is_valid_time_zone_offset(tz))
                return false;

            const long local_ms = dt_to_timestamp_ms(local_dt);
            out = DateTime(local_ms - offset_to_ms(offset), offset);
            return true;
        }

        /// \brief Try to parse ISO8601 string to DateTime.
        /// \param str Input ISO8601 string.
        /// \param out Output DateTime when parsing succeeds.
        /// \return True on success.
        static bool try_parse_iso8601(const string str, DateTime &out) {
            DateTimeStruct dt = create_date_time_struct(0);
            TimeZoneStruct tz = create_time_zone_struct(0, 0, true);
            if (!parse_iso8601(str, dt, tz))
                return false;

            out = DateTime(dt_to_timestamp_ms(dt) - offset_to_ms(time_zone_struct_to_offset(tz)),
                           time_zone_struct_to_offset(tz));
            return true;
        }

        /// \brief Format to ISO8601 string with stored offset.
        string to_iso8601() const { return to_iso8601_ms(local_ms(), m_offset); }

        /// \brief Access UTC milliseconds.
        long unix_ms() const { return m_utc_ms; }

        /// \brief Access stored UTC offset.
        int utc_offset() const { return m_offset; }

        /// \brief Return copy with new offset preserving instant.
        DateTime with_offset(const int new_offset) const { return DateTime(m_utc_ms, new_offset); }

        /// \brief Return copy with zero offset.
        DateTime to_utc() const { return with_offset(0); }

    private:
        DateTime(const long utc_ms, const int offset): m_utc_ms(utc_ms), m_offset(offset) {}

        static long offset_to_ms(const int offset) { return (long)offset * MS_PER_SEC; }

        long local_ms() const { return m_utc_ms + offset_to_ms(m_offset); }

        long m_utc_ms;
        int m_offset;
    };

} // namespace time_shield

#endif // __TIME_SHIELD_DATE_TIME_MQH__
