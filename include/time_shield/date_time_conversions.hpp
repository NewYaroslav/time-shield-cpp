// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_DATE_TIME_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_DATE_TIME_CONVERSIONS_HPP_INCLUDED

/// \file date_time_conversions.hpp
/// \brief Conversions involving DateTimeStruct and day boundary helpers.

#include "config.hpp"
#include "constants.hpp"
#include "date_conversions.hpp"
#include "date_struct.hpp"
#include "date_time_struct.hpp"
#include "detail/fast_date.hpp"
#include "detail/floor_math.hpp"
#include "enums.hpp"
#include "time_unit_conversions.hpp"
#include "time_utils.hpp"
#include "types.hpp"
#include "unix_time_conversions.hpp"
#include "validation.hpp"

#include <cmath>
#include <ctime>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace time_shield {

/// \ingroup time_conversions
/// \{

    namespace legacy {

        /// \ingroup time_structures
        /// \brief Converts a timestamp to a date-time structure.
        ///
        /// This function converts a timestamp (usually an integer representing seconds since epoch)
        /// to a custom date-time structure. The default type for the timestamp is int64_t.
        ///
        /// \tparam T1 The date-time structure type to be returned.
        /// \tparam T2 The type of the timestamp (default is int64_t).
        /// \param ts The timestamp to be converted.
        /// \return A date-time structure of type T1.
        template<class T1 = DateTimeStruct, class T2 = ts_t>
        T1 to_date_time(T2 ts) {
            // 9223372029693630000 - значение на момент 292277024400 от 2000 года
            // Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
            // Поэтому пришлось снизить до 9223371890843040000
            constexpr int64_t BIAS_292277022000 = 9223371890843040000LL;
            constexpr int64_t BIAS_2000 = 946684800LL;

            int64_t y = MAX_YEAR;
            int64_t secs = -((static_cast<int64_t>(ts) - BIAS_2000) - BIAS_292277022000);

            const int64_t n_400_years = secs / SEC_PER_400_YEARS;
            secs -= n_400_years * SEC_PER_400_YEARS;
            y -= n_400_years * 400LL;

            const int64_t n_100_years = secs / SEC_PER_100_YEARS;
            secs -= n_100_years * SEC_PER_100_YEARS;
            y -= n_100_years * 100LL;

            const int64_t n_4_years = secs / SEC_PER_4_YEARS;
            secs -= n_4_years * SEC_PER_4_YEARS;
            y -= n_4_years * 4LL;

            const int64_t n_1_years = secs / SEC_PER_YEAR;
            secs -= n_1_years * SEC_PER_YEAR;
            y -= n_1_years;

            T1 date_time;

            if (secs == 0) {
                date_time.year = y;
                date_time.mon = 1;
                date_time.day = 1;
                return date_time;
            }

            date_time.year = y - 1;
            const bool is_leap_year = is_leap_year_date(date_time.year);
            secs = is_leap_year ? SEC_PER_LEAP_YEAR - secs : SEC_PER_YEAR - secs;
            const int days = static_cast<int>(secs / SEC_PER_DAY);

            constexpr int JAN_AND_FEB_DAY_LEAP_YEAR = 60 - 1;
            constexpr int TABLE_MONTH_OF_YEAR[] = {
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 31 январь
                2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // 28 февраль
                3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  // 31 март
                4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,    // 30 апрель
                5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
                9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
                10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
                11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
                12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
            };
            constexpr int TABLE_DAY_OF_YEAR[] = {
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,    // 31 январь
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,             // 28 февраль
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,    // 31 март
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,       // 30 апрель
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
            };

            if (is_leap_year) {
                const int prev_days = days - 1;
                date_time.day = days == JAN_AND_FEB_DAY_LEAP_YEAR ? (TABLE_DAY_OF_YEAR[prev_days] + 1) :
                    (days > JAN_AND_FEB_DAY_LEAP_YEAR ? TABLE_DAY_OF_YEAR[prev_days] : TABLE_DAY_OF_YEAR[days]);
                date_time.mon = days >= JAN_AND_FEB_DAY_LEAP_YEAR ? TABLE_MONTH_OF_YEAR[prev_days] : TABLE_MONTH_OF_YEAR[days];
            } else {
                date_time.day = TABLE_DAY_OF_YEAR[days];
                date_time.mon = TABLE_MONTH_OF_YEAR[days];
            }

            ts_t day_secs = static_cast<ts_t>(detail::floor_mod(secs, SEC_PER_DAY));
            date_time.hour = static_cast<decltype(date_time.hour)>(day_secs / SEC_PER_HOUR);
            ts_t min_secs = static_cast<ts_t>(day_secs - date_time.hour * SEC_PER_HOUR);
            date_time.min = static_cast<decltype(date_time.min)>(min_secs / SEC_PER_MIN);
            date_time.sec = static_cast<decltype(date_time.sec)>(min_secs - date_time.min * SEC_PER_MIN);
#           ifdef TIME_SHIELD_CPP17
            if TIME_SHIELD_IF_CONSTEXPR (std::is_floating_point<T2>::value) {
                date_time.ms = static_cast<int>(std::round(std::fmod(static_cast<double>(ts), static_cast<double>(MS_PER_SEC))));
            } else date_time.ms = 0;
#           else
            if (std::is_floating_point<T2>::value) {
                date_time.ms = static_cast<int>(std::round(std::fmod(static_cast<double>(ts), static_cast<double>(MS_PER_SEC))));
            } else date_time.ms = 0;
#           endif
            return date_time;
        }

    } // namespace legacy

    /// \ingroup time_structures
    /// \brief Converts a timestamp to a date-time structure.
    ///
    /// This function converts a timestamp (usually an integer representing seconds since epoch)
    /// to a custom date-time structure. The default type for the timestamp is int64_t.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    ///
    /// \tparam T1 The date-time structure type to be returned.
    /// \tparam T2 The type of the timestamp (default is int64_t).
    /// \param ts The timestamp to be converted.
    /// \return A date-time structure of type T1.
    template<class T1 = DateTimeStruct, class T2 = ts_t>
    T1 to_date_time(T2 ts) {
        const int64_t whole_sec = static_cast<int64_t>(ts);
        const detail::DaySplit split = detail::split_unix_day(whole_sec);
        const detail::FastDate date = detail::fast_date_from_days(split.days);

        T1 date_time{};
        date_time.year = static_cast<decltype(date_time.year)>(date.year);
        date_time.mon = static_cast<decltype(date_time.mon)>(date.month);
        date_time.day = static_cast<decltype(date_time.day)>(date.day);

        const ts_t day_secs = static_cast<ts_t>(split.sec_of_day);
        date_time.hour = static_cast<decltype(date_time.hour)>(day_secs / SEC_PER_HOUR);
        const ts_t min_secs = static_cast<ts_t>(day_secs - date_time.hour * SEC_PER_HOUR);
        date_time.min = static_cast<decltype(date_time.min)>(min_secs / SEC_PER_MIN);
        date_time.sec = static_cast<decltype(date_time.sec)>(min_secs - date_time.min * SEC_PER_MIN);
#       ifdef TIME_SHIELD_CPP17
        if TIME_SHIELD_IF_CONSTEXPR (std::is_floating_point<T2>::value) {
            date_time.ms = static_cast<int>(std::round(std::fmod(static_cast<double>(ts), static_cast<double>(MS_PER_SEC))));
        } else date_time.ms = 0;
#       else
        if (std::is_floating_point<T2>::value) {
            date_time.ms = static_cast<int>(std::round(std::fmod(static_cast<double>(ts), static_cast<double>(MS_PER_SEC))));
        } else date_time.ms = 0;
#       endif
        return date_time;
    }

    /// \ingroup time_structures
    /// \brief Converts a timestamp in milliseconds to a date-time structure with milliseconds.
    /// \tparam T The type of the date-time structure to return.
    /// \param ts The timestamp in milliseconds to convert.
    /// \return T A date-time structure with the corresponding date and time components.
    template<class T>
    inline T to_date_time_ms(ts_ms_t ts) {
        const ts_t sec = ms_to_sec<ts_t, ts_ms_t>(ts);
        T date_time = to_date_time<T, ts_t>(sec);
        date_time.ms = ms_of_ts(ts); // Extract and set the ms component
        return date_time;
    }

    namespace legacy {

        /// \brief Converts a date and time to a timestamp.
        ///
        /// This function converts a given date and time to a timestamp, which is the number
        /// of seconds since the Unix epoch (January 1, 1970).
        ///
        /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
        /// and are automatically reordered.
        ///
        /// \tparam T1 The type of the year parameter (default is int64_t).
        /// \tparam T2 The type of the other date and time parameters (default is int).
        /// \param year The year value.
        /// \param month The month value.
        /// \param day   The day value.
        /// \param hour  The hour value (default is 0).
        /// \param min   The minute value (default is 0).
        /// \param sec   The second value (default is 0).
        /// \return Timestamp representing the given date and time.
        /// \throws std::invalid_argument if the date-time combination is invalid.
        ///
        /// \par Aliases:
        /// The following function names are provided as aliases:
        /// - `ts(...)`
        /// - `get_ts(...)`
        /// - `get_timestamp(...)`
        /// - `timestamp(...)`
        /// - `to_ts(...)`
        ///
        /// These aliases are macro-generated and behave identically to `to_timestamp`.
        ///
        /// \sa ts() \sa get_ts() \sa get_timestamp() \sa timestamp() \sa to_ts()
        template<class T1 = year_t, class T2 = int>
        TIME_SHIELD_CONSTEXPR inline ts_t to_timestamp(
                T1 year,
                T2 month,
                T2 day,
                T2 hour  = 0,
                T2 min   = 0,
                T2 sec   = 0) {

            if (day >= UNIX_EPOCH && year <= 31) {
                return to_timestamp((T1)day, month, (T2)year, hour, min, sec);
            }
            if (!is_valid_date_time(year, month, day, hour, min, sec)) {
                throw std::invalid_argument("Invalid date-time combination");
            }

            int64_t secs = 0;
            int64_t years = (static_cast<int64_t>(MAX_YEAR) - year);

            const int64_t n_400_years = years / 400LL;
            secs += n_400_years * SEC_PER_400_YEARS;
            years -= n_400_years * 400LL;

            const int64_t n_100_years = years / 100LL;
            secs += n_100_years * SEC_PER_100_YEARS;
            years -= n_100_years * 100LL;

            const int64_t n_4_years = years / 4LL;
            secs += n_4_years * SEC_PER_4_YEARS;
            years -= n_4_years * 4LL;

            secs += years * SEC_PER_YEAR;

            // 9223372029693630000 - значение на момент 292277024400 от 2000 года
            // Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
            // Поэтому пришлось снизить до 9223371890843040000
            constexpr int64_t BIAS_292277022000 = 9223371890843040000LL;
            constexpr int64_t BIAS_2000 = 946684800LL;

            secs = BIAS_292277022000 - secs;
            secs += BIAS_2000;

            if (month == 1 && day == 1 &&
                hour == 0 && min == 0 &&
                sec == 0) {
                return secs;
            }

            constexpr int lmos[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
            constexpr int mos[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

            secs += (is_leap_year_date(year) ? (lmos[month - 1] + day - 1) : (mos[month - 1] + day - 1)) * SEC_PER_DAY;
            secs += SEC_PER_HOUR * hour + SEC_PER_MIN * min + sec;
            return secs;
        }

        /// \brief Converts a date and time to a timestamp without validation.
        ///
        /// This function converts a given date and time to a timestamp, which is the number
        /// of seconds since the Unix epoch (January 1, 1970).
        ///
        /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
        /// and are automatically reordered.
        ///
        /// \tparam T1 The type of the year parameter (default is int64_t).
        /// \tparam T2 The type of the other date and time parameters (default is int).
        /// \param year The year value.
        /// \param month The month value.
        /// \param day   The day value.
        /// \param hour  The hour value (default is 0).
        /// \param min   The minute value (default is 0).
        /// \param sec   The second value (default is 0).
        /// \return Timestamp representing the given date and time.
        template<class T1 = year_t, class T2 = int>
        TIME_SHIELD_CONSTEXPR inline ts_t to_timestamp_unchecked(
                T1 year,
                T2 month,
                T2 day,
                T2 hour  = 0,
                T2 min   = 0,
                T2 sec   = 0) noexcept {

            if (day >= UNIX_EPOCH && year <= 31) {
                return to_timestamp_unchecked((T1)day, month, (T2)year, hour, min, sec);
            }

            int64_t secs = 0;
            int64_t years = (static_cast<int64_t>(MAX_YEAR) - year);

            const int64_t n_400_years = years / 400LL;
            secs += n_400_years * SEC_PER_400_YEARS;
            years -= n_400_years * 400LL;

            const int64_t n_100_years = years / 100LL;
            secs += n_100_years * SEC_PER_100_YEARS;
            years -= n_100_years * 100LL;

            const int64_t n_4_years = years / 4LL;
            secs += n_4_years * SEC_PER_4_YEARS;
            years -= n_4_years * 4LL;

            secs += years * SEC_PER_YEAR;

            // 9223372029693630000 - значение на момент 292277024400 от 2000 года
            // Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
            // Поэтому пришлось снизить до 9223371890843040000
            constexpr int64_t BIAS_292277022000 = 9223371890843040000LL;
            constexpr int64_t BIAS_2000 = 946684800LL;

            secs = BIAS_292277022000 - secs;
            secs += BIAS_2000;

            if (month == 1 && day == 1 &&
                hour == 0 && min == 0 &&
                sec == 0) {
                return secs;
            }

            constexpr int lmos[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
            constexpr int mos[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

            secs += (is_leap_year_date(year) ? (lmos[month - 1] + day - 1) : (mos[month - 1] + day - 1)) * SEC_PER_DAY;
            secs += SEC_PER_HOUR * hour + SEC_PER_MIN * min + sec;
            return secs;
        }

    } // namespace legacy

    /// \brief Converts a date and time to a timestamp without validation.
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value (default is 0).
    /// \param min   The minute value (default is 0).
    /// \param sec   The second value (default is 0).
    /// \return Timestamp representing the given date and time.
    template<class T1 = year_t, class T2 = int>
    TIME_SHIELD_CONSTEXPR inline ts_t to_timestamp_unchecked(
            T1 year,
            T2 month,
            T2 day,
            T2 hour  = 0,
            T2 min   = 0,
            T2 sec   = 0) noexcept {

        if (day >= UNIX_EPOCH && year <= 31) {
            return to_timestamp_unchecked((T1)day, month, (T2)year, hour, min, sec);
        }

        const dse_t unix_day = date_to_unix_day(year, month, day);
        return static_cast<ts_t>(unix_day * SEC_PER_DAY
            + SEC_PER_HOUR * static_cast<int64_t>(hour)
            + SEC_PER_MIN * static_cast<int64_t>(min)
            + static_cast<int64_t>(sec));
    }

    /// \brief Converts a date and time to a timestamp.
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value (default is 0).
    /// \param min   The minute value (default is 0).
    /// \param sec   The second value (default is 0).
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    ///
    /// \par Aliases:
    /// The following function names are provided as aliases:
    /// - `ts(...)`
    /// - `get_ts(...)`
    /// - `get_timestamp(...)`
    /// - `timestamp(...)`
    /// - `to_ts(...)`
    ///
    /// These aliases are macro-generated and behave identically to `to_timestamp`.
    ///
    /// \sa ts() \sa get_ts() \sa get_timestamp() \sa timestamp() \sa to_ts()
    template<class T1 = year_t, class T2 = int>
    TIME_SHIELD_CONSTEXPR inline ts_t to_timestamp(
            T1 year,
            T2 month,
            T2 day,
            T2 hour  = 0,
            T2 min   = 0,
            T2 sec   = 0) {

        if (day >= UNIX_EPOCH && year <= 31) {
            return to_timestamp((T1)day, month, (T2)year, hour, min, sec);
        }
        if (!is_valid_date_time(year, month, day, hour, min, sec)) {
            throw std::invalid_argument("Invalid date-time combination");
        }

        return to_timestamp_unchecked(year, month, day, hour, min, sec);
    }

    /// \ingroup time_structures
    /// \brief Converts a date-time structure to a timestamp.
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T The type of the date-time structure.
    /// \param date_time The date-time structure.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t dt_to_timestamp(
            const T& date_time) {
        return to_timestamp(
            date_time.year,
            date_time.mon,
            date_time.day,
            date_time.hour,
            date_time.min,
            date_time.sec
        );
    }

    /// \ingroup time_structures
    /// \brief Converts a std::tm structure to a timestamp.
    ///
    /// This function converts a given std::tm structure to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// \param timeinfo Pointer to a std::tm structure containing the date and time information.
    /// \return Timestamp representing the given date and time.
    TIME_SHIELD_CONSTEXPR inline ts_t tm_to_timestamp(
            const std::tm *timeinfo) {
        return to_timestamp(
            static_cast<year_t>(timeinfo->tm_year + 1900),
            static_cast<int>(timeinfo->tm_mon + 1),
            static_cast<int>(timeinfo->tm_mday),
            static_cast<int>(timeinfo->tm_hour),
            static_cast<int>(timeinfo->tm_min),
            static_cast<int>(timeinfo->tm_sec)
        );
    }

    /// \ingroup time_structures
    /// \brief Converts a date-time structure to a timestamp in milliseconds.
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is year_t).
    /// \tparam T2 The type of the month, day, hour, minute, and second parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value (default is 0).
    /// \param min The minute value (default is 0).
    /// \param sec The second value (default is 0).
    /// \param ms The millisecond value (default is 0).
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T1 = year_t, class T2 = int>
    TIME_SHIELD_CONSTEXPR inline ts_ms_t to_timestamp_ms(
            T1 year,
            T2 month,
            T2 day,
            T2 hour = 0,
            T2 min  = 0,
            T2 sec  = 0,
            T2 ms   = 0) {
        int64_t sec_value = static_cast<int64_t>(to_timestamp<T1, T2>(year, month, day, hour, min, sec));
        int64_t ms_value = static_cast<int64_t>(ms);
        sec_value += detail::floor_div(ms_value, static_cast<int64_t>(MS_PER_SEC));
        ms_value = detail::floor_mod(ms_value, static_cast<int64_t>(MS_PER_SEC));
        if ((sec_value > 0 &&
             sec_value > ((std::numeric_limits<int64_t>::max)() - ms_value) / MS_PER_SEC) ||
            (sec_value < 0 &&
             sec_value < (std::numeric_limits<int64_t>::min)() / MS_PER_SEC)) {
            return ERROR_TIMESTAMP;
        }
        return static_cast<ts_ms_t>(sec_value * MS_PER_SEC + ms_value);
    }

    /// \ingroup time_structures
    /// \brief Converts a date-time structure to a timestamp in milliseconds.
    ///
    /// This function converts a given date and time structure to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T The type of the date-time structure.
    /// \param date_time The date-time structure containing year, month, day, hour, minute, second, and millisecond fields.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_ms_t dt_to_timestamp_ms(
            const T& date_time) {
        int64_t sec_value = static_cast<int64_t>(dt_to_timestamp(date_time));
        int64_t ms_value = static_cast<int64_t>(date_time.ms);
        sec_value += detail::floor_div(ms_value, static_cast<int64_t>(MS_PER_SEC));
        ms_value = detail::floor_mod(ms_value, static_cast<int64_t>(MS_PER_SEC));
        if ((sec_value > 0 &&
             sec_value > ((std::numeric_limits<int64_t>::max)() - ms_value) / MS_PER_SEC) ||
            (sec_value < 0 &&
             sec_value < (std::numeric_limits<int64_t>::min)() / MS_PER_SEC)) {
            return ERROR_TIMESTAMP;
        }
        return static_cast<ts_ms_t>(sec_value * MS_PER_SEC + ms_value);
    }

    /// \ingroup time_structures
    /// \brief Converts a std::tm structure to a timestamp in milliseconds.
    ///
    /// This function converts a given std::tm structure to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \param timeinfo Pointer to a std::tm structure containing the date and time information.
    /// \return Timestamp in milliseconds representing the given date and time.
    TIME_SHIELD_CONSTEXPR inline ts_t tm_to_timestamp_ms(
            const std::tm *timeinfo) {
        return sec_to_ms(tm_to_timestamp(timeinfo));
    }

    /// \brief Converts a date and time to a floating-point timestamp.
    ///
    /// This function converts a given date and time to a floating-point timestamp,
    /// which is the number of seconds (with fractional milliseconds) since the Unix epoch
    /// (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is year_t).
    /// \tparam T2 The type of the month, day, hour, minute, and second parameters (default is int).
    /// \tparam T3 The type of the millisecond parameter (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value (default is 0).
    /// \param min The minute value (default is 0).
    /// \param sec The second value (default is 0).
    /// \param ms The millisecond value (default is 0).
    /// \return Floating-point timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T1 = year_t, class T2 = int, class T3 = int>
    TIME_SHIELD_CONSTEXPR inline fts_t to_ftimestamp(
            T1 year,
            T2 month,
            T2 day,
            T2 hour  = 0,
            T2 min   = 0,
            T2 sec   = 0,
            T3 ms    = 0) {
        int64_t sec_value = static_cast<int64_t>(to_timestamp(year, month, day, hour, min, sec));
        int64_t ms_value = static_cast<int64_t>(ms);
        sec_value += detail::floor_div(ms_value, static_cast<int64_t>(MS_PER_SEC));
        ms_value = detail::floor_mod(ms_value, static_cast<int64_t>(MS_PER_SEC));
        return static_cast<fts_t>(sec_value) +
            static_cast<fts_t>(ms_value) / static_cast<fts_t>(MS_PER_SEC);
    }

    /// \ingroup time_structures
    /// \brief Converts a date-time structure to a floating-point timestamp.
    ///
    /// This function converts a given date and time structure to a floating-point timestamp,
    /// which is the number of seconds (with fractional milliseconds) since the Unix epoch
    /// (January 1, 1970).
    ///
    /// \tparam T The type of the date-time structure.
    /// \param date_time The date-time structure containing year, month, day, hour, minute, second, and millisecond fields.
    /// \return Floating-point timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline fts_t dt_to_ftimestamp(
            const T& date_time) {
        int64_t sec_value = static_cast<int64_t>(to_timestamp(date_time));
        int64_t ms_value = static_cast<int64_t>(date_time.ms);
        sec_value += detail::floor_div(ms_value, static_cast<int64_t>(MS_PER_SEC));
        ms_value = detail::floor_mod(ms_value, static_cast<int64_t>(MS_PER_SEC));
        return static_cast<fts_t>(sec_value) +
            static_cast<fts_t>(ms_value) / static_cast<fts_t>(MS_PER_SEC);
    }

    /// \brief Converts a std::tm structure to a floating-point timestamp.
    ///
    /// This function converts a given std::tm structure to a floating-point timestamp,
    /// which is the number of seconds (with fractional milliseconds) since the Unix epoch
    /// (January 1, 1970).
    ///
    /// \param timeinfo Pointer to the std::tm structure containing the date and time.
    /// \return Floating-point timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    TIME_SHIELD_CONSTEXPR inline fts_t tm_to_ftimestamp(
            const std::tm* timeinfo) {
        return static_cast<fts_t>(tm_to_timestamp(timeinfo));
    }

    /// \brief Get the start of the day timestamp.
    ///
    /// This function returns the timestamp at the start of the day.
    /// The function sets the hours, minutes, and seconds to zero.
    ///
    /// \param ts Timestamp.
    /// \return Start of the day timestamp.
    constexpr ts_t start_of_day(ts_t ts = time_shield::ts()) noexcept {
        return ts - detail::floor_mod(ts, SEC_PER_DAY);
    }

    /// \brief Get timestamp of the start of the previous day.
    ///
    /// This function returns the timestamp at the start of the previous day.
    ///
    /// \param ts Timestamp of the current day.
    /// \param days Number of days to go back (default is 1).
    /// \return Timestamp of the start of the previous day.
    template<class T = int>
    constexpr ts_t start_of_prev_day(ts_t ts = time_shield::ts(), T days = 1) noexcept {
        return ts - detail::floor_mod(ts, SEC_PER_DAY) - SEC_PER_DAY * days;
    }

    /// \brief Get the start of the day timestamp in seconds.
    ///
    /// This function returns the timestamp at the start of the day in seconds.
    /// The function sets the hours, minutes, and seconds to zero.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Start of the day timestamp in seconds.
    constexpr ts_t start_of_day_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_day(ms_to_sec<ts_t>(ts_ms));
    }

    /// \brief Get the start of the day timestamp in milliseconds.
    ///
    /// This function returns the timestamp at the start of the day in milliseconds.
    /// The function sets the hours, minutes, seconds, and milliseconds to zero.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Start of the day timestamp in milliseconds.
    constexpr ts_ms_t start_of_day_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - detail::floor_mod(ts_ms, MS_PER_DAY);
    }

    /// \brief Get the timestamp of the start of the day after a specified number of days.
    ///
    /// Calculates the timestamp for the beginning of the day after a specified number of days
    /// relative to the given timestamp.
    ///
    /// \param ts The current timestamp in seconds.
    /// \param days The number of days after the current day (default is 1).
    /// \return The timestamp in seconds representing the beginning of the specified future day.
    template<class T = int>
    constexpr ts_t start_of_next_day(ts_t ts, T days = 1) noexcept {
        return start_of_day(ts) + days * SEC_PER_DAY;
    }

    /// \brief Get the timestamp of the start of the day after a specified number of days.
    ///
    /// Calculates the timestamp for the beginning of the day after a specified number of days
    /// relative to the given timestamp in milliseconds.
    ///
    /// \param ts_ms The current timestamp in milliseconds.
    /// \param days The number of days after the current day (default is 1).
    /// \return The timestamp in milliseconds representing the beginning of the specified future day.
    template<class T = int>
    constexpr ts_ms_t start_of_next_day_ms(ts_ms_t ts_ms, T days = 1) noexcept {
        return start_of_day_ms(ts_ms) + days * MS_PER_DAY;
    }

    /// \brief Calculate the timestamp for a specified number of days in the future.
    ///
    /// Adds the given number of days to the provided timestamp, without adjusting to the start of the day.
    ///
    /// \param ts The current timestamp in seconds.
    /// \param days The number of days to add to the current timestamp (default is 1).
    /// \return The timestamp in seconds after adding the specified number of days.
    template<class T = int>
    constexpr ts_t next_day(ts_t ts, T days = 1) noexcept {
        return ts + days * SEC_PER_DAY;
    }

    /// \brief Calculate the timestamp for a specified number of days in the future (milliseconds).
    ///
    /// Adds the given number of days to the provided timestamp, without adjusting to the start of the day.
    ///
    /// \param ts_ms The current timestamp in milliseconds.
    /// \param days The number of days to add to the current timestamp (default is 1).
    /// \return The timestamp in milliseconds after adding the specified number of days.
    template<class T = int>
    constexpr ts_ms_t next_day_ms(ts_ms_t ts_ms, T days = 1) noexcept {
        return ts_ms + days * MS_PER_DAY;
    }

    /// \brief Get the timestamp at the end of the day.
    ///
    /// This function sets the hour to 23, minute to 59, and second to 59.
    ///
    /// \param ts Timestamp.
    /// \return Timestamp at the end of the day.
    constexpr ts_t end_of_day(ts_t ts = time_shield::ts()) noexcept {
        return ts - detail::floor_mod(ts, SEC_PER_DAY) + SEC_PER_DAY - 1;
    }

    /// \brief Get the timestamp at the end of the day in seconds.
    ///
    /// This function sets the hour to 23, minute to 59, and second to 59.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at the end of the day in seconds.
    constexpr ts_t end_of_day_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_day(ms_to_sec<ts_t>(ts_ms));
    }

    /// \brief Get the timestamp at the end of the day in milliseconds.
    ///
    /// This function sets the hour to 23, minute to 59, second to 59, and millisecond to 999.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at the end of the day in milliseconds.
    constexpr ts_ms_t end_of_day_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - detail::floor_mod(ts_ms, MS_PER_DAY) + MS_PER_DAY - 1;
    }

    /// \brief Get the timestamp of the start of the year.
    /// \param year Year.
    /// \return Timestamp at 00:00:00 of the first day of the year.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_year_date(T year) {
        const ts_t year_ts = to_timestamp(year, 1, 1);

        return start_of_day(year_ts);
    }

    /// \brief Get the timestamp in milliseconds of the start of the year.
    ///
    /// This function returns the timestamp at the start of the specified year in milliseconds.
    ///
    /// \param year Year.
    /// \return Timestamp of the start of the year in milliseconds.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_ms_t start_of_year_date_ms(T year) {
        return sec_to_ms(start_of_year_date(year));
    }

    /// \brief Get the start of the year timestamp.
    ///
    /// This function resets the days, months, hours, minutes, and seconds of the given timestamp
    /// to the beginning of the year.
    ///
    /// \param ts Timestamp.
    /// \return Start of the year timestamp.
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_year(ts_t ts) noexcept {
        constexpr ts_t BIAS_2100 = 4102444800;
        if (ts >= 0 && ts < BIAS_2100) {
            constexpr ts_t SEC_PER_YEAR_X2 = SEC_PER_YEAR * 2;
            ts_t year_start_ts = detail::floor_mod(ts, SEC_PER_4_YEARS);
            if (year_start_ts < SEC_PER_YEAR) {
                return ts - year_start_ts;
            } else if (year_start_ts < SEC_PER_YEAR_X2) {
                return ts + SEC_PER_YEAR - year_start_ts;
            } else if (year_start_ts < (SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR)) {
                return ts + SEC_PER_YEAR_X2 - year_start_ts;
            }
            return ts + (SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR) - year_start_ts;
        }

        constexpr ts_t BIAS_2000 = 946684800;
        ts_t secs = ts - BIAS_2000;

        ts_t offset_y400 = detail::floor_mod(secs, SEC_PER_400_YEARS);
        ts_t start_ts = secs - offset_y400 + BIAS_2000;
        secs = offset_y400;

        if (secs >= SEC_PER_FIRST_100_YEARS) {
            secs -= SEC_PER_FIRST_100_YEARS;
            start_ts += SEC_PER_FIRST_100_YEARS;
            while (secs >= SEC_PER_100_YEARS) {
                secs -= SEC_PER_100_YEARS;
                start_ts += SEC_PER_100_YEARS;
            }

            constexpr ts_t SEC_PER_4_YEARS_V2 = 4 * SEC_PER_YEAR;
            if (secs >= SEC_PER_4_YEARS_V2) {
                secs -= SEC_PER_4_YEARS_V2;
                start_ts += SEC_PER_4_YEARS_V2;
            } else {
                start_ts += secs - detail::floor_mod(secs, SEC_PER_YEAR);
                return start_ts;
            }
        }

        ts_t offset_4y = detail::floor_mod(secs, SEC_PER_4_YEARS);
        start_ts += secs - offset_4y;
        secs = offset_4y;

        if (secs >= SEC_PER_LEAP_YEAR) {
            secs -= SEC_PER_LEAP_YEAR;
            start_ts += SEC_PER_LEAP_YEAR;
            start_ts += secs - detail::floor_mod(secs, SEC_PER_YEAR);
            return start_ts;
        }

        start_ts += secs - detail::floor_mod(secs, SEC_PER_YEAR);
        return start_ts;
    }

    /// \brief Get the timestamp at the start of the year in milliseconds.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at 00:00:00.000 of the first day of the year.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t start_of_year_ms(ts_ms_t ts_ms = time_shield::ts_ms()) {
        return sec_to_ms(start_of_year(ms_to_sec<ts_t>(ts_ms)));
    }

    /// \brief Get the end-of-year timestamp.
    ///
    /// This function finds the last timestamp of the current year.
    ///
    /// \param ts Timestamp.
    /// \return End-of-year timestamp.
    TIME_SHIELD_CONSTEXPR inline ts_t end_of_year(ts_t ts = time_shield::ts()) {
        const ts_t year_start = start_of_year(ts);
        const ts_t year_days = static_cast<ts_t>(num_days_in_year_ts(ts));
        return year_start + year_days * SEC_PER_DAY - 1;
    }

    /// \brief Get the timestamp in milliseconds of the end of the year.
    ///
    /// This function finds the last millisecond of the current year in milliseconds.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return End-of-year timestamp in milliseconds.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_ms_t end_of_year_ms(ts_ms_t ts_ms = time_shield::ts_ms()) {
        return sec_to_ms(end_of_year(ms_to_sec<ts_t>(ts_ms))) + (MS_PER_SEC - 1);
    }

    /// \brief Get the day of the year.
    ///
    /// This function returns the day of the year for the specified timestamp.
    ///
    /// \param ts Timestamp.
    /// \return Day of the year.
    template<class T = int>
    inline T day_of_year(ts_t ts = time_shield::ts()) {
        return  static_cast<T>(((ts - start_of_year(ts)) / SEC_PER_DAY) + 1);
    }

    /// \brief Get the month of the year.
    ///
    /// This function returns the month of the year for the specified timestamp.
    ///
    /// \param ts Timestamp.
    /// \return Month of the year.
    template<class T = Month>
    TIME_SHIELD_CONSTEXPR inline T month_of_year(ts_t ts) noexcept {
        constexpr int JAN_AND_FEB_DAY_LEAP_YEAR = 60;
        constexpr int TABLE_MONTH_OF_YEAR[] = {
            0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 31 январь
            2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // 28 февраль
            3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  // 31 март
            4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,    // 30 апрель
            5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
            9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
            10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
            11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
            12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
        };
        const size_t dy = day_of_year<size_t>(ts);
        return static_cast<T>((is_leap_year(ts) && dy >= JAN_AND_FEB_DAY_LEAP_YEAR) ? TABLE_MONTH_OF_YEAR[dy - 1] : TABLE_MONTH_OF_YEAR[dy]);
    }

    /// \brief Get the day of the month.
    ///
    /// This function returns the day of the month for the specified timestamp.
    ///
    /// \param ts Timestamp.
    /// \return Day of the month.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR inline T day_of_month(ts_t ts = time_shield::ts()) {
        constexpr int JAN_AND_FEB_DAY_LEAP_YEAR = 60;
        // таблица для обычного года, не високосного
        constexpr int TABLE_DAY_OF_YEAR[] = {
            0,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,    // 31 январь
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,             // 28 февраль
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,    // 31 март
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,       // 30 апрель
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
        };
        const size_t dy = day_of_year<size_t>(ts);
        if(is_leap_year(ts)) {
            if(dy == JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1] + 1;
            if(dy > JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1];
        }
        return TABLE_DAY_OF_YEAR[dy];
    }

    /// \brief Get the number of days in a month.
    ///
    /// This function calculates and returns the number of days in the specified month and year.
    ///
    /// \param year Year as an integer.
    /// \param month Month as an integer.
    /// \return The number of days in the given month and year.
    template<class T1 = int, class T2 = year_t, class T3 = int>
    TIME_SHIELD_CONSTEXPR T1 num_days_in_month(T2 year, T3 month) noexcept {
        constexpr T1 num_days[13] = {0,31,30,31,30,31,30,31,31,30,31,30,31};
        return (month > MONTHS_PER_YEAR || month < 0)
            ? static_cast<T1>(0)
            : (month == FEB ? static_cast<T1>(is_leap_year_date(year) ? 29 : 28) : num_days[month]);
    }

    /// \brief Get the number of days in the month of the given timestamp.
    ///
    /// This function calculates and returns the number of days in the month of the specified timestamp.
    ///
    /// \param ts The timestamp to extract month and year from.
    /// \return The number of days in the month of the given timestamp.
    template<class T1 = int>
    TIME_SHIELD_CONSTEXPR T1 num_days_in_month_ts(ts_t ts = time_shield::ts()) noexcept {
        constexpr T1 num_days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        const int month = month_of_year<int>(ts);
        if (month == FEB) {
            return is_leap_year(ts) ? 29 : 28;
        }
        return num_days[month];
    }

    /// \brief Get the second of the week day from a timestamp.
    /// \param ts Timestamp.
    /// \return Weekday (SUN = 0, MON = 1, ... SAT = 6).
    template<class T = Weekday>
    constexpr T weekday_of_ts(ts_t ts) noexcept {
        const ts_t days = detail::floor_div(ts, SEC_PER_DAY);
        return static_cast<T>(detail::floor_mod(days + THU, DAYS_PER_WEEK));
    }

    /// \brief Get the weekday from a timestamp in milliseconds.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Weekday (SUN = 0, MON = 1, ... SAT = 6).
    template<class T = Weekday>
    constexpr T weekday_of_ts_ms(ts_ms_t ts_ms) {
        return weekday_of_ts<T>(ms_to_sec<ts_t>(ts_ms));
    }

    /// \brief Alias for weekday_of_ts.
    /// \copydoc weekday_of_ts
    template<class T = Weekday>
    constexpr T get_weekday_from_ts(ts_t ts) noexcept {
        return weekday_of_ts<T>(ts);
    }

    /// \brief Alias for weekday_of_ts_ms.
    /// \copydoc weekday_of_ts_ms
    template<class T = Weekday>
    constexpr T get_weekday_from_ts_ms(ts_ms_t ts_ms) {
        return weekday_of_ts_ms<T>(ts_ms);
    }

    /// \brief Get the timestamp at the start of the current month.
    ///
    /// This function returns the timestamp at the start of the current month,
    /// setting the day to the first day of the month and the time to 00:00:00.
    ///
    /// \param ts Timestamp (default is current timestamp)
    /// \return Timestamp at the start of the current month
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_month(ts_t ts = time_shield::ts()) {
        return start_of_day(ts) - (day_of_month(ts) - 1) * SEC_PER_DAY;
    }

    /// \brief Get the last timestamp of the current month.
    ///
    /// This function returns the last timestamp of the current month,
    /// setting the day to the last day of the month and the time to 23:59:59.
    ///
    /// \param ts Timestamp (default is current timestamp)
    /// \return Last timestamp of the current month
    TIME_SHIELD_CONSTEXPR inline ts_t end_of_month(ts_t ts = time_shield::ts()) {
        return end_of_day(ts) + (num_days_in_month_ts(ts) - day_of_month(ts)) * SEC_PER_DAY;
    }

    /// \brief Get the timestamp of the last Sunday of the current month.
    ///
    /// This function returns the timestamp of the last Sunday of the current month,
    /// setting the day to the last Sunday and the time to 00:00:00.
    ///
    /// \param ts Timestamp (default is current timestamp)
    /// \return Timestamp of the last Sunday of the current month
    TIME_SHIELD_CONSTEXPR inline ts_t last_sunday_of_month(ts_t ts = time_shield::ts()) {
        return end_of_month(ts) - weekday_of_ts(ts) * SEC_PER_DAY;
    }

    /// \brief Get the day of the last Sunday of the given month and year.
    ///
    /// This function returns the day of the last Sunday of the specified month and year.
    ///
    /// \param year Year
    /// \param month Month (1 = January, 12 = December)
    /// \return Day of the last Sunday of the given month and year
    template<class T1 = int, class T2 = year_t, class T3 = int>
    TIME_SHIELD_CONSTEXPR inline T1 last_sunday_month_day(T2 year, T3 month) {
        const T1 days = num_days_in_month(year, month);
        return days - day_of_week_date(year, month, days);
    }

    /// \brief Get the timestamp of the beginning of the week.
    ///
    /// This function finds the timestamp of the beginning of the week,
    /// which corresponds to the start of Sunday.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the beginning of the week.
    constexpr ts_t start_of_week(ts_t ts = time_shield::ts()) {
        return start_of_day(ts) - weekday_of_ts(ts) * SEC_PER_DAY;
    }

    /// \brief Get the timestamp of the end of the week.
    ///
    /// This function finds the timestamp of the end of the week,
    /// which corresponds to the end of Saturday.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the end of the week.
    constexpr ts_t end_of_week(ts_t ts = time_shield::ts()) {
        return start_of_day(ts) + (DAYS_PER_WEEK - weekday_of_ts(ts)) * SEC_PER_DAY - 1;
    }

    /// \brief Get the timestamp of the start of Saturday.
    ///
    /// This function finds the timestamp of the beginning of the day on Saturday,
    /// which corresponds to the start of Saturday.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the start of Saturday.
    constexpr ts_t start_of_saturday(ts_t ts = time_shield::ts()) {
        return start_of_day(ts) + (SAT - weekday_of_ts(ts)) * SEC_PER_DAY;
    }


    /// \brief Get the timestamp at the start of the hour.
    ///
    /// This function sets the minute and second to zero.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Timestamp at the start of the hour.
    constexpr ts_t start_of_hour(ts_t ts = time_shield::ts()) noexcept {
        return ts - detail::floor_mod(ts, SEC_PER_HOUR);
    }

    /// \brief Get the timestamp at the start of the hour.
    ///
    /// This function sets the minute and second to zero.
    ///
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
    /// \return Timestamp at the start of the hour in seconds.
    constexpr ts_t start_of_hour_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_hour(ms_to_sec<ts_t>(ts_ms));
    }

    /// \brief Get the timestamp at the start of the hour.
    /// This function sets the minute and second to zero.
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
    /// \return Timestamp at the start of the hour in milliseconds.
    constexpr ts_ms_t start_of_hour_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - detail::floor_mod(ts_ms, MS_PER_HOUR);
    }

    /// \brief Get the timestamp at the end of the hour.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the end of the hour.
    constexpr ts_t end_of_hour(ts_t ts = time_shield::ts()) noexcept {
        return ts - detail::floor_mod(ts, SEC_PER_HOUR) + SEC_PER_HOUR - 1;
    }

    /// \brief Get the timestamp at the end of the hour in seconds.
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp).
    /// \return Returns the timestamp of the end of the hour in seconds.
    constexpr ts_t end_of_hour_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_hour(ms_to_sec<ts_t>(ts_ms));
    }

    /// \brief Get the timestamp at the end of the hour in milliseconds.
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp).
    /// \return Returns the timestamp of the end of the hour in milliseconds.
    constexpr ts_ms_t end_of_hour_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - detail::floor_mod(ts_ms, MS_PER_HOUR) + MS_PER_HOUR - 1;
    }

    /// \brief Get the timestamp of the beginning of the minute.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the beginning of the minute.
    constexpr ts_t start_of_min(ts_t ts = time_shield::ts()) noexcept {
        return ts - detail::floor_mod(ts, SEC_PER_MIN);
    }

    /// \brief Get the timestamp of the end of the minute.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the end of the minute.
    constexpr ts_t end_of_min(ts_t ts = time_shield::ts()) noexcept {
        return ts - detail::floor_mod(ts, SEC_PER_MIN) + SEC_PER_MIN - 1;
    }

    /// \brief Get minute of day.
    /// This function returns a value between 0 to 1439 (minute of day).
    /// \param ts Timestamp in seconds (default: current timestamp).
    /// \return Minute of day.
    template<class T = int>
    constexpr T min_of_day(ts_t ts = time_shield::ts()) noexcept {
        const ts_t minutes = detail::floor_div(ts, SEC_PER_MIN);
        return static_cast<T>(detail::floor_mod(minutes, MIN_PER_DAY));
    }

    /// \brief Get hour of day.
    /// This function returns a value between 0 to 23.
    /// \param ts Timestamp in seconds (default: current timestamp).
    /// \return Hour of day.
    template<class T = int>
    constexpr T hour_of_day(ts_t ts = time_shield::ts()) noexcept {
        const ts_t hours = detail::floor_div(ts, SEC_PER_HOUR);
        return static_cast<T>(detail::floor_mod(hours, HOURS_PER_DAY));
    }

    /// \brief Get minute of hour.
    /// This function returns a value between 0 to 59.
    /// \param ts Timestamp in seconds (default: current timestamp).
    /// \return Minute of hour.
    template<class T = int>
    constexpr T min_of_hour(ts_t ts = time_shield::ts()) noexcept {
        const ts_t minutes = detail::floor_div(ts, SEC_PER_MIN);
        return static_cast<T>(detail::floor_mod(minutes, MIN_PER_HOUR));
    }

    /// \brief Get the timestamp of the start of the period.
    /// \param p Period duration in seconds.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the start of the period.
    template<class T = int>
    constexpr ts_t start_of_period(T p, ts_t ts = time_shield::ts()) {
        return ts - detail::floor_mod(ts, static_cast<ts_t>(p));
    }

    /// \brief Get the timestamp of the end of the period.
    /// \param p Period duration in seconds.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the end of the period.
    template<class T = int>
    constexpr ts_t end_of_period(T p, ts_t ts = time_shield::ts()) {
        return ts - detail::floor_mod(ts, static_cast<ts_t>(p)) + p - 1;
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_DATE_TIME_CONVERSIONS_HPP_INCLUDED
