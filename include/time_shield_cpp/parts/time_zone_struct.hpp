#pragma once
/// \file time_zone_struct.hpp
/// \brief Header for time zone structure and related functions.
#ifndef _TIME_SHIELD_TIME_ZONE_STRUCT_HPP_INCLUDED
#define _TIME_SHIELD_TIME_ZONE_STRUCT_HPP_INCLUDED

#include "types.hpp"
#include "constants.hpp"

namespace time_shield {

    /// \brief Structure to represent time zone information.
    /// \details
    /// This structure contains the hour and minute components of a time zone offset,
    /// as well as a boolean indicating whether the offset is positive or negative.
    struct TimeZoneStruct {
        int hour;           ///< Hour component of time (0-23)
        int min;            ///< Minute component of time (0-59)
        bool is_positive;   ///< True if the time zone offset is positive, false if negative
    };

    /// \brief Creates a TimeZoneStruct instance.
    /// \param hour The hour component of the time.
    /// \param min The minute component of the time.
    /// \param is_positive True if the time zone offset is positive, false if negative.
    /// \return A TimeZoneStruct instance with the provided time components.
    inline const TimeZoneStruct create_time_zone_struct(
            const int& hour,
            const int& min,
            const bool& is_positive = true) {
        return TimeZoneStruct{hour, min, is_positive};
    }

//------------------------------------------------------------------------------

    /// \brief Converts an integer to a TimeZoneStruct.
    /// \param offset The integer to convert.
    /// \return A TimeZoneStruct represented by the given integer.
    inline const TimeZoneStruct to_time_zone_struct(
            const tz_t& offset) {
        int abs_val = std::abs(offset);
        int hour = abs_val / SEC_PER_HOUR;
        int min = abs_val % SEC_PER_MIN;
        bool is_positive = (offset >= 0);
        return TimeZoneStruct{hour, min, is_positive};
    }

    /// \brief Alias for to_time_zone_struct function.
    /// \copydoc to_time_zone_struct
    inline const TimeZoneStruct to_tz(const tz_t& offset) {
        return to_time_zone_struct(offset);
    }

//------------------------------------------------------------------------------

    /// \brief Converts a TimeZoneStruct to a string representation.
    /// \param tz The TimeZoneStruct to convert.
    /// \return A string representation of the TimeZoneStruct.
    inline const std::string time_zone_struct_to_string(const TimeZoneStruct& tz) {
        char sign = tz.is_positive ? '+' : '-';
        return std::string(1, sign) + (tz.hour < 10 ? "0" : "") + std::to_string(tz.hour) + ":" + (tz.min < 10 ? "0" : "") + std::to_string(tz.min);
    }

    /// \brief Alias for time_zone_struct_to_string function.
    /// \copydoc time_zone_struct_to_string
    inline const std::string to_string(const TimeZoneStruct& tz) {
        return time_zone_struct_to_string(tz);
    }

    /// \brief Alias for time_zone_struct_to_string function.
    /// \copydoc time_zone_struct_to_string
    inline const std::string to_str(const TimeZoneStruct& tz) {
        return time_zone_struct_to_string(tz);
    }

//------------------------------------------------------------------------------

    /// \brief Converts a TimeZoneStruct to a single integer representation.
    /// \param tz The TimeZoneStruct to convert.
    /// \return An integer representing the TimeZoneStruct.
    inline const tz_t time_zone_struct_to_offset(const TimeZoneStruct& tz) {
        return (tz.is_positive ? 1 : -1) * (tz.hour * SEC_PER_HOUR + tz.min * SEC_PER_MIN);
    }

    /// \brief Alias for time_zone_struct_to_offset function.
    /// \copydoc time_zone_struct_to_offset
    inline const tz_t tz_to_offset(const TimeZoneStruct& tz) {
        return time_zone_struct_to_offset(tz);
    }

    /// \brief Alias for time_zone_struct_to_offset function.
    /// \copydoc time_zone_struct_to_offset
    inline const tz_t to_offset(const TimeZoneStruct& tz) {
        return time_zone_struct_to_offset(tz);
    }

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_STRUCT_HPP_INCLUDED
