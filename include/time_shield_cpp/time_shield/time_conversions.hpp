#pragma once
#ifndef _TIME_SHIELD_TIME_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_CONVERSIONS_HPP_INCLUDED

/// \file time_conversions.hpp
/// \brief Header file for time conversion functions.
///
/// This file contains functions for converting between different time representations
/// and performing various time-related calculations.

#include "enums.hpp"
#include "validation.hpp"
#include "time_utils.hpp"
#include "time_zone_struct.hpp"
#include <cmath>
#include <ctime>
#include <stdexcept>

namespace time_shield {

/// \ingroup time_conversions
/// \{

    /// \brief Get the nanosecond part of the second from a floating-point timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in floating-point seconds.
    /// \return T Nanosecond part of the second.
    template<class T = int>
    constexpr T ns_of_sec(fts_t ts) noexcept {
        fts_t temp;
        return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(NS_PER_SEC)));
    }

    /// \brief Get the microsecond part of the second from a floating-point timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in floating-point seconds.
    /// \return T Microsecond part of the second.
    template<class T = int>
    constexpr T us_of_sec(fts_t ts) noexcept {
        fts_t temp;
        return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(US_PER_SEC)));
    }

    /// \brief Get the millisecond part of the second from a floating-point timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in floating-point seconds.
    /// \return T Millisecond part of the second.
    template<class T = int>
    constexpr T ms_of_sec(fts_t ts) noexcept {
        fts_t temp;
        return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(MS_PER_SEC)));
    }

    /// \brief Get the millisecond part of the timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in milliseconds.
    /// \return T Millisecond part of the timestamp.
    template<class T = int>
    constexpr T ms_of_ts(ts_ms_t ts) noexcept {
        return ts % MS_PER_SEC;
    }

#   ifndef TIME_SHIELD_CPP17
    /// \brief Helper function for converting seconds to milliseconds (floating-point version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in seconds.
    /// \param tag std::true_type indicates a floating-point type.
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t sec_to_ms_impl(T t, std::true_type tag) noexcept {
        return static_cast<ts_ms_t>(std::round(t * static_cast<T>(MS_PER_SEC)));
    }

    /// \brief Helper function for converting seconds to milliseconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in seconds.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t sec_to_ms_impl(T t, std::false_type tag) noexcept {
        return static_cast<ts_ms_t>(t) * static_cast<ts_ms_t>(MS_PER_SEC);
    }
#   endif // TIME_SHIELD_CPP17

    /// \brief Converts a timestamp from seconds to milliseconds.
    /// \tparam T1 The type of the output timestamp (default is ts_ms_t).
    /// \tparam T2 The type of the input timestamp.
    /// \param ts Timestamp in seconds.
    /// \return T1 Timestamp in milliseconds.
    template<class T1 = ts_ms_t, class T2>
    constexpr T1 sec_to_ms(T2 ts) noexcept {
#       ifdef TIME_SHIELD_CPP17
        if constexpr (std::is_floating_point_v<T2>) {
            return static_cast<T1>(std::round(ts * static_cast<T2>(MS_PER_SEC)));
        } else {
            return static_cast<T1>(ts) * static_cast<T1>(MS_PER_SEC);
        }
#       else
        return sec_to_ms_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{});
#       endif
    }

    /// \brief Converts a floating-point timestamp from seconds to milliseconds.
    /// \param ts Timestamp in floating-point seconds.
    /// \return ts_ms_t Timestamp in milliseconds.
    inline ts_ms_t fsec_to_ms(fts_t ts) noexcept {
        return static_cast<ts_ms_t>(std::round(ts * static_cast<fts_t>(MS_PER_SEC)));
    }

    /// \brief Converts a timestamp from milliseconds to seconds.
    /// \tparam T1 The type of the output timestamp (default is ts_t).
    /// \tparam T2 The type of the input timestamp (default is ts_ms_t).
    /// \param ts_ms Timestamp in milliseconds.
    /// \return T1 Timestamp in seconds.
    template<class T1 = ts_t, class T2 = ts_ms_t>
    constexpr T1 ms_to_sec(T2 ts_ms) noexcept {
        return static_cast<T1>(ts_ms) / static_cast<T1>(MS_PER_SEC);
    }

    /// \brief Converts a timestamp from milliseconds to floating-point seconds.
    /// \tparam T The type of the input timestamp (default is ts_ms_t).
    /// \param ts_ms Timestamp in milliseconds.
    /// \return fts_t Timestamp in floating-point seconds.
    template<class T = ts_ms_t>
    constexpr fts_t ms_to_fsec(T ts_ms) noexcept {
        return static_cast<fts_t>(ts_ms) / static_cast<fts_t>(MS_PER_SEC);
    }

//----------------------------------------------------------------------------//
// Minutes -> Milliseconds
//----------------------------------------------------------------------------//
#   ifndef TIME_SHIELD_CPP17
    /// \brief Helper function for converting minutes to milliseconds (floating-point version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in minutes.
    /// \param tag std::true_type indicates a floating-point type (double or float).
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t min_to_ms_impl(T t, std::true_type tag) noexcept {
        return static_cast<ts_ms_t>(std::round(t * static_cast<T>(MS_PER_MIN)));
    }

    /// \brief Helper function for converting minutes to milliseconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in minutes.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t min_to_ms_impl(T t, std::false_type tag) noexcept {
        return static_cast<ts_ms_t>(t) * static_cast<ts_ms_t>(MS_PER_MIN);
    }
