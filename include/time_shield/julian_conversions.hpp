// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_JULIAN_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_JULIAN_CONVERSIONS_HPP_INCLUDED

/// \file julian_conversions.hpp
/// \brief Julian Date / MJD / JDN helpers using the proleptic Gregorian calendar.
/// \ingroup time_conversions
///
/// JD epoch used here:
/// - Unix epoch (1970-01-01 00:00:00 UTC) is JD 2440587.5
///
/// Notes:
/// - JD and MJD are returned as double (jd_t/mjd_t).
/// - These functions are intended for utility/analytics, not for high-precision astronomy.
/// - Legacy `gregorian_to_*` overloads use `day, month, year` order for compatibility.

#include "config.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "validation.hpp"

#include <cmath>
#include <cstdint>

namespace time_shield {

    namespace detail {

        inline jd_t gregorian_dmy_to_jd_unchecked(double day, int64_t month, int64_t year) noexcept {
            if (month == 1 || month == 2) {
                year -= 1;
                month += 12;
            }
            const double a = std::floor(static_cast<double>(year) / 100.0);
            const double b = 2.0 - a + std::floor(a / 4.0);
            const double jd = std::floor(365.25 * (static_cast<double>(year) + 4716.0))
                            + std::floor(30.6000001 * (static_cast<double>(month) + 1.0))
                            + day + b - 1524.5;
            return static_cast<jd_t>(jd);
        }

        inline jdn_t gregorian_dmy_to_jdn_unchecked(int64_t day, int64_t month, int64_t year) noexcept {
            const int64_t a = (14LL - month) / 12LL;
            const int64_t y = year + 4800LL - a;
            const int64_t m = month + 12LL * a - 3LL;
            const int64_t jdn = day
                              + (153LL * m + 2LL) / 5LL
                              + 365LL * y
                              + y / 4LL
                              - y / 100LL
                              + y / 400LL
                              - 32045LL;
            return static_cast<jdn_t>(jdn);
        }

        inline double day_fraction_from_hms(
                int hour,
                int minute,
                int second,
                int millisecond) noexcept {
            return (static_cast<double>(hour) / 24.0) +
                   (static_cast<double>(minute) / (24.0 * 60.0)) +
                   ((static_cast<double>(second) + static_cast<double>(millisecond) / 1000.0)
                     / static_cast<double>(SEC_PER_DAY));
        }

    } // namespace detail

    /// \brief Convert Unix timestamp (floating seconds) to Julian Date (JD).
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Julian Date value.
    inline jd_t fts_to_jd(fts_t ts) noexcept {
        return static_cast<jd_t>(2440587.5)
             + static_cast<jd_t>(ts) / static_cast<jd_t>(SEC_PER_DAY);
    }

    /// \brief Convert Unix timestamp (seconds) to Julian Date (JD).
    /// \param ts Unix timestamp in seconds since Unix epoch.
    /// \return Julian Date value.
    inline jd_t ts_to_jd(ts_t ts) noexcept {
        return fts_to_jd(static_cast<fts_t>(ts));
    }

    /// \brief Convert Gregorian date (with optional fractional day) to Julian Date (JD).
    /// \param day Day of month (may include fractional part).
    /// \param month Month [1..12].
    /// \param year Full year in the proleptic Gregorian calendar.
    /// \return Julian Date value.
    inline jd_t gregorian_to_jd(double day, int64_t month, int64_t year) noexcept {
        return detail::gregorian_dmy_to_jd_unchecked(day, month, year);
    }

    /// \brief Convert Gregorian date/time components to Julian Date (JD).
    /// \param day Day of month [1..31].
    /// \param month Month [1..12].
    /// \param year Full year in the proleptic Gregorian calendar.
    /// \param hour Hour of day [0..23].
    /// \param minute Minute of hour [0..59].
    /// \param second Second of minute [0..59].
    /// \param millisecond Millisecond of second [0..999].
    /// \return Julian Date value.
    inline jd_t gregorian_to_jd(
            uint32_t day,
            uint32_t month,
            uint32_t year,
            uint32_t hour,
            uint32_t minute,
            uint32_t second = 0,
            uint32_t millisecond = 0) noexcept {
        return detail::gregorian_dmy_to_jd_unchecked(
                static_cast<double>(day) + detail::day_fraction_from_hms(
                        static_cast<int>(hour),
                        static_cast<int>(minute),
                        static_cast<int>(second),
                        static_cast<int>(millisecond)),
                static_cast<int64_t>(month),
                static_cast<int64_t>(year));
    }

