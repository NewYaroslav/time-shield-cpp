// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_ZONE_STRUCT_HPP_INCLUDED
#define _TIME_SHIELD_TIME_ZONE_STRUCT_HPP_INCLUDED

/// \file time_zone_struct.hpp
/// \brief Header for time zone structure and related functions.

#include "config.hpp"
#include "types.hpp"
#include "constants.hpp"

#include <string>

namespace time_shield {

    /// \ingroup time_structures
    /// \brief Structure to represent time zone information.
    /// \details
    /// This structure contains the hour and minute components of a time zone offset,
    /// as well as a boolean indicating whether the offset is positive or negative.
    struct TimeZoneStruct {
        int hour;           ///< Hour component of time (0-23)
        int min;            ///< Minute component of time (0-59)
        bool is_positive;   ///< True if the time zone offset is positive, false if negative
    };

    /// \ingroup time_structures
    /// \brief Creates a TimeZoneStruct instance.
    /// \param hour The hour component of the time.
    /// \param min The minute component of the time.
    /// \param is_positive True if the time zone offset is positive, false if negative.
    /// \return A TimeZoneStruct instance with the provided time components.
    inline TimeZoneStruct create_time_zone_struct(
            int hour,
            int min,
            bool is_positive = true) {
        return TimeZoneStruct{hour, min, is_positive};
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures_time_conversions
    /// \brief Converts an integer to a TimeZoneStruct.
    /// \param offset The integer to convert.
    /// \return A TimeZoneStruct represented by the given integer.
    inline TimeZoneStruct to_time_zone_struct(tz_t offset) {
        const int64_t off = static_cast<int64_t>(offset);
        const int64_t abs_val = (off < 0) ? -off : off;

        const int hour = static_cast<int>(abs_val / static_cast<int64_t>(SEC_PER_HOUR));
        const int min  = static_cast<int>((abs_val % static_cast<int64_t>(SEC_PER_HOUR)) /
                                          static_cast<int64_t>(SEC_PER_MIN));

        return TimeZoneStruct{hour, min, off >= 0};
    }


    /// \ingroup time_structures_time_conversions
    /// \brief Alias for to_time_zone_struct function.
    /// \copydoc to_time_zone_struct
    inline TimeZoneStruct to_tz(tz_t offset) {
        return to_time_zone_struct(offset);
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures_time_formatting
    /// \brief Converts a TimeZoneStruct to a string representation.
    /// \param tz The TimeZoneStruct to convert.
    /// \return A string representation of the TimeZoneStruct.
    inline std::string time_zone_struct_to_string(const TimeZoneStruct& tz) {
        char sign = tz.is_positive ? '+' : '-';
        return std::string(1, sign) + (tz.hour < 10 ? "0" : "") + std::to_string(tz.hour) + ":" + (tz.min < 10 ? "0" : "") + std::to_string(tz.min);
    }

    /// \ingroup time_structures_time_formatting
    /// \brief Alias for time_zone_struct_to_string function.
    /// \copydoc time_zone_struct_to_string
    inline std::string to_string(const TimeZoneStruct& tz) {
        return time_zone_struct_to_string(tz);
    }

    /// \ingroup time_structures_time_formatting
    /// \brief Alias for time_zone_struct_to_string function.
    /// \copydoc time_zone_struct_to_string
    inline std::string to_str(const TimeZoneStruct& tz) {
        return time_zone_struct_to_string(tz);
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures_time_conversions
    /// \brief Convert a TimeZoneStruct to a numeric UTC offset (seconds).
    /// \param tz Time zone descriptor.
    /// \return UTC offset in seconds (local = utc + offset).
    TIME_SHIELD_CONSTEXPR inline tz_t time_zone_struct_to_offset(const TimeZoneStruct& tz) noexcept {
        return tz.is_positive
            ? static_cast<tz_t>( static_cast<tz_t>(tz.hour) * static_cast<tz_t>(SEC_PER_HOUR)
                               + static_cast<tz_t>(tz.min)  * static_cast<tz_t>(SEC_PER_MIN) )
            : static_cast<tz_t>(-( static_cast<tz_t>(tz.hour) * static_cast<tz_t>(SEC_PER_HOUR)
                                 + static_cast<tz_t>(tz.min)  * static_cast<tz_t>(SEC_PER_MIN) ));
    }

    /// \ingroup time_structures_time_conversions
    /// \brief Alias for time_zone_struct_to_offset.
    /// \copydoc time_zone_struct_to_offset
    TIME_SHIELD_CONSTEXPR inline tz_t tz_to_offset(const TimeZoneStruct& tz) noexcept {
        return time_zone_struct_to_offset(tz);
    }

    /// \ingroup time_structures_time_conversions
    /// \brief Alias for time_zone_struct_to_offset.
    /// \copydoc time_zone_struct_to_offset
    TIME_SHIELD_CONSTEXPR inline tz_t to_offset(const TimeZoneStruct& tz) noexcept {
        return time_zone_struct_to_offset(tz);
    }

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_ZONE_STRUCT_HPP_INCLUDED
