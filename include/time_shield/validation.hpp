// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_VALIDATION_HPP_INCLUDED
#define _TIME_SHIELD_VALIDATION_HPP_INCLUDED

/// \file validation.hpp
/// \brief Header file with time-related validation functions.
///
/// This file contains functions for validating dates, times, and timestamps.

#include "config.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "time_zone_struct.hpp"

namespace time_shield {

/// \defgroup time_validation Time Validation
/// \brief A comprehensive set of functions for validating dates, times, leap years, and time zones.
///
/// This module provides functionalities to validate the correctness of date-time values,
/// leap years, and time zone offsets. It also includes utilities for determining weekends
/// and ensuring the validity of timestamp-based calculations.
///
/// ### Key Features:
/// - Validate leap years using dates or timestamps.
/// - Ensure the correctness of date components (year, month, day).
/// - Verify the validity of time components (hour, minute, second, millisecond).
/// - Check the validity of time zones and time zone structures.
/// - Determine if a given timestamp or day corresponds to a weekend.
///
/// ### Usage Examples:
/// - Check if a year is a leap year:
///   \code{.cpp}
///   bool is_leap = time_shield::is_leap_year_date(2024);
///   \endcode
///
/// - Validate a specific date:
///   \code{.cpp}
///   bool is_valid = time_shield::is_valid_date(2024, 2, 29);
///   \endcode
///
/// - Check if a timestamp falls on a weekend:
///   \code{.cpp}
///   bool is_weekend = time_shield::is_day_off(1698249600); // Saturday, Oct 26, 2024
///   \endcode
///
/// \{

    /// \brief Checks if the given year is a leap year.
    /// \tparam T The type of the year (default is year_t).
    /// \param year Year to check.
    /// \return true if the year is a leap year, false otherwise.
    template<class T = year_t>
    constexpr bool is_leap_year_date(T year) noexcept {
        return ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0));
    }

    /// \brief Alias for is_leap_year_date function.
    /// \copydoc is_leap_year_date
    template<class T = year_t>
    constexpr bool check_leap_year(T year) noexcept {
        return is_leap_year_date(year);
    }

    /// \brief Alias for is_leap_year_date function.
    /// \copydoc is_leap_year_date
    template<class T = year_t>
    constexpr bool leap_year(T year) noexcept {
        return is_leap_year_date(year);
    }

//------------------------------------------------------------------------------

    /// \brief Checks if the given year is a leap year.
    ///
    /// This function determines whether the year corresponding to the provided timestamp
    /// is a leap year.
    ///
    /// \tparam T The type of the year parameter (default is year_t).
    /// \param ts Timestamp in seconds since the Unix epoch.
    /// \return Returns true if the year is a leap year.
    TIME_SHIELD_CONSTEXPR inline bool is_leap_year_ts(ts_t ts) {
        // 9223372029693630000 - значение на момент 292277024400 от 2000 года
        // Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
        // Поэтому пришлось снизить до 9223371890843040000
        constexpr int64_t BIAS_292277022000 = 9223371890843040000LL;
        constexpr int64_t BIAS_2000         = 946684800LL;

        int64_t y = MAX_YEAR;
        int64_t secs = -((ts - BIAS_2000) - BIAS_292277022000);

        const int64_t n_400_years = secs / SEC_PER_400_YEARS;
        secs -= n_400_years * SEC_PER_400_YEARS;
        y -= n_400_years * 400;

        const int64_t n_100_years = secs / SEC_PER_100_YEARS;
        secs -= n_100_years * SEC_PER_100_YEARS;
        y -= n_100_years * 100;

        const int64_t n_4_years = secs / SEC_PER_4_YEARS;
        secs -= n_4_years * SEC_PER_4_YEARS;
        y -= n_4_years * 4;

        const int64_t n_1_years = secs / SEC_PER_YEAR;
        secs -= n_1_years * SEC_PER_YEAR;
        y -= n_1_years;

        y = secs == 0 ? y : y - 1;
        return is_leap_year_date(y);
    }

    /// \brief Alias for is_leap_year_ts function.
    /// \copydoc is_leap_year_ts
    TIME_SHIELD_CONSTEXPR inline bool leap_year_ts(ts_t ts) {
        return is_leap_year_ts(ts);
    }

    /// \brief Alias for is_leap_year_ts function.
    /// \copydoc is_leap_year_ts
    TIME_SHIELD_CONSTEXPR inline bool check_leap_year_ts(ts_t ts) {
        return is_leap_year_ts(ts);
    }

    /// \brief Alias for is_leap_year_ts function.
    /// \copydoc is_leap_year_ts
    TIME_SHIELD_CONSTEXPR inline bool is_leap_year(ts_t ts) {
        return is_leap_year_ts(ts);
    }