#   endif // TIME_SHIELD_CPP17

    /// \brief Converts a timestamp from minutes to milliseconds.
    /// \tparam T1 The type of the output timestamp (default is ts_ms_t).
    /// \tparam T2 The type of the input timestamp.
    /// \param ts Timestamp in minutes.
    /// \return T1 Timestamp in milliseconds.
    template<class T1 = ts_ms_t, class T2>
    constexpr T1 min_to_ms(T2 ts) noexcept {
#       ifdef TIME_SHIELD_CPP17
        if constexpr (std::is_floating_point_v<T2>) {
            return static_cast<T1>(std::round(ts * static_cast<T2>(MS_PER_MIN)));
        } else {
            return static_cast<T1>(ts) * static_cast<T1>(MS_PER_MIN);
        }
#       else
        return min_to_ms_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{});
#       endif
    }

    /// \brief Converts a timestamp from milliseconds to minutes.
    /// \tparam T1 The type of the output timestamp (default is int).
    /// \tparam T2 The type of the input timestamp (default is ts_ms_t).
    /// \param ts Timestamp in milliseconds.
    /// \return T1 Timestamp in minutes.
    template<class T1 = int, class T2 = ts_ms_t>
    constexpr T1 ms_to_min(T2 ts) noexcept {
        return static_cast<T1>(ts) / static_cast<T1>(MS_PER_MIN);
    }

//----------------------------------------------------------------------------//
// Minutes -> Seconds
//----------------------------------------------------------------------------//
#   ifndef TIME_SHIELD_CPP17
    /// \brief Helper function for converting minutes to seconds (floating-point version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in minutes.
    /// \param tag std::true_type indicates a floating-point type (double or float).
    /// \return ts_t Timestamp in seconds.
    template<class T>
    constexpr ts_t min_to_sec_impl(T t, std::true_type tag) noexcept {
        return static_cast<ts_t>(std::round(t * static_cast<T>(SEC_PER_MIN)));
    }

    /// \brief Helper function for converting minutes to seconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in minutes.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_t Timestamp in seconds.
    template<class T>
    constexpr ts_t min_to_sec_impl(T t, std::false_type tag) noexcept {
        return static_cast<ts_t>(t) * static_cast<ts_t>(SEC_PER_MIN);
    }
#   endif // TIME_SHIELD_CPP17

    /// \brief Converts a timestamp from minutes to seconds.
    /// \tparam T1 The type of the output timestamp (default is ts_t).
    /// \tparam T2 The type of the input timestamp.
    /// \param ts Timestamp in minutes.
    /// \return T1 Timestamp in seconds.
    template<class T1 = ts_t, class T2>
    constexpr T1 min_to_sec(T2 ts) noexcept {
#       ifdef TIME_SHIELD_CPP17
        if constexpr (std::is_floating_point_v<T2>) {
            return static_cast<T1>(std::round(ts * static_cast<T2>(SEC_PER_MIN)));
        } else {
            return static_cast<T1>(ts) * static_cast<T1>(SEC_PER_MIN);
        }
#       else
        return min_to_sec_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{});
#       endif
    }

    /// \brief Converts a timestamp from seconds to minutes.
    /// \tparam T1 The type of the output timestamp (default is int).
    /// \tparam T2 The type of the input timestamp (default is ts_t).
    /// \param ts Timestamp in seconds.
    /// \return T1 Timestamp in minutes.
    template<class T1 = int, class T2 = ts_t>
    constexpr T1 sec_to_min(T2 ts) noexcept {
        return static_cast<T1>(ts) / static_cast<T1>(SEC_PER_MIN);
    }

    /// \brief Converts a timestamp from minutes to floating-point seconds.
    /// \tparam T The type of the input timestamp (default is int).
    /// \param min Timestamp in minutes.
    /// \return fts_t Timestamp in floating-point seconds.
    template<class T = int>
    constexpr fts_t min_to_fsec(T min) noexcept {
        return static_cast<fts_t>(min) * static_cast<fts_t>(SEC_PER_MIN);
    }

    /// \brief Converts a timestamp from seconds to floating-point minutes.
    /// \tparam T The type of the input timestamp (default is ts_t).
    /// \param ts Timestamp in seconds.
    /// \return double Timestamp in floating-point minutes.
    template<class T = ts_t>
    constexpr double sec_to_fmin(T ts) noexcept {
        return static_cast<double>(ts) / static_cast<double>(SEC_PER_MIN);
    }
    
//----------------------------------------------------------------------------//
// Hours -> Milliseconds
//----------------------------------------------------------------------------//

#   ifndef TIME_SHIELD_CPP17
    /// \brief Helper function for converting hours to milliseconds (floating-point version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in hours.
    /// \param tag std::true_type indicates a floating-point type (double or float).
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t hour_to_ms_impl(T t, std::true_type tag) noexcept {
        return static_cast<ts_ms_t>(std::round(t * static_cast<T>(MS_PER_HOUR)));
    }

    /// \brief Helper function for converting hours to milliseconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in hours.
    /// \param tag Type tag used to select the integral overload (must be std::false_type).
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t hour_to_ms_impl(T t, std::false_type tag) noexcept {
        return static_cast<ts_ms_t>(t) * static_cast<ts_ms_t>(MS_PER_HOUR);
    }
#   endif // TIME_SHIELD_CPP17

    /// \brief Converts a timestamp from hours to milliseconds.
    /// \tparam T1 The type of the output timestamp (default is ts_ms_t).
    /// \tparam T2 The type of the input timestamp.
    /// \param ts Timestamp in hours.
    /// \return T1 Timestamp in milliseconds.
    template<class T1 = ts_ms_t, class T2>
    constexpr T1 hour_to_ms(T2 ts) noexcept {
#       ifdef TIME_SHIELD_CPP17
        if constexpr (std::is_floating_point_v<T2>) {
            return static_cast<T1>(std::round(ts * static_cast<T2>(MS_PER_HOUR)));
        } else {
            return static_cast<T1>(ts) * static_cast<T1>(MS_PER_HOUR);
        }
#       else
        return hour_to_ms_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{});
