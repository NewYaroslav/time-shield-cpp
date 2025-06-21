//+------------------------------------------------------------------+
//|                                                  date_struct.mqh |
//|                                Time Shield - MQL5 Date Structure |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_DATE_STRUCT_MQH__
#define __TIME_SHIELD_DATE_STRUCT_MQH__

/// \file date_struct.mqh
/// \ingroup mql5
/// \brief Header for date structure and related functions (MQL5).
///
/// This file contains the definition of the `DateStruct` structure and a function
/// to create `DateStruct` instances for working with calendar dates in MQL5.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

namespace time_shield {

    /// \ingroup time_structures
    /// \brief Structure to represent a date in MQL5.
    struct DateStruct {
       long year;   ///< Year component of the date.
       int  mon;    ///< Month component of the date (1–12).
       int  day;    ///< Day component of the date (1–31).
    };

    /// \ingroup time_structures
    /// \brief Creates a `DateStruct` instance.
    /// \param year The year component of the date.
    /// \param mon The month component of the date, defaults to 1 (January).
    /// \param day The day component of the date, defaults to 1.
    /// \return A `DateStruct` instance with the provided date components.
    DateStruct create_date_struct(const long year, const int mon = 1, const int day = 1) {
       DateStruct result;
       result.year = year;
       result.mon  = mon;
       result.day  = day;
       return result;
    }

}; // namespace time_shield

#endif // __TIME_SHIELD_DATE_STRUCT_MQH__
