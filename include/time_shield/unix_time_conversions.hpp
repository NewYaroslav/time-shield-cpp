// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_UNIX_TIME_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_UNIX_TIME_CONVERSIONS_HPP_INCLUDED

/// \file unix_time_conversions.hpp
/// \brief Conversions related to UNIX-based time units and epochs.

#include "config.hpp"
#include "constants.hpp"
#include "detail/fast_date.hpp"
#include "time_unit_conversions.hpp"
#include "time_utils.hpp"
#include "types.hpp"

namespace time_shield {

/// \ingroup time_conversions
/// \{

    namespace legacy {

        /// \brief Converts a UNIX timestamp to a year.
        /// \tparam T The type of the year (default is year_t).
        /// \param ts UNIX timestamp.
        /// \return T Year corresponding to the given timestamp.
        template<class T = year_t>
        TIME_SHIELD_CONSTEXPR T years_since_epoch(ts_t ts) noexcept {
            // 9223372029693630000 - значение на момент 292277024400 от 2000 года
            // Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
            // Поэтому пришлось снизить до 9223371890843040000
            constexpr int64_t BIAS_292277022000 = 9223371890843040000LL;
            constexpr int64_t BIAS_2000 = 946684800LL;

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
            return y - UNIX_EPOCH;
        }

    } // namespace legacy