#       endif
    }

    /// \brief Converts a timestamp from milliseconds to hours.
    /// \tparam T1 The type of the output timestamp (default is int).
    /// \tparam T2 The type of the input timestamp (default is ts_ms_t).
    /// \param ts Timestamp in milliseconds.
    /// \return T1 Timestamp in hours.
    template<class T1 = int, class T2 = ts_ms_t>
    constexpr T1 ms_to_hour(T2 ts) noexcept {
        return static_cast<T1>(ts) / static_cast<T1>(MS_PER_HOUR);
    }

//----------------------------------------------------------------------------//
// Hours -> Seconds
//----------------------------------------------------------------------------//

#   ifndef TIME_SHIELD_CPP17
    /// \brief Helper function for converting hours to seconds (floating-point version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in hours.
    /// \param tag std::true_type indicates a floating-point type (double or float).
    /// \return ts_t Timestamp in seconds.
    template<class T>
    constexpr ts_t hour_to_sec_impl(T t, std::true_type tag) noexcept {
        return static_cast<ts_t>(std::round(t * static_cast<T>(SEC_PER_HOUR)));
    }

    /// \brief Helper function for converting hours to seconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in hours.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_t Timestamp in seconds.
    template<class T>
    constexpr ts_t hour_to_sec_impl(T t, std::false_type tag) noexcept {
        return static_cast<ts_t>(t) * static_cast<ts_t>(SEC_PER_HOUR);
    }
#   endif // TIME_SHIELD_CPP17

    /// \brief Converts a timestamp from hours to seconds.
    /// \tparam T1 The type of the output timestamp (default is ts_t).
    /// \tparam T2 The type of the input timestamp.
    /// \param ts Timestamp in hours.
    /// \return T1 Timestamp in seconds.
    template<class T1 = ts_t, class T2>
    constexpr T1 hour_to_sec(T2 ts) noexcept {
#       ifdef TIME_SHIELD_CPP17
        if constexpr (std::is_floating_point_v<T2>) {
            return static_cast<T1>(std::round(ts * static_cast<T2>(SEC_PER_HOUR)));
        } else {
            return static_cast<T1>(ts) * static_cast<T1>(SEC_PER_HOUR);
        }
#       else
        return hour_to_sec_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{});
#       endif
    }

    /// \brief Converts a timestamp from seconds to hours.
    /// \tparam T1 The type of the output timestamp (default is int).
    /// \tparam T2 The type of the input timestamp (default is ts_t).
    /// \param ts Timestamp in seconds.
    /// \return T1 Timestamp in hours.
    template<class T1 = int, class T2 = ts_t>
    constexpr T1 sec_to_hour(T2 ts) noexcept {
        return static_cast<T1>(ts) / static_cast<T1>(SEC_PER_HOUR);
    }

    /// \brief Converts a timestamp from hours to floating-point seconds.
    /// \tparam T The type of the input timestamp (default is int).
    /// \param hr Timestamp in hours.
    /// \return fts_t Timestamp in floating-point seconds.
    template<class T = int>
    constexpr fts_t hour_to_fsec(T hr) noexcept {
        return static_cast<fts_t>(hr) * static_cast<fts_t>(SEC_PER_HOUR);
    }

    /// \brief Converts a timestamp from seconds to floating-point hours.
    /// \tparam T The type of the input timestamp (default is ts_t).
    /// \param ts Timestamp in seconds.
    /// \return double Timestamp in floating-point hours.
    template<class T = ts_t>
    constexpr double sec_to_fhour(T ts) noexcept {
        return static_cast<double>(ts) / static_cast<double>(SEC_PER_HOUR);
    }

//------------------------------------------------------------------------------

    /// \brief Converts a UNIX timestamp to a year.
    /// \tparam T The type of the year (default is year_t).
    /// \param ts UNIX timestamp.
    /// \return T Year corresponding to the given timestamp.
    template<class T = year_t>
    constexpr T get_unix_year(ts_t ts) noexcept {
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

//------------------------------------------------------------------------------

    /// \brief Converts a 24-hour format hour to a 12-hour format.
    /// \tparam T Numeric type of the hour (default is int).
    /// \param hour The hour in 24-hour format to convert.
    /// \return The hour in 12-hour format.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR inline T hour24_to_12(T hour) noexcept {
        if (hour == 0 || hour > 12) return 12;
        return hour;
    }

//------------------------------------------------------------------------------

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
        uint64_t secs = -((ts - BIAS_2000) - BIAS_292277022000);

        const uint64_t n_400_years = secs / SEC_PER_400_YEARS;
        secs -= n_400_years * SEC_PER_400_YEARS;
        y -= n_400_years * 400;

        const uint64_t n_100_years = secs / SEC_PER_100_YEARS;
        secs -= n_100_years * SEC_PER_100_YEARS;
        y -= n_100_years * 100;

        const uint64_t n_4_years = secs / SEC_PER_4_YEARS;
        secs -= n_4_years * SEC_PER_4_YEARS;
        y -= n_4_years * 4;

        const uint64_t n_1_years = secs / SEC_PER_YEAR;
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

        ts_t day_secs = static_cast<ts_t>(secs % SEC_PER_DAY);
        date_time.hour = static_cast<decltype(date_time.hour)>(day_secs / SEC_PER_HOUR);
        ts_t min_secs = static_cast<ts_t>(day_secs - date_time.hour * SEC_PER_HOUR);
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