    /// \brief Convert Gregorian date/time components to Julian Date (JD) using year-first order.
    /// \param year Full year in the proleptic Gregorian calendar.
    /// \param month Month [1..12].
    /// \param day Day of month [1..31].
    /// \param hour Hour of day [0..23].
    /// \param minute Minute of hour [0..59].
    /// \param second Second of minute [0..59].
    /// \param millisecond Millisecond of second [0..999].
    /// \return Julian Date value.
    inline jd_t gregorian_ymd_to_jd(
            year_t year,
            int month,
            int day,
            int hour = 0,
            int minute = 0,
            int second = 0,
            int millisecond = 0) noexcept {
        return detail::gregorian_dmy_to_jd_unchecked(
                static_cast<double>(day) + detail::day_fraction_from_hms(hour, minute, second, millisecond),
                static_cast<int64_t>(month),
                static_cast<int64_t>(year));
    }

    /// \brief Convert Unix timestamp (floating seconds) to Modified Julian Date (MJD).
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Modified Julian Date value.
    inline mjd_t fts_to_mjd(fts_t ts) noexcept {
        return static_cast<mjd_t>(fts_to_jd(ts) - 2400000.5);
    }

    /// \brief Convert Unix timestamp (seconds) to Modified Julian Date (MJD).
    /// \param ts Unix timestamp in seconds since Unix epoch.
    /// \return Modified Julian Date value.
    inline mjd_t ts_to_mjd(ts_t ts) noexcept {
        return static_cast<mjd_t>(fts_to_mjd(static_cast<fts_t>(ts)));
    }

    /// \brief Convert Gregorian date to Julian Day Number (JDN).
    /// \details JDN is an integer day count with no fractional part.
    /// \param day Day of month [1..31].
    /// \param month Month [1..12].
    /// \param year Full year in the proleptic Gregorian calendar.
    /// \return Julian Day Number value.
    inline jdn_t gregorian_to_jdn(uint32_t day, uint32_t month, uint32_t year) noexcept {
        return detail::gregorian_dmy_to_jdn_unchecked(
                static_cast<int64_t>(day),
                static_cast<int64_t>(month),
                static_cast<int64_t>(year));
    }

    /// \brief Convert Gregorian date to Julian Day Number (JDN) using year-first order.
    /// \details JDN is an integer day count with no fractional part.
    /// \param year Full year in the proleptic Gregorian calendar.
    /// \param month Month [1..12].
    /// \param day Day of month [1..31].
    /// \return Julian Day Number value.
    inline jdn_t gregorian_ymd_to_jdn(year_t year, int month, int day) noexcept {
        return detail::gregorian_dmy_to_jdn_unchecked(
                static_cast<int64_t>(day),
                static_cast<int64_t>(month),
                static_cast<int64_t>(year));
    }

    /// \brief Try converting Gregorian date/time components to Julian Date (JD) using year-first order.
    /// \param year Full year in the proleptic Gregorian calendar.
    /// \param month Month [1..12].
    /// \param day Day of month [1..31].
    /// \param hour Hour of day [0..23].
    /// \param minute Minute of hour [0..59].
    /// \param second Second of minute [0..59].
    /// \param millisecond Millisecond of second [0..999].
    /// \param out Receives the Julian Date value on success.
    /// \return True on success, false when date/time components are invalid.
    inline bool try_gregorian_ymd_to_jd(
            year_t year,
            int month,
            int day,
            int hour,
            int minute,
            int second,
            int millisecond,
            jd_t& out) noexcept {
        if (!is_valid_date(year, month, day) || !is_valid_time(hour, minute, second, millisecond)) {
            return false;
        }
        out = gregorian_ymd_to_jd(year, month, day, hour, minute, second, millisecond);
        return true;
    }

    /// \brief Try converting Gregorian date to Julian Day Number (JDN) using year-first order.
    /// \param year Full year in the proleptic Gregorian calendar.
    /// \param month Month [1..12].
    /// \param day Day of month [1..31].
    /// \param out Receives the Julian Day Number value on success.
    /// \return True on success, false when the date is invalid or produces a negative JDN.
    inline bool try_gregorian_ymd_to_jdn(
            year_t year,
            int month,
            int day,
            jdn_t& out) noexcept {
        if (!is_valid_date(year, month, day)) {
            return false;
        }
        const int64_t a = (14LL - static_cast<int64_t>(month)) / 12LL;
        const int64_t y = static_cast<int64_t>(year) + 4800LL - a;
        const int64_t m = static_cast<int64_t>(month) + 12LL * a - 3LL;
        const int64_t jdn = static_cast<int64_t>(day)
                          + (153LL * m + 2LL) / 5LL
                          + 365LL * y
                          + y / 4LL
                          - y / 100LL
                          + y / 400LL
                          - 32045LL;
        if (jdn < 0) {
            return false;
        }
        out = static_cast<jdn_t>(jdn);
        return true;
    }

} // namespace time_shield

#endif // _TIME_SHIELD_JULIAN_CONVERSIONS_HPP_INCLUDED
