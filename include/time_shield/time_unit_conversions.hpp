// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_UNIT_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_UNIT_CONVERSIONS_HPP_INCLUDED

/// \file time_unit_conversions.hpp
/// \brief Helper functions for unit conversions between seconds, minutes, hours, and milliseconds.

#include "config.hpp"
#include "constants.hpp"
#include "types.hpp"

#include <cmath>
#include <type_traits>

namespace time_shield {

/// \ingroup time_conversions
/// \{

    /// \brief Get the nanosecond part of the second from a floating-point timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in floating-point seconds.
    /// \return T Nanosecond part of the second.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR T ns_of_sec(fts_t ts) noexcept {
        fts_t temp = 0;
        return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(NS_PER_SEC)));
    }

    /// \brief Get the microsecond part of the second from a floating-point timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in floating-point seconds.
    /// \return T Microsecond part of the second.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR T us_of_sec(fts_t ts) noexcept {
        fts_t temp = 0;
        return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(US_PER_SEC)));
    }

    /// \brief Get the millisecond part of the second from a floating-point timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in floating-point seconds.
    /// \return T Millisecond part of the second.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR T ms_of_sec(fts_t ts) noexcept {
        fts_t temp = 0;
        return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(MS_PER_SEC)));
    }

    /// \brief Get the millisecond part of the timestamp.
    /// \tparam T Type of the returned value (default is int).
    /// \param ts Timestamp in milliseconds.
    /// \return T Millisecond part of the timestamp.
    template<class T = int>
    constexpr T ms_of_ts(ts_ms_t ts) noexcept {
        return static_cast<T>(ts % MS_PER_SEC);
    }

#   ifndef TIME_SHIELD_CPP17
    /// \brief Helper function for converting seconds to milliseconds (floating-point version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in seconds.
    /// \param tag std::true_type indicates a floating-point type.
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t sec_to_ms_impl(T t, std::true_type) noexcept {
        return static_cast<ts_ms_t>(std::round(t * static_cast<T>(MS_PER_SEC)));
    }

    /// \brief Helper function for converting seconds to milliseconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in seconds.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t sec_to_ms_impl(T t, std::false_type) noexcept {
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
        return static_cast<T1>(sec_to_ms_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{}));
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
    constexpr ts_ms_t min_to_ms_impl(T t, std::true_type) noexcept {
        return static_cast<ts_ms_t>(std::round(t * static_cast<T>(MS_PER_MIN)));
    }

    /// \brief Helper function for converting minutes to milliseconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in minutes.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t min_to_ms_impl(T t, std::false_type) noexcept {
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
        return static_cast<T1>(min_to_ms_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{}));
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
    constexpr ts_t min_to_sec_impl(T t, std::true_type) noexcept {
        return static_cast<ts_t>(std::round(t * static_cast<T>(SEC_PER_MIN)));
    }

    /// \brief Helper function for converting minutes to seconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in minutes.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_t Timestamp in seconds.
    template<class T>
    constexpr ts_t min_to_sec_impl(T t, std::false_type) noexcept {
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
        return static_cast<T1>(min_to_sec_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{}));
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
    constexpr ts_ms_t hour_to_ms_impl(T t, std::true_type) noexcept {
        return static_cast<ts_ms_t>(std::round(t * static_cast<T>(MS_PER_HOUR)));
    }

    /// \brief Helper function for converting hours to milliseconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in hours.
    /// \param tag Type tag used to select the integral overload (must be std::false_type).
    /// \return ts_ms_t Timestamp in milliseconds.
    template<class T>
    constexpr ts_ms_t hour_to_ms_impl(T t, std::false_type) noexcept {
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
        return static_cast<T1>(hour_to_ms_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{}));
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
    constexpr ts_t hour_to_sec_impl(T t, std::true_type) noexcept {
        return static_cast<ts_t>(std::round(t * static_cast<T>(SEC_PER_HOUR)));
    }

    /// \brief Helper function for converting hours to seconds (integral version).
    /// \tparam T Type of the input timestamp.
    /// \param t Timestamp in hours.
    /// \param tag std::false_type indicates a non-floating-point type.
    /// \return ts_t Timestamp in seconds.
    template<class T>
    constexpr ts_t hour_to_sec_impl(T t, std::false_type) noexcept {
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
        return static_cast<T1>(hour_to_sec_impl(ts, typename std::conditional<
            (std::is_same<T2, double>::value || std::is_same<T2, float>::value),
            std::true_type,
            std::false_type
        >::type{}));
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

    /// \brief Converts a 24-hour format hour to a 12-hour format.
    /// \tparam T Numeric type of the hour (default is int).
    /// \param hour The hour in 24-hour format to convert.
    /// \return The hour in 12-hour format.
    template<class T = int>
    TIME_SHIELD_CONSTEXPR inline T hour24_to_12(T hour) noexcept {
        if (hour == 0 || hour > 12) return 12;
        return hour;
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_UNIT_CONVERSIONS_HPP_INCLUDED