//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Converts a timestamp in milliseconds to a date-time structure with milliseconds.
    /// \tparam T The type of the date-time structure to return.
    /// \param ts The timestamp in milliseconds to convert.
    /// \return T A date-time structure with the corresponding date and time components.
    template<class T>
    inline T to_date_time_ms(ts_ms_t ts) {
        T date_time = to_date_time<T, ts_ms_t>(ms_to_sec<ts_ms_t, ts_ms_t>(ts));
        date_time.ms = ms_of_ts(ts); // Extract and set the ms component
        return date_time;
    }

//------------------------------------------------------------------------------

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
        uint64_t years = (static_cast<int64_t>(MAX_YEAR) - year);

        const int64_t n_400_years = years / 400;
        secs += n_400_years * SEC_PER_400_YEARS;
        years -= n_400_years * 400;

        const int64_t n_100_years = years / 100;
        secs += n_100_years * SEC_PER_100_YEARS;
        years -= n_100_years * 100;

        const int64_t n_4_years = years / 4;
        secs += n_4_years * SEC_PER_4_YEARS;
        years -= n_4_years * 4;

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

//------------------------------------------------------------------------------

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
            date_time.sec);
    }

//------------------------------------------------------------------------------

    /// \brief Converts a std::tm structure to a timestamp.
    ///
    /// This function converts a standard C++ std::tm structure to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// \param timeinfo Pointer to a std::tm structure containing the date and time information.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    TIME_SHIELD_CONSTEXPR inline ts_t tm_to_timestamp(
            const std::tm *timeinfo) {
        return to_timestamp(
            timeinfo->tm_year + 1900,
            timeinfo->tm_mon + 1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);
    }

//------------------------------------------------------------------------------

    /// \brief Converts a date and time to a timestamp in milliseconds.
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
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
            T2 hour  = 0,
            T2 min   = 0,
            T2 sec   = 0,
            T2 ms    = 0) {
        return sec_to_ms(to_timestamp<T1, T2>(year, month, day, hour, min, sec)) + ms;
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Converts a date-time structure to a timestamp in milliseconds.
    ///
    /// This function converts a given date-time structure to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T The type of the date-time structure.
    /// \param date_time The date-time structure.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t dt_to_timestamp_ms(
            const T& date_time) {
        return sec_to_ms(dt_to_timestamp(date_time)) + date_time.ms;
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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
        return static_cast<fts_t>(to_timestamp(year, month, day, hour, min, sec)) +
            static_cast<fts_t>(ms)/static_cast<fts_t>(MS_PER_SEC);
    }

//------------------------------------------------------------------------------

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
        return static_cast<fts_t>(to_timestamp(date_time)) +
            static_cast<fts_t>(date_time.ms)/static_cast<fts_t>(MS_PER_SEC);
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

    /// \brief Get UNIX day.
    ///
    /// This function returns the number of days elapsed since the UNIX epoch.
    ///
    /// \tparam T The return type of the function (default is unixday_t).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Number of days since the UNIX epoch.
    template<class T = uday_t>
    constexpr T get_unix_day(ts_t ts = time_shield::ts()) noexcept {
        return ts / SEC_PER_DAY;
    }

//------------------------------------------------------------------------------

    /// \brief Get the number of days between two timestamps.
    ///
    /// This function calculates the number of days between two timestamps.
    ///
    /// \tparam T The type of the return value, defaults to int.
    /// \param start The timestamp of the start of the period.
    /// \param stop The timestamp of the end of the period.
    /// \return The number of days between start and stop.
    template<class T = int>
    constexpr T get_days_difference(ts_t start, ts_t stop) noexcept {
        return (stop - start) / SEC_PER_DAY;
    }

//------------------------------------------------------------------------------

    /// \brief Get UNIX day from milliseconds timestamp.
    ///
    /// This function returns the number of days elapsed since the UNIX epoch, given a timestamp in milliseconds.
    ///
    /// \tparam T The return type of the function (default is unixday_t).
    /// \param t_ms Timestamp in milliseconds (default is current timestamp in milliseconds).
    /// \return Number of days since the UNIX epoch.
    template<class T = uday_t>
    constexpr T get_unix_day_ms(ts_ms_t t_ms = time_shield::ts_ms()) noexcept {
        return get_unix_day(ms_to_sec(t_ms));
    }

//------------------------------------------------------------------------------

    /// \brief Converts a UNIX day to a timestamp in seconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding
    /// timestamp in seconds at the start of the specified day.
    ///
    /// \tparam T The return type of the function (default is ts_t).
    /// \param unix_day Number of days since the UNIX epoch.
    /// \return The timestamp in seconds representing the beginning of the specified UNIX day.
    template<class T = ts_t>
    constexpr T unix_day_to_timestamp(uday_t unix_day) noexcept {
        return unix_day * SEC_PER_DAY;
    }