//------------------------------------------------------------------------------

    /// \brief Check if the time zone is valid.
    /// \tparam T The type of the time zone components (default is int).
    /// \param hour The hour component of the time zone.
    /// \param min The minute component of the time zone.
    /// \return True if the time zone is valid, false otherwise.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_time_zone(
            T hour,
            T min) noexcept {
        if (hour < 0 || hour > 23) return false;
        if (min < 0 || min > 59) return false;
        return true;
    }

    /// \brief Alias for is_valid_time_zone function.
    /// \copydoc is_valid_time_zone
    template<class T = int>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_tz(
            T hour,
            T min) {
        return is_valid_time_zone(hour, min);
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Check if the time zone is valid.
    /// \tparam T The type of the time zone structure (default is TimeZoneStruct).
    /// \param time_zone The time zone structure containing hour and minute components.
    /// \return True if the time zone is valid, false otherwise.
    template<class T = TimeZoneStruct>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_time_zone_offset(
            const T& time_zone) noexcept {
        return is_valid_time_zone(time_zone.hour, time_zone.min);
    }

    /// \ingroup time_structures
    /// \brief Alias for is_valid_time_zone_offset function.
    /// \copydoc is_valid_time_zone_offset
    template<class T = TimeZoneStruct>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_time_zone(
            const T& time_zone) {
        return is_valid_time_zone_offset(time_zone);
    }

    /// \ingroup time_structures
    /// \brief Alias for is_valid_time_zone_offset function.
    /// \copydoc is_valid_time_zone_offset
    template<class T = TimeZoneStruct>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_tz(
            const T& time_zone) {
        return is_valid_time_zone_offset(time_zone);
    }

//------------------------------------------------------------------------------

    /// \brief Checks the correctness of the specified time.
    /// \tparam T1 The type of the hour, minute, and second values (default is int).
    /// \tparam T2 The type of the millisecond value (default is int).
    /// \param hour Hour
    /// \param min Minute
    /// \param sec Second
    /// \param ms Millisecond (default is 0).
    /// \return true if the time is valid, false otherwise.
    template<class T1 = int, class T2 = int>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_time(
            T1 hour,
            T1 min,
            T1 sec,
            T2 ms = 0) noexcept {
        if (hour < 0 || hour > 23) return false;
        if (min < 0 || min > 59) return false;
        if (sec < 0 || sec > 59) return false;
        if (ms < 0 || ms > 999) return false;
        return true;
    }

    /// \ingroup time_structures
    /// \brief Checks the correctness of the specified time.
    /// \tparam T The type of the time structure.
    /// \param time Time structure.
    /// \return true if the time is valid, false otherwise.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_time(
            const T& time) noexcept {
        return is_valid_time(time.hour, time.min, time.sec, time.ms);
    }

    /// \brief Checks the correctness of the specified date.
    /// \tparam T1 The type of the year or day value (default is year_t).
    /// \tparam T2 The type of the month and day values (default is int).
    /// \param year Year or day.
    /// \param month Month.
    /// \param day Day or year.
    /// \return true if the date is valid, false otherwise.
    template<class T1 = year_t, class T2 = int>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_date(
            T1 year,
            T2 month,
            T2 day) noexcept {
        if (day > 31 && year <= 31) {
            return is_valid_date((T1)day, month, (T2)year);
        }
        if (year > MAX_YEAR) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        if (month == FEB) {
            const bool is_leap_year = is_leap_year_date(year);
            if (is_leap_year && day > 29) return false;
            if (!is_leap_year && day > 28) return false;
        } else {
            switch(month) {
            case 4:
            case 6:
            case 9:
            case 11:
                if (day > 30) return false;
            default:
                break;
            };
        }
        return true;
    }

    /// \ingroup time_structures
    /// \brief Checks the correctness of the specified date.
    /// \tparam T The type of the date-time structure.
    /// \param date Date-time structure.
    /// \return true if the date is valid, false otherwise.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_date(const T& date) noexcept {
        return is_valid_date(date.year, date.mon, date.day);
    }

    /// \brief Checks the correctness of a date and time.
    /// \tparam T1 The type of the year or day value (default is year_t).
    /// \tparam T2 The type of the month and day values (default is int).
    /// \tparam T3 The type of the millisecond value (default is int).
    /// \param year Year or day.
    /// \param month Month.
    /// \param day Day or year.
    /// \param hour Hour (default is 0).
    /// \param min Minute (default is 0).
    /// \param sec Second (default is 0).
    /// \param ms Millisecond (default is 0).
    /// \return true if the date and time are valid, false otherwise.
    template<class T1 = year_t, class T2 = int, class T3 = int>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_date_time(
            T1 year,
            T2 month,
            T2 day,
            T2 hour  = 0,
            T2 min   = 0,
            T2 sec   = 0,
            T3 ms    = 0) noexcept {
        if (!is_valid_date(year, month, day)) return false;
        if (!is_valid_time(hour, min, sec, ms)) return false;
        return true;
    }

    /// \ingroup time_structures
    /// \brief Checks the correctness of a date and time.
    /// \tparam T The type of the date-time structure.
    /// \param date_time Date-time structure.
    /// \return true if the date and time are valid, false otherwise.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline bool is_valid_date_time(
            const T &date_time) noexcept {
        if (!is_valid_date(date_time)) return false;
        if (!is_valid_time(date_time)) return false;
        return true;
    }

