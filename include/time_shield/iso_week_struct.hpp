// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_ISO_WEEK_STRUCT_HPP_INCLUDED
#define _TIME_SHIELD_ISO_WEEK_STRUCT_HPP_INCLUDED

/// \file iso_week_struct.hpp
/// \brief Header for ISO week date structure.
///
/// This file defines the IsoWeekDateStruct structure used to represent ISO 8601 week dates.

#include <cstdint>

namespace time_shield {

    /// \ingroup time_structures
    /// \brief Structure to represent an ISO week date.
    struct IsoWeekDateStruct {
        int64_t year;    ///< ISO week-numbering year component.
        int32_t week;    ///< ISO week number component (1-52/53).
        int32_t weekday; ///< ISO weekday component (1=Monday .. 7=Sunday).
    };

    /// \ingroup time_structures
    /// \brief Creates an IsoWeekDateStruct instance.
    /// \param year ISO week-numbering year component.
    /// \param week ISO week number component.
    /// \param weekday ISO weekday component (1=Monday .. 7=Sunday).
    /// \return An IsoWeekDateStruct instance with the provided components.
    inline const IsoWeekDateStruct create_iso_week_date_struct(
            int64_t year,
            int32_t week = 1,
            int32_t weekday = 1) {
        IsoWeekDateStruct data{year, week, weekday};
        return data;
    }

}; // namespace time_shield

#endif // _TIME_SHIELD_ISO_WEEK_STRUCT_HPP_INCLUDED