//------------------------------------------------------------------------------
    
    /// \brief Converts a UNIX day to a timestamp in milliseconds.
    ///
    /// Converts a number of days since the UNIX epoch (January 1, 1970) to the corresponding timestamp
    /// in milliseconds at the start of the specified day.
    ///
    /// \tparam T The return type of the function (default is ts_t).
    /// \param unix_day Number of days since the UNIX epoch.
    /// \return The timestamp in milliseconds representing the beginning of the specified UNIX day.
    template<class T = ts_t>
    constexpr T unix_day_to_timestamp_ms(uday_t unix_day) noexcept {
        return unix_day * MS_PER_DAY;
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

    /// \brief Get UNIX minute.
    ///
    /// This function returns the number of minutes elapsed since the UNIX epoch.
    ///
    /// \tparam T The return type of the function (default is int64_t).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Number of minutes since the UNIX epoch.
    template<class T = int64_t>
    constexpr T get_unix_min(ts_t ts = ts()) {
        return ts / SEC_PER_MIN;
    }

//------------------------------------------------------------------------------

    /// \brief Get the second of the day.
    ///
    /// This function returns a value from 0 to MAX_SEC_PER_DAY representing the second of the day.
    ///
    /// \tparam T The return type of the function (default is int).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Second of the day.
    template<class T = int>
    constexpr T sec_of_day(ts_t ts = ts()) noexcept {
        return ts % SEC_PER_DAY;
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
        return sec_of_day(ms_to_sec(ts_ms));
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
        return hour * SEC_PER_HOUR + min * SEC_PER_MIN + sec;
    }

    /// \brief Get the second of the minute.
    ///
    /// This function returns a value between 0 and 59 representing the second of the minute.
    ///
    /// \tparam T The return type of the function (default is int).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Second of the minute.
    template<class T = int>
    constexpr T sec_of_min(ts_t ts = ts()) {
        return (ts % SEC_PER_MIN);
    }

    /// \brief Get the second of the hour.
    ///
    /// This function returns a value between 0 and 3599 representing the second of the hour.
    ///
    /// \tparam T The return type of the function (default is int).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Second of the hour.
    template<class T = int>
    constexpr T sec_of_hour(ts_t ts = ts()) {
        return (ts % SEC_PER_HOUR);
    }

//------------------------------------------------------------------------------

    /// \brief Get the year from the timestamp.
    ///
    /// This function returns the year of the specified timestamp in seconds since the Unix epoch.
    ///
    /// \tparam T The return type of the function (default is year_t).
    /// \param ts Timestamp in seconds (default is current timestamp).
    /// \return Year of the specified timestamp.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T get_year(ts_t ts = ts()) {
        return get_unix_year(ts) + UNIX_EPOCH;
    }

//------------------------------------------------------------------------------

    /// \brief Get the year from the timestamp in milliseconds.
    ///
    /// This function returns the year of the specified timestamp in milliseconds since the Unix epoch.
    ///
    /// \tparam T The return type of the function (default is year_t).
    /// \param ts_ms Timestamp in milliseconds (default is current timestamp).
    /// \return Year of the specified timestamp.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T get_year_ms(ts_ms_t ts_ms = ts_ms()) {
        return get_year(ms_to_sec(ts_ms));
    }

//------------------------------------------------------------------------------

    /// \brief Get the start of the year timestamp.
    ///
    /// This function resets the days, months, hours, minutes, and seconds of the given timestamp
    /// to the beginning of the year.
    ///
    /// \param ts Timestamp.
    /// \return Start of the year timestamp.
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_year(ts_t ts) noexcept {
        constexpr ts_t BIAS_2100 = 4102444800;
        if (ts < BIAS_2100) {
            constexpr ts_t SEC_PER_YEAR_X2 = SEC_PER_YEAR * 2;
            ts_t year_start_ts = ts % SEC_PER_4_YEARS;
            if (year_start_ts < SEC_PER_YEAR) {
                return ts - year_start_ts;
            } else
            if (year_start_ts < SEC_PER_YEAR_X2) {
                return ts + SEC_PER_YEAR - year_start_ts;
            } else
            if (year_start_ts < (SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR)) {
                return ts + SEC_PER_YEAR_X2 - year_start_ts;
            }
            return ts + (SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR) - year_start_ts;
        }

        constexpr ts_t BIAS_2000 = 946684800;
        ts_t secs = ts - BIAS_2000;

        ts_t offset_y400 = secs % SEC_PER_400_YEARS;
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
                start_ts += secs - secs % SEC_PER_YEAR;
                return start_ts;
            }
        }

        ts_t offset_4y = secs % SEC_PER_4_YEARS;
        start_ts += secs - offset_4y;
        secs = offset_4y;

        if (secs >= SEC_PER_LEAP_YEAR) {
            secs -= SEC_PER_LEAP_YEAR;
            start_ts += SEC_PER_LEAP_YEAR;
            start_ts += secs - secs % SEC_PER_YEAR;
        }
        return start_ts;
    }

