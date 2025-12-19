// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_DATE_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_DATE_CONVERSIONS_HPP_INCLUDED

/// \file date_conversions.hpp
/// \brief Conversions related to calendar dates and DateStruct helpers.

#include "config.hpp"
#include "constants.hpp"
#include "types.hpp"
#include "unix_time_conversions.hpp"
#include "validation.hpp"

namespace time_shield {

/// \ingroup time_conversions
/// \{

    /// \brief Get the year from the timestamp.
    ///
    /// This function returns the year of the specified timestamp in seconds since the Unix epoch.
    ///
    /// \tparam T The return type of the function (default is year_t).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Year of the specified timestamp.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T year_of(ts_t ts = time_shield::ts()) {
        return years_since_epoch<T>(ts) + static_cast<T>(UNIX_EPOCH);
    }

    /// \brief Get the year from the timestamp in milliseconds.
    ///
    /// This function returns the year of the specified timestamp in milliseconds since the Unix epoch.
    ///
    /// \tparam T The return type of the function (default is year_t).
    /// \param ts_ms Timestamp in milliseconds (default is current timestamp).
    /// \return Year of the specified timestamp.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T year_of_ms(ts_ms_t ts_ms = time_shield::ts_ms()) {
        return year_of<T>(ms_to_sec<ts_t>(ts_ms));
    }

    /// \brief Get the number of days in a year.
    /// \param year Year.
    /// \return Number of days in the given year.
    template<class T1 = int, class T2 = year_t>
    TIME_SHIELD_CONSTEXPR inline T1 num_days_in_year(T2 year) noexcept {
        if (is_leap_year_date(year)) return DAYS_PER_LEAP_YEAR;
        return DAYS_PER_YEAR;
    }

    /// \brief Get the number of days in the current year.
    ///
    /// This function calculates and returns the number of days in the current year based on the provided timestamp.
    ///
    /// \param ts Timestamp.
    /// \return Number of days in the current year.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR inline T num_days_in_year_ts(ts_t ts = time_shield::ts()) {
        if (is_leap_year_ts(ts)) return DAYS_PER_LEAP_YEAR;
        return DAYS_PER_YEAR;
    }

    /// \brief Get the day of the week.
    /// \tparam T1 Return type (default: Weekday).
    /// \tparam T2 Year type.
    /// \tparam T3 Month type.
    /// \tparam T4 Day type.
    /// \param year Year.
    /// \param month Month.
    /// \param day Day.
    /// \return Day of the week (SUN = 0, MON = 1, ... SAT = 6).
    template<class T1 = Weekday, class T2 = year_t, class T3 = int, class T4 = int>
    TIME_SHIELD_CONSTEXPR inline T1 day_of_week_date(T2 year, T3 month, T4 day) {
        year_t a = 0;
        year_t y = 0;
        year_t m = 0;
        year_t R = 0;
        a = (14 - month) / MONTHS_PER_YEAR;
        y = year - a;
        m = month + MONTHS_PER_YEAR * a - 2;
        R = 7000 + ( day + y + (y / 4) - (y / 100) + (y / 400) + (31 * m) / MONTHS_PER_YEAR);
        return static_cast<T1>(R % DAYS_PER_WEEK);
    }

    /// \ingroup time_structures
    /// \brief Get the day of the week from a date structure.
    ///
    /// This function takes a date structure with fields 'year', 'mon', and 'day',
    /// and returns the day of the week (SUN = 0, MON = 1, ... SAT = 6).
    ///
    /// \param date Structure containing year, month, and day.
    /// \return Day of the week (SUN = 0, MON = 1, ... SAT = 6).
    template<class T1 = Weekday, class T2>
    TIME_SHIELD_CONSTEXPR inline T1 weekday_of_date(const T2& date) {
        return day_of_week_date(date.year, date.mon, date.day);
    }

    /// \ingroup time_structures
    /// \brief Alias for weekday_of_date.
    /// \copydoc weekday_of_date
    template<class T1 = Weekday, class T2>
    TIME_SHIELD_CONSTEXPR inline T1 weekday_from_date(const T2& date) {
        return weekday_of_date<T1>(date);
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_DATE_CONVERSIONS_HPP_INCLUDED
