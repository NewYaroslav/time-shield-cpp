// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_ZONE_OFFSET_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_ZONE_OFFSET_CONVERSIONS_HPP_INCLUDED

/// \file time_zone_offset_conversions.hpp
/// \brief Conversions between numeric offsets and TimeZoneStruct.

#include "config.hpp"
#include "constants.hpp"
#include "time_zone_struct.hpp"
#include "types.hpp"

namespace time_shield {

/// \ingroup time_conversions
/// \{

    /// \brief Converts an integer to a time zone structure.
    /// \tparam T The type of the time zone structure (default is TimeZoneStruct).
    /// \param offset The integer to convert.
    /// \return A time zone structure of type T represented by the given integer.
    /// \details The function assumes that the type T has members `hour`, `min`, and `is_positive`.
    template<class T = TimeZoneStruct>
    inline T to_time_zone(tz_t offset) {
        const int64_t off = static_cast<int64_t>(offset);
        const int64_t abs_val = (off < 0) ? -off : off;

        T tz;
        tz.hour = static_cast<decltype(tz.hour)>(abs_val / static_cast<int64_t>(SEC_PER_HOUR));
        tz.min  = static_cast<decltype(tz.min)>(
            (abs_val % static_cast<int64_t>(SEC_PER_HOUR)) / static_cast<int64_t>(SEC_PER_MIN)
        );
        tz.is_positive = (off >= 0);
        return tz;
    }

    /// \brief Convert time zone struct to offset in seconds.
    /// \details Expects fields: hour, min, is_positive.
    template<class T>
    TIME_SHIELD_CONSTEXPR inline tz_t to_tz_offset(const T& tz) noexcept {
        const int sign = tz.is_positive ? 1 : -1;
        const int64_t sec = static_cast<int64_t>(tz.hour) * SEC_PER_HOUR
                          + static_cast<int64_t>(tz.min)  * SEC_PER_MIN;
        return static_cast<tz_t>(sign * sec);
    }

    /// \brief Build offset in seconds from hours/minutes.
    /// \param hour Signed hours (e.g. -3, +5).
    /// \param min  Minutes (0..59).
    TIME_SHIELD_CONSTEXPR inline tz_t tz_offset_hm(int hour, int min = 0) noexcept {
        const int sign = (hour < 0) ? -1 : 1;
        const int64_t ah = (hour < 0) ? -static_cast<int64_t>(hour) : static_cast<int64_t>(hour);
        const int64_t am = (min  < 0) ? -static_cast<int64_t>(min)  : static_cast<int64_t>(min);
        return static_cast<tz_t>(sign * (ah * SEC_PER_HOUR + am * SEC_PER_MIN));
    }

    /// \brief Check if a numeric offset is within supported bounds.
    /// \details Conservative range: [-12:00, +14:00].
    TIME_SHIELD_CONSTEXPR inline bool is_valid_tz_offset(tz_t off) noexcept {
        // conservative range: [-12:00, +14:00]
        return off % 60 == 0
            && off >= -12 * SEC_PER_HOUR
            && off <=  14 * SEC_PER_HOUR;
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_ZONE_OFFSET_CONVERSIONS_HPP_INCLUDED