//------------------------------------------------------------------------------

    /// \brief Get the start of the year timestamp in milliseconds.
    ///
    /// This function resets the days, months, hours, minutes, and seconds of the given timestamp
    /// to the beginning of the year.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Start of year timestamp in milliseconds.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t start_of_year_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return sec_to_ms(start_of_year(ms_to_sec(ts_ms)));
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the start of the year.
    ///
    /// This function returns the timestamp at the start of the specified year.
    ///
    /// \param year Year.
    /// \return Timestamp of the start of the year.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_year_date(T year) {
        if (year < 2100) {
            const ts_t year_diff = year >= UNIX_EPOCH ? year - UNIX_EPOCH : UNIX_EPOCH - year;
            const ts_t year_start_ts = (year_diff / 4) * SEC_PER_4_YEARS;
            const ts_t year_remainder = year_diff % 4;
            constexpr ts_t SEC_PER_YEAR_X2 = 2 * SEC_PER_YEAR;
            constexpr ts_t SEC_PER_YEAR_V2 = SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR;
            switch (year_remainder) {
                case 0: return year_start_ts;
                case 1: return year_start_ts + SEC_PER_YEAR;
                case 2: return year_start_ts + SEC_PER_YEAR_X2;
                default: break;
            };
            return year_start_ts + SEC_PER_YEAR_V2;
        }
        return to_timestamp(year, 1, 1);
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

    /// \brief Get the end-of-year timestamp.
    ///
    /// This function finds the last timestamp of the current year.
    ///
    /// \param ts Timestamp.
    /// \return End-of-year timestamp.
    TIME_SHIELD_CONSTEXPR inline ts_t end_of_year(ts_t ts = time_shield::ts()) {
        constexpr ts_t BIAS_2100 = 4102444800;
        if (ts < BIAS_2100) {
            constexpr ts_t SEC_PER_YEAR_X2 = SEC_PER_YEAR * 2;
            constexpr ts_t SEC_PER_YEAR_X3 = SEC_PER_YEAR * 3;
            constexpr ts_t SEC_PER_YEAR_X3_V2 = SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR;
            ts_t year_end_ts = ts % SEC_PER_4_YEARS;
            if (year_end_ts < SEC_PER_YEAR) {
                return ts + SEC_PER_YEAR - year_end_ts - 1;
            } else
            if (year_end_ts < SEC_PER_YEAR_X2) {
                return ts + SEC_PER_YEAR_X2 - year_end_ts - 1;
            } else
            if (year_end_ts < SEC_PER_YEAR_X3_V2) {
                return ts + SEC_PER_YEAR_X3_V2 - year_end_ts - 1;
            }
            return ts + (SEC_PER_YEAR_X3 + SEC_PER_LEAP_YEAR) - year_end_ts - 1;
        }

        constexpr ts_t BIAS_2000 = 946684800;
        ts_t secs = ts - BIAS_2000;

        ts_t offset_y400 = secs % SEC_PER_400_YEARS;
        ts_t end_ts = secs - offset_y400 + BIAS_2000;
        secs = offset_y400;

        if (secs >= SEC_PER_FIRST_100_YEARS) {
            secs -= SEC_PER_FIRST_100_YEARS;
            end_ts += SEC_PER_FIRST_100_YEARS;
            while (secs >= SEC_PER_100_YEARS) {
                secs -= SEC_PER_100_YEARS;
                end_ts += SEC_PER_100_YEARS;
            }

            constexpr ts_t SEC_PER_4_YEARS_V2 = 4 * SEC_PER_YEAR;
            if (secs >= SEC_PER_4_YEARS_V2) {
                secs -= SEC_PER_4_YEARS_V2;
                end_ts += SEC_PER_4_YEARS_V2;
            } else {
                end_ts += secs - secs % SEC_PER_YEAR;
                return end_ts + SEC_PER_YEAR - 1;
            }
        }

        ts_t offset_4y = secs % SEC_PER_4_YEARS;
        end_ts += secs - offset_4y;
        secs = offset_4y;

        if (secs >= SEC_PER_LEAP_YEAR) {
            secs -= SEC_PER_LEAP_YEAR;
            end_ts += SEC_PER_LEAP_YEAR;
            end_ts += secs - secs % SEC_PER_YEAR;
            end_ts += SEC_PER_YEAR;
        } else {
            end_ts += SEC_PER_LEAP_YEAR;
        }
        return end_ts - 1;
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp in milliseconds of the end of the year.
    ///
    /// This function finds the last timestamp of the current year in milliseconds.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return End-of-year timestamp in milliseconds.
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_ms_t end_of_year_ms(ts_ms_t ts_ms = ts_ms()) {
        return sec_to_ms(end_of_year(ms_to_sec(ts_ms)));
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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
        const size_t dy = day_of_year(ts);
        return static_cast<T>((is_leap_year(ts) && dy >= JAN_AND_FEB_DAY_LEAP_YEAR) ? TABLE_MONTH_OF_YEAR[dy - 1] : TABLE_MONTH_OF_YEAR[dy]);
    }

//------------------------------------------------------------------------------

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
        const size_t dy = day_of_year(ts);
        if(is_leap_year(ts)) {
            if(dy == JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1] + 1;
            if(dy > JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1];
        }
        return TABLE_DAY_OF_YEAR[dy];
    }

//------------------------------------------------------------------------------

    /// \brief Get the number of days in a month.
    ///
    /// This function calculates and returns the number of days in the specified month and year.
    ///
    /// \param year Year as an integer.
    /// \param month Month as an integer.
    /// \return The number of days in the given month and year.
    template<class T1 = int, class T2 = year_t, class T3 = int>
    constexpr T1 num_days_in_month(T2 year, T3 month) noexcept {
        if (month > MONTHS_PER_YEAR || month < 0) return 0;
        constexpr T1 num_days[13] = {0,31,30,31,30,31,30,31,31,30,31,30,31};
        if (month == FEB) {
            if (is_leap_year_date(year)) return 29;
            return 28;
        }
        return num_days[month];
    }

//------------------------------------------------------------------------------

    /// \brief Get the number of days in the month of the given timestamp.
    ///
    /// This function calculates and returns the number of days in the month of the specified timestamp.
    ///
    /// \param ts The timestamp to extract month and year from.
    /// \return The number of days in the month of the given timestamp.
    template<class T1 = int>
    TIME_SHIELD_CONSTEXPR T1 num_days_in_month_ts(ts_t ts = time_shield::ts()) noexcept {
        constexpr T1 num_days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        const int month = month_of_year(ts);
        if (month == FEB) {
            return is_leap_year(ts) ? 29 : 28;
        }
        return num_days[month];
    }

//------------------------------------------------------------------------------

    /// \brief Get the number of days in a given year.
    ///
    /// This function calculates and returns the number of days in the specified year.
    ///
    /// \param year Year.
    /// \return Number of days in the given year.
    template<class T1 = int, class T2 = year_t>
    constexpr T1 num_days_in_year(T2 year) noexcept {
        if (is_leap_year_date(year)) return DAYS_PER_LEAP_YEAR;
        return DAYS_PER_YEAR;
    }

