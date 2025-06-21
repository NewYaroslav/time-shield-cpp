//+------------------------------------------------------------------+
//|                                       date_time_struct.mqh       |
//|                      Time Shield - MQL5 DateTime Structure       |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_DATE_TIME_STRUCT_MQH__
#define __TIME_SHIELD_DATE_TIME_STRUCT_MQH__

/// \file date_time_struct.mqh
/// \ingroup mql5
/// \brief Header for date and time structure and related functions (MQL5).
///
/// This file contains the definition of the `DateTimeStruct` structure and
/// a function to create `DateTimeStruct` instances for working with
/// combined date and time values in MQL5.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

namespace time_shield {

    /// \ingroup time_structures
    /// \brief Structure to represent date and time in MQL5.
    struct DateTimeStruct {
       long year;   ///< Year component of the date.
       int  mon;    ///< Month component of the date (1-12).
       int  day;    ///< Day component of the date (1-31).
       int  hour;   ///< Hour component of time (0-23).
       int  min;    ///< Minute component of time (0-59).
       int  sec;    ///< Second component of time (0-59).
       int  ms;     ///< Millisecond component of time (0-999).
    };

    /// \ingroup time_structures
    /// \brief Creates a `DateTimeStruct` instance.
    /// \param year The year component of the date.
    /// \param mon The month component of the date, defaults to 1 (January).
    /// \param day The day component of the date, defaults to 1.
    /// \param hour The hour component of the time, defaults to 0.
    /// \param min The minute component of the time, defaults to 0.
    /// \param sec The second component of the time, defaults to 0.
    /// \param ms The millisecond component of the time, defaults to 0.
    /// \return A `DateTimeStruct` instance with the provided date and time components.
    DateTimeStruct create_date_time_struct(
            const long year,
            const int mon = 1,
            const int day = 1,
            const int hour = 0,
            const int min = 0,
            const int sec = 0,
            const int ms  = 0) {
       DateTimeStruct result;
       result.year = year;
       result.mon  = mon;
       result.day  = day;
       result.hour = hour;
       result.min  = min;
       result.sec  = sec;
       result.ms   = ms;
       return result;
    }

}; // namespace time_shield

#endif // __TIME_SHIELD_DATE_TIME_STRUCT_MQH__