    /// \brief Converts a UNIX timestamp to a year.
    /// \tparam T The type of the year (default is year_t).
    /// \param ts UNIX timestamp.
    /// \return T Year corresponding to the given timestamp.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR T years_since_epoch(ts_t ts) noexcept {
        const detail::DaySplit split = detail::split_unix_day(ts);
        const int64_t year = detail::fast_year_from_days_constexpr(split.days);
        return static_cast<T>(year - UNIX_EPOCH);
    }

    namespace legacy {

        /// \brief Convert a calendar date to UNIX day count.
        ///
        /// Calculates the number of days since the UNIX epoch (January 1, 1970)
        /// for the provided calendar date components.
        ///
        /// \tparam Year Type of the year component.
        /// \tparam Month Type of the month component.
        /// \tparam Day Type of the day component.
        /// \param year Year component of the date.
        /// \param month Month component of the date.
        /// \param day Day component of the date.
        /// \return Number of days since the UNIX epoch.
        template<class Year, class Month, class Day>
        TIME_SHIELD_CONSTEXPR inline uday_t date_to_unix_day(
                Year year,
                Month month,
                Day day) noexcept {
            const int64_t y = static_cast<int64_t>(year) - (static_cast<int64_t>(month) <= 2 ? 1 : 0);
            const int64_t m = static_cast<int64_t>(month) <= 2
                    ? static_cast<int64_t>(month) + 9
                    : static_cast<int64_t>(month) - 3;
            const int64_t era = (y >= 0 ? y : y - 399) / 400;
            const int64_t yoe = y - era * 400;
            const int64_t doy = (153 * m + 2) / 5 + static_cast<int64_t>(day) - 1;
            const int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
            return static_cast<uday_t>(era * 146097 + doe - 719468);
        }

    } // namespace legacy

    /// \brief Convert a calendar date to UNIX day count.
    ///
    /// Calculates the number of days since the UNIX epoch (January 1, 1970)
    /// for the provided calendar date components.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    ///
    /// \tparam Year Type of the year component.
    /// \tparam Month Type of the month component.
    /// \tparam Day Type of the day component.
    /// \param year Year component of the date.
    /// \param month Month component of the date.
    /// \param day Day component of the date.
    /// \return Number of days since the UNIX epoch.
    template<class Year, class Month, class Day>
    TIME_SHIELD_CONSTEXPR inline uday_t date_to_unix_day(
            Year year,
            Month month,
            Day day) noexcept {
        return static_cast<uday_t>(
            detail::fast_days_from_date_constexpr(
                static_cast<int64_t>(year),
                static_cast<int>(month),
                static_cast<int>(day)));
    }

    /// \brief Get UNIX day.
    ///
    /// This function returns the number of days elapsed since the UNIX epoch.
    ///
    /// \tparam T The return type of the function (default is unixday_t).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Number of days since the UNIX epoch.
    template<class T = uday_t>
    constexpr T days_since_epoch(ts_t ts = time_shield::ts()) noexcept {
        return ts / SEC_PER_DAY;
    }

    /// \brief Get UNIX day from milliseconds timestamp.
    ///
    /// This function returns the number of days elapsed since the UNIX epoch, given a timestamp in milliseconds.
    ///
    /// \tparam T The return type of the function (default is unixday_t).
    /// \param t_ms Timestamp in milliseconds (default is current timestamp in milliseconds).
    /// \return Number of days since the UNIX epoch.
    template<class T = uday_t>
    constexpr T days_since_epoch_ms(ts_ms_t t_ms = time_shield::ts_ms()) noexcept {
        return days_since_epoch<T>(ms_to_sec<ts_t>(t_ms));
    }

    /// \brief Get the number of days between two timestamps.
    ///
    /// This function calculates the number of days between two timestamps.
    ///
    /// \tparam T The type of the return value, defaults to int.
    /// \param start The timestamp of the start of the period.
    /// \param stop The timestamp of the end of the period.
    /// \return The number of days between start and stop.
    template<class T = int>
    constexpr T days_between(ts_t start, ts_t stop) noexcept {
        return static_cast<T>((stop - start) / SEC_PER_DAY);
    }

    /// \brief Converts a UNIX day to a timestamp in seconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding
    /// timestamp in seconds at the start of the specified day.
    ///
    /// \tparam T The return type of the function (default is ts_t).
    /// \param unix_day Number of days since the UNIX epoch.
    /// \return The timestamp in seconds representing the beginning of the specified UNIX day.
    template<class T = ts_t>
    constexpr T unix_day_to_ts(uday_t unix_day) noexcept {
        return unix_day * SEC_PER_DAY;
    }

    /// \brief Converts a UNIX day to a timestamp in milliseconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding timestamp
    /// in milliseconds at the start of the specified day.
    ///
    /// \tparam T The return type of the function (default is ts_t).
    /// \param unix_day Number of days since the UNIX epoch.
    /// \return The timestamp in milliseconds representing the beginning of the specified UNIX day.
    template<class T = ts_t>
    constexpr T unix_day_to_ts_ms(uday_t unix_day) noexcept {
        return unix_day * MS_PER_DAY;
    }

    /// \brief Converts a UNIX day to a timestamp representing the end of the day in seconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding
    /// timestamp in seconds at the end of the specified day (23:59:59).
    ///
    /// \tparam T The return type of the function (default is ts_t).
    /// \param unix_day The number of days since the UNIX epoch.
    /// \return The timestamp in seconds representing the end of the specified UNIX day.
    template<class T = ts_t>
    constexpr T end_of_day_from_unix_day(uday_t unix_day) noexcept {
        return unix_day * SEC_PER_DAY + SEC_PER_DAY - 1;
    }

    /// \brief Converts a UNIX day to a timestamp representing the end of the day in milliseconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding
    /// timestamp in milliseconds at the end of the specified day (23:59:59.999).
    ///
    /// \tparam T The return type of the function (default is ts_ms_t).
    /// \param unix_day The number of days since the UNIX epoch.
    /// \return The timestamp in milliseconds representing the end of the specified UNIX day.
    template<class T = ts_ms_t>
    constexpr T end_of_day_from_unix_day_ms(uday_t unix_day) noexcept {
        return unix_day * MS_PER_DAY + MS_PER_DAY - 1;
    }

    /// \brief Converts a UNIX day to a timestamp representing the start of the next day in seconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding
    /// timestamp in seconds at the start of the next day (00:00:00).
    ///
    /// \tparam T The return type of the function (default is ts_t).
    /// \param unix_day The number of days since the UNIX epoch.
    /// \return The timestamp in seconds representing the beginning of the next UNIX day.
    template<class T = ts_ms_t>
    constexpr T start_of_next_day_from_unix_day(uday_t unix_day) noexcept {
        return unix_day * SEC_PER_DAY + SEC_PER_DAY;
    }

    /// \brief Converts a UNIX day to a timestamp representing the start of the next day in milliseconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding
    /// timestamp in milliseconds at the start of the next day (00:00:00.000).
    ///
    /// \tparam T The return type of the function (default is ts_ms_t).
    /// \param unix_day The number of days since the UNIX epoch.
    /// \return The timestamp in milliseconds representing the beginning of the next UNIX day.
    template<class T = ts_ms_t>
    constexpr T start_of_next_day_from_unix_day_ms(uday_t unix_day) noexcept {
        return unix_day * MS_PER_DAY + MS_PER_DAY;
    }

    /// \brief Get UNIX minute.
    ///
    /// This function returns the number of minutes elapsed since the UNIX epoch.
    ///
    /// \tparam T The return type of the function (default is int64_t).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Number of minutes since the UNIX epoch.
    template<class T = int64_t>
    constexpr T min_since_epoch(ts_t ts = time_shield::ts()) {
        return ts / SEC_PER_MIN;
    }

    /// \brief Get the second of the day.
    ///
    /// This function returns a value from 0 to MAX_SEC_PER_DAY representing the second of the day.
    ///
    /// \tparam T The return type of the function (default is int).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Second of the day.
    template<class T = int>
    constexpr T sec_of_day(ts_t ts = time_shield::ts()) noexcept {
        return static_cast<T>(ts % SEC_PER_DAY);
    }

    /// \brief Get the second of the day from milliseconds timestamp.
    ///
    /// This function returns a value from 0 to MAX_SEC_PER_DAY representing the second of the day, given a timestamp in milliseconds.
    ///
    /// \tparam T The return type of the function (default is int).
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Second of the day.
    template<class T = int>
    constexpr T sec_of_day_ms(ts_ms_t ts_ms) noexcept {
        return sec_of_day(ms_to_sec<ts_t>(ts_ms));
    }

    /// \brief Get the second of the day.
    ///
    /// This function returns a value between 0 and MAX_SEC_PER_DAY representing the second of the day, given the hour, minute, and second.
    ///
    /// \tparam T1 The return type of the function (default is int).
    /// \tparam T2 The type of the hour, minute, and second parameters (default is int).
    /// \param hour Hour of the day.
    /// \param min Minute of the hour.
    /// \param sec Second of the minute.
    /// \return Second of the day.
    template<class T1 = int, class T2 = int>
    constexpr T1 sec_of_day(
            T2 hour,
            T2 min,
            T2 sec) noexcept {
        return static_cast<T1>(hour) * static_cast<T1>(SEC_PER_HOUR) +
               static_cast<T1>(min)  * static_cast<T1>(SEC_PER_MIN)  +
               static_cast<T1>(sec);
    }

    /// \brief Get the second of the minute.
    ///
    /// This function returns a value between 0 and 59 representing the second of the minute.
    ///
    /// \tparam T The return type of the function (default is int).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Second of the minute.
    template<class T = int>
    constexpr T sec_of_min(ts_t ts = time_shield::ts()) {
        return static_cast<T>(ts % SEC_PER_MIN);
    }

    /// \brief Get the second of the hour.
    ///
    /// This function returns a value between 0 and 3599 representing the second of the hour.
    ///
    /// \tparam T The return type of the function (default is int).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Second of the hour.
    template<class T = int>
    constexpr T sec_of_hour(ts_t ts = time_shield::ts()) {
        return static_cast<T>(ts % SEC_PER_HOUR);
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_UNIX_TIME_CONVERSIONS_HPP_INCLUDED