//------------------------------------------------------------------------------

    /// \brief Get the number of days in the current year.
    ///
    /// This function calculates and returns the number of days in the current year based on the provided timestamp.
    ///
    /// \param ts Timestamp.
    /// \return Number of days in the current year.
    template<class T = int>
    constexpr T num_days_in_year_ts(ts_t ts = time_shield::ts()) {
        if (is_leap_year_ts(ts)) return DAYS_PER_LEAP_YEAR;
        return DAYS_PER_YEAR;
    }

//------------------------------------------------------------------------------

    /// \brief Get the start of the day timestamp.
    ///
    /// This function returns the timestamp at the start of the day.
    /// The function sets the hours, minutes, and seconds to zero.
    ///
    /// \param ts Timestamp.
    /// \return Start of the day timestamp.
    constexpr ts_t start_of_day(ts_t ts = time_shield::ts()) noexcept {
        return ts - (ts % SEC_PER_DAY);
    }

//------------------------------------------------------------------------------

    /// \brief Get timestamp of the start of the previous day.
    ///
    /// This function returns the timestamp at the start of the previous day.
    ///
    /// \param ts Timestamp of the current day.
    /// \param days Number of days to go back (default is 1).
    /// \return Timestamp of the start of the previous day.
    template<class T = int>
    constexpr ts_t start_of_prev_day(ts_t ts = time_shield::ts(), T days = 1) noexcept {
        return ts - (ts % SEC_PER_DAY) - SEC_PER_DAY * days;
    }

//------------------------------------------------------------------------------

    /// \brief Get the start of the day timestamp in seconds.
    ///
    /// This function returns the timestamp at the start of the day in seconds.
    /// The function sets the hours, minutes, and seconds to zero.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Start of the day timestamp in seconds.
    constexpr ts_t start_of_day_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_day(ms_to_sec(ts_ms));
    }

//------------------------------------------------------------------------------

    /// \brief Get the start of the day timestamp in milliseconds.
    ///
    /// This function returns the timestamp at the start of the day in milliseconds.
    /// The function sets the hours, minutes, seconds, and milliseconds to zero.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Start of the day timestamp in milliseconds.
    constexpr ts_ms_t start_of_day_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - (ts_ms % MS_PER_DAY);
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the end of the day.
    ///
    /// This function sets the hour to 23, minute to 59, and second to 59.
    ///
    /// \param ts Timestamp.
    /// \return Timestamp at the end of the day.
    constexpr ts_t end_of_day(ts_t ts = time_shield::ts()) noexcept {
        return ts - (ts % SEC_PER_DAY) + SEC_PER_DAY - 1;
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the end of the day in seconds.
    ///
    /// This function sets the hour to 23, minute to 59, and second to 59.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at the end of the day in seconds.
    constexpr ts_t end_of_day_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_day(ms_to_sec(ts_ms));
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the end of the day in milliseconds.
    ///
    /// This function sets the hour to 23, minute to 59, second to 59, and millisecond to 999.
    ///
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at the end of the day in milliseconds.
    constexpr ts_ms_t end_of_day_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - (ts_ms % MS_PER_DAY) + MS_PER_DAY - 1;
    }

