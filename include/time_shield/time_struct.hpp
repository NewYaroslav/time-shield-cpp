#pragma once
#ifndef _TIME_SHIELD_TIME_STRUCT_HPP_INCLUDED
#define _TIME_SHIELD_TIME_STRUCT_HPP_INCLUDED

/// \file time_struct.hpp
/// \brief Header for time structure and related functions.
///
/// This file contains the definition of the TimeStruct structure and a function to create TimeStruct instances.

namespace time_shield {

    /// \ingroup time_structures
    /// \brief Structure to represent time.
    struct TimeStruct {
        int16_t hour;   ///< Hour component of time (0-23)
        int16_t min;    ///< Minute component of time (0-59)
        int16_t sec;    ///< Second component of time (0-59)
        int16_t ms;     ///< Millisecond component of time (0-999)
    };

    /// \ingroup time_structures
    /// \brief Creates a TimeStruct instance.
    /// \param hour The hour component of the time.
    /// \param min The minute component of the time.
    /// \param sec The second component of the time, defaults to 0.
    /// \param ms The millisecond component of the time, defaults to 0.
    /// \return A TimeStruct instance with the provided time components.
    inline const TimeStruct create_time_struct(
            int16_t hour,
            int16_t min,
            int16_t sec = 0,
            int16_t ms = 0) {
        TimeStruct data{hour, min, sec, ms};
        return data;
    }

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_STRUCT_HPP_INCLUDED