//------------------------------------------------------------------------------

    /// \brief Check if a given timestamp corresponds to a weekend day (Saturday or Sunday).
    ///
    /// This function checks if the given timestamp falls on a weekend day, which is either Saturday or Sunday.
    ///
    /// \param ts Timestamp to check (default: current timestamp).
    /// \return true if the day is a weekend day, false otherwise.
    TIME_SHIELD_CONSTEXPR inline bool is_day_off(ts_t ts) noexcept {
        const int wd = ((ts / SEC_PER_DAY + THU) % DAYS_PER_WEEK);
        return (wd == SUN || wd == SAT);
    }

    /// \brief Alias for is_day_off function.
    /// \copydoc is_day_off
    TIME_SHIELD_CONSTEXPR inline bool is_weekend(ts_t ts) noexcept {
        return is_day_off(ts);
    }

//------------------------------------------------------------------------------

    /// \brief Check if a given day (since Unix epoch) corresponds to a weekend day (Saturday or Sunday).
    /// This function checks if the given day (number of days since Unix epoch) falls on a weekend day,
    /// which is either Saturday or Sunday.
    /// \param unix_day Day to check (number of days since Unix epoch).
    /// \return true if the day is a weekend day, false otherwise.
    template<class T = uday_t>
    TIME_SHIELD_CONSTEXPR inline bool is_day_off_unix_day(T unix_day) noexcept {
        const int wd = (unix_day + THU) % DAYS_PER_WEEK;
        return (wd == SUN || wd == SAT);
    }

    /// \brief Alias for is_day_off_unix_day function.
    /// \copydoc is_day_off_unix_day
    template<class T = uday_t>
    TIME_SHIELD_CONSTEXPR inline bool is_weekend_unix_day(T unix_day) noexcept {
        return is_day_off_unix_day(unix_day);
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_VALIDATION_HPP_INCLUDED