//------------------------------------------------------------------------------

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
    constexpr T1 day_of_week_date(T2 year, T3 month, T4 day) {
        year_t a, y, m, R;
        a = (14 - month) / MONTHS_PER_YEAR;
        y = year - a;
        m = month + MONTHS_PER_YEAR * a - 2;
        R = 7000 + ( day + y + (y / 4) - (y / 100) + (y / 400) + (31 * m) / MONTHS_PER_YEAR);
        return static_cast<T1>(R % DAYS_PER_WEEK);
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Get the day of the week from a date structure.
    ///
    /// This function takes a date structure with fields 'year', 'mon', and 'day',
    /// and returns the day of the week (SUN = 0, MON = 1, ... SAT = 6).
    ///
    /// \param date Structure containing year, month, and day.
    /// \return Day of the week (SUN = 0, MON = 1, ... SAT = 6).
    template<class T1 = Weekday, class T2>
    constexpr T1 get_weekday_from_date(const T2& date) {
        return day_of_week_date(date.year, date.mon, date.day);
    }

//------------------------------------------------------------------------------

    /// \brief Get the weekday from a timestamp.
    /// \param ts Timestamp.
    /// \return Weekday (SUN = 0, MON = 1, ... SAT = 6).
    template<class T = Weekday>
    constexpr T get_weekday_from_ts(ts_t ts) noexcept {
        return static_cast<T>((ts / SEC_PER_DAY + THU) % DAYS_PER_WEEK);
    }

//------------------------------------------------------------------------------

    /// \brief Get the weekday from a timestamp in milliseconds.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Weekday (SUN = 0, MON = 1, ... SAT = 6).
    template<class T = Weekday>
    constexpr T get_weekday_from_ts_ms(ts_ms_t ts_ms) {
        return get_weekday_from_ts(ms_to_sec(ts_ms));
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the last Sunday of the current month.
    ///
    /// This function returns the timestamp of the last Sunday of the current month,
    /// setting the day to the last Sunday and the time to 00:00:00.
    ///
    /// \param ts Timestamp (default is current timestamp)
    /// \return Timestamp of the last Sunday of the current month
    TIME_SHIELD_CONSTEXPR inline ts_t last_sunday_of_month(ts_t ts = time_shield::ts()) {
        return end_of_month(ts) - get_weekday_from_ts(ts) * SEC_PER_DAY;
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the start of the hour.
    ///
    /// This function sets the minute and second to zero.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Timestamp at the start of the hour.
    constexpr ts_t start_of_hour(ts_t ts = time_shield::ts()) noexcept {
        return ts - (ts % SEC_PER_HOUR);
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the start of the hour.
    ///
    /// This function sets the minute and second to zero.
    ///
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
    /// \return Timestamp at the start of the hour in seconds.
    constexpr ts_t start_of_hour_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_hour(ms_to_sec(ts_ms));
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the start of the hour.
    /// This function sets the minute and second to zero.
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
    /// \return Timestamp at the start of the hour in milliseconds.
    constexpr ts_ms_t start_of_hour_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - (ts_ms % MS_PER_HOUR);
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the end of the hour.
    /// This function sets the minute and second to 59.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Timestamp at the end of the hour.
    constexpr ts_t end_of_hour(ts_t ts = time_shield::ts()) noexcept {
        return ts - (ts % SEC_PER_HOUR) + SEC_PER_HOUR - 1;
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the end of the hour.
    ///
    /// This function sets the minute and second to 59.
    ///
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
    /// \return Timestamp at the end of the hour in seconds.
    constexpr ts_t end_of_hour_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_hour(ms_to_sec(ts_ms));
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp at the end of the hour.
    ///
    /// This function sets the minute and second to 59.
    ///
    /// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
    /// \return Timestamp at the end of the hour in milliseconds.
    constexpr ts_ms_t end_of_hour_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return ts_ms - (ts_ms % MS_PER_HOUR) + MS_PER_HOUR - 1;
    }

//------------------------------------------------------------------------------

    /// \brief Get the hour of the day.
    ///
    /// This function returns a value between 0 to 23 representing the hour of the day.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Hour of the day.
    template<class T = int>
    constexpr T hour_of_day(ts_t ts = time_shield::ts()) noexcept {
        return ((ts / SEC_PER_HOUR) % HOURS_PER_DAY);
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the beginning of the week.
    ///
    /// This function finds the timestamp of the beginning of the week,
    /// which corresponds to the start of Sunday.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the beginning of the week.
    constexpr ts_t start_of_week(ts_t ts = time_shield::ts()) {
        return start_of_day(ts) - get_weekday_from_ts(ts) * SEC_PER_DAY;
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the end of the week.
    ///
    /// This function finds the timestamp of the end of the week,
    /// which corresponds to the end of Saturday.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the end of the week.
    constexpr ts_t end_of_week(ts_t ts = time_shield::ts()) {
        return start_of_day(ts) + (DAYS_PER_WEEK - get_weekday_from_ts(ts)) * SEC_PER_DAY - 1;
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the start of Saturday.
    ///
    /// This function finds the timestamp of the beginning of the day on Saturday,
    /// which corresponds to the start of Saturday.
    ///
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the start of Saturday.
    constexpr ts_t start_of_saturday(ts_t ts = time_shield::ts()) {
        return start_of_day(ts) + (SAT - get_weekday_from_ts(ts)) * SEC_PER_DAY;
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the beginning of the minute.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the beginning of the minute.
    constexpr ts_t start_of_min(ts_t ts = time_shield::ts()) noexcept {
        return ts - (ts % SEC_PER_MIN);
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the end of the minute.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the end of the minute.
    constexpr ts_t end_of_min(ts_t ts = time_shield::ts()) noexcept {
        return ts - (ts % SEC_PER_MIN) + SEC_PER_MIN - 1;
    }

//------------------------------------------------------------------------------

    /// \brief Get minute of day.
    /// This function returns a value between 0 to 1439 (minute of day).
    /// \param ts Timestamp in seconds (default: current timestamp).
    /// \return Minute of day.
    template<class T = int>
    constexpr T min_of_day(ts_t ts = time_shield::ts()) noexcept {
        return ((ts / SEC_PER_MIN) % MIN_PER_DAY);
    }

//------------------------------------------------------------------------------

    /// \brief Get minute of hour.
    /// This function returns a value between 0 to 59.
    /// \param ts Timestamp in seconds (default: current timestamp).
    /// \return Minute of hour.
    template<class T = int>
    constexpr T min_of_hour(ts_t ts = time_shield::ts()) noexcept {
        return ((ts / SEC_PER_MIN) % MIN_PER_HOUR);
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the start of the period.
    /// \param p Period duration in seconds.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the start of the period.
    template<class T = int>
    constexpr ts_t start_of_period(T p, ts_t ts = time_shield::ts()) {
        return ts - (ts % p);
    }

//------------------------------------------------------------------------------

    /// \brief Get the timestamp of the end of the period.
    /// \param p Period duration in seconds.
    /// \param ts Timestamp (default: current timestamp).
    /// \return Returns the timestamp of the end of the period.
    template<class T = int>
    constexpr ts_t end_of_period(T p, ts_t ts = time_shield::ts()) {
        return ts - (ts % p) + p - 1;
    }

//------------------------------------------------------------------------------

    /// \brief Converts an integer to a time zone structure.
    /// \tparam T The type of the time zone structure (default is TimeZoneStruct).
    /// \param offset The integer to convert.
    /// \return A time zone structure of type T represented by the given integer.
    /// \details The function assumes that the type T has members `hour`, `min`, and `is_positive`.
    template<class T = TimeZoneStruct>
    inline T to_time_zone(tz_t offset) {
        T tz;
        int abs_val = std::abs(offset);
        tz.hour = abs_val / SEC_PER_HOUR;
        tz.min = abs_val % SEC_PER_MIN;
        tz.is_positive = (offset >= 0);
        return tz;
    }

/// \}

}; // namespace time_shield

#include "time_conversion_aliases.hpp"

#endif // _TIME_SHIELD_TIME_CONVERSIONS_HPP_INCLUDED
