#pragma once
#ifndef _TIME_SHIELD_DATE_TIME_STRUCT_HPP_INCLUDED
#define _TIME_SHIELD_DATE_TIME_STRUCT_HPP_INCLUDED

/// \file date_time_struct.hpp
/// \brief Header for date and time structure and related functions.
///
/// This file contains the definition of the DateTimeStruct structure and a function to create DateTimeStruct instances.

#include <cstdint>

namespace time_shield {

    /// \ingroup time_structures
    /// \brief Structure to represent date and time.
    struct DateTimeStruct {
        int64_t year;   ///< Year component of the date.
        int     mon;    ///< Month component of the date (1-12).
        int     day;    ///< Day component of the date (1-31).
        int     hour;   ///< Hour component of time (0-23)
        int     min;    ///< Minute component of time (0-59)
        int     sec;    ///< Second component of time (0-59)
        int     ms;     ///< Millisecond component of time (0-999)
    };

    /// \ingroup time_structures
    /// \brief Creates a DateTimeStruct instance.
    /// \param year The year component of the date.
    /// \param mon The month component of the date, defaults to 1 (January).
    /// \param day The day component of the date, defaults to 1.
    /// \param hour The hour component of the time, defaults to 0.
    /// \param min The minute component of the time, defaults to 0.
    /// \param sec The second component of the time, defaults to 0.
    /// \param ms The millisecond component of the time, defaults to 0.
    /// \return A DateTimeStruct instance with the provided date and time components.
    inline const DateTimeStruct create_date_time_struct(
            int64_t year,
            int mon = 1,
            int day = 1,
            int hour = 0,
            int min = 0,
            int sec = 0,
            int ms  = 0) {
        DateTimeStruct date_time;
        date_time.year  = year;
        date_time.mon   = mon;
        date_time.day   = day;
        date_time.hour  = hour;
        date_time.min   = min;
        date_time.sec   = sec;
        date_time.ms    = ms;
        return date_time;
    }

}; // namespace time_shield

#endif // _TIME_SHIELD_DATE_TIME_STRUCT_HPP_INCLUDED
