//+------------------------------------------------------------------+
//|                                                  time_struct.mqh |
//|                                Time Shield - MQL5 Time Structure |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_STRUCT_MQH__
#define __TIME_SHIELD_TIME_STRUCT_MQH__

/// \file time_struct.mqh
/// \ingroup mql5
/// \brief Header for time structure and related functions (MQL5).
///
/// This file contains the definition of the `TimeStruct` structure and a
/// function to create `TimeStruct` instances for working with time values
/// in MQL5.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

namespace time_shield {

    /// \ingroup time_structures
    /// \brief Structure to represent time in MQL5.
    struct TimeStruct {
       int hour;   ///< Hour component of time (0-23).
       int min;    ///< Minute component of time (0-59).
       int sec;    ///< Second component of time (0-59).
       int ms;     ///< Millisecond component of time (0-999).
    };

    /// \ingroup time_structures
    /// \brief Creates a `TimeStruct` instance.
    /// \param hour The hour component of the time.
    /// \param min The minute component of the time.
    /// \param sec The second component of the time, defaults to 0.
    /// \param ms The millisecond component of the time, defaults to 0.
    /// \return A `TimeStruct` instance with the provided time components.
    TimeStruct create_time_struct(
            const int hour,
            const int min,
            const int sec = 0,
            const int ms  = 0) {
       TimeStruct result;
       result.hour = hour;
       result.min  = min;
       result.sec  = sec;
       result.ms   = ms;
       return result;
    }

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_STRUCT_MQH__
