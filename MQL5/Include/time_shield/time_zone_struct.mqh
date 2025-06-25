//+------------------------------------------------------------------+
//|                                             time_zone_struct.mqh |
//|                           Time Shield - MQL5 Time Zone Structure |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_ZONE_STRUCT_MQH__
#define __TIME_SHIELD_TIME_ZONE_STRUCT_MQH__

/// \file time_zone_struct.mqh
/// \ingroup mql5
/// \brief Header for time zone structure and related functions (MQL5).
///
/// This file contains the definition of the `TimeZoneStruct` structure and
/// helper functions to create and convert time zone values in MQL5.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include "constants.mqh"

namespace time_shield {

    /// \ingroup mql5_time_structures
    /// \brief Structure to represent a time zone offset in MQL5.
    struct TimeZoneStruct {
       int  hour;        ///< Hour component of the offset (0-23)
       int  min;         ///< Minute component of the offset (0-59)
       bool is_positive; ///< True if the offset is positive, false if negative
    };

    /// \fn TimeZoneStruct create_time_zone_struct(int  hour, int  min, bool is_positive = true)
    /// \ingroup mql5_time_structures
    /// \brief Creates a `TimeZoneStruct` instance.
    /// \param hour The hour component of the offset.
    /// \param min The minute component of the offset.
    /// \param is_positive True if the offset is positive, false if negative.
    /// \return A `TimeZoneStruct` instance with the provided components.
    TimeZoneStruct create_time_zone_struct(
            int  hour,
            int  min,
            bool is_positive = true) {
       TimeZoneStruct result;
       result.hour        = hour;
       result.min         = min;
       result.is_positive = is_positive;
       return result;
    }

    //----------------------------------------------------------------------

    /// \fn TimeZoneStruct to_time_zone_struct(int offset)
    /// \ingroup mql5_time_structures_time_conversions
    /// \brief Converts an integer offset to a `TimeZoneStruct`.
    /// \param offset The integer offset in seconds to convert.
    /// \return A `TimeZoneStruct` represented by the given offset.
    TimeZoneStruct to_time_zone_struct(int offset) {
       int abs_val      = (int)MathAbs(offset);
       int hour         = abs_val / (int)SEC_PER_HOUR;
       int min          = abs_val % (int)SEC_PER_MIN;
       bool is_positive = (offset >= 0);
       return create_time_zone_struct(hour, min, is_positive);
    }

    /// \copybrief to_time_zone_struct(int)
    /// \see to_time_zone_struct
    TimeZoneStruct to_tz(int offset) {
       return to_time_zone_struct(offset);
    }

    //----------------------------------------------------------------------

    /// \fn string time_zone_struct_to_string(const TimeZoneStruct &tz)
    /// \ingroup mql5_time_structures_time_formatting
    /// \brief Converts a `TimeZoneStruct` to a string representation.
    /// \param tz The `TimeZoneStruct` to convert.
    /// \return A string representation like "+03:00" or "-05:30".
    string time_zone_struct_to_string(const TimeZoneStruct &tz) {
       string result = tz.is_positive ? "+" : "-";
       if (tz.hour < 10)
          result += "0";
       result += IntegerToString(tz.hour);
       result += ":";
       if (tz.min < 10)
          result += "0";
       result += IntegerToString(tz.min);
       return result;
    }

    /// \fn string time_zone_struct_to_string(const TimeZoneStruct &tz)
    /// \ingroup mql5_time_structures_time_conversions
    /// \brief Alias for \ref time_zone_struct_to_string.
    /// \details This function is a simple wrapper around \ref time_zone_struct_to_string.
    /// \copydoc time_zone_struct_to_string
    string to_string(const TimeZoneStruct &tz) {
       return time_zone_struct_to_string(tz);
    }

    /// \fn string time_zone_struct_to_string(const TimeZoneStruct &tz)
    /// \ingroup mql5_time_structures_time_conversions
    /// \brief Alias for \ref time_zone_struct_to_string.
    /// \details This function is a simple wrapper around \ref time_zone_struct_to_string.
    /// \copydoc time_zone_struct_to_string
    string to_str(const TimeZoneStruct &tz) {
       return time_zone_struct_to_string(tz);
    }

    //----------------------------------------------------------------------

    /// \fn string int time_zone_struct_to_offset(const TimeZoneStruct &tz)
    /// \ingroup mql5_time_structures_time_conversions
    /// \brief Converts a `TimeZoneStruct` to a single integer offset.
    /// \param tz The `TimeZoneStruct` to convert.
    /// \return An integer representing the offset in seconds.
    int time_zone_struct_to_offset(const TimeZoneStruct &tz) {
       int sign = tz.is_positive ? 1 : -1;
       return sign * (tz.hour * (int)SEC_PER_HOUR + tz.min * (int)SEC_PER_MIN);
    }

    /// \fn int tz_to_offset(const TimeZoneStruct &tz)
    /// \ingroup mql5_time_structures_time_conversions
    /// \brief Alias for \ref time_zone_struct_to_offset.
    /// \details This function is a simple wrapper around \ref time_zone_struct_to_offset.
    /// \copydoc time_zone_struct_to_offset
    int tz_to_offset(const TimeZoneStruct &tz) {
       return time_zone_struct_to_offset(tz);
    }

    /// \fn int tz_to_offset(const TimeZoneStruct &tz)
    /// \ingroup mql5_time_structures_time_conversions
    /// \brief Alias for \ref time_zone_struct_to_offset.
    /// \details This function is a simple wrapper around \ref time_zone_struct_to_offset.
    /// \copydoc time_zone_struct_to_offset
    int to_offset(const TimeZoneStruct &tz) {
       return time_zone_struct_to_offset(tz);
    }

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_ZONE_STRUCT_MQH__
