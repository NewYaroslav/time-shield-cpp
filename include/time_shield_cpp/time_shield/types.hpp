#pragma once
#ifndef _TIME_SHIELD_TYPES_HPP_INCLUDED
#define _TIME_SHIELD_TYPES_HPP_INCLUDED

/// \file types.hpp
/// \brief Header file with type definitions.
///
/// This file contains type definitions for various time-related units and formats.

#include <cstdint>

namespace time_shield {

/// \defgroup time_types Time Types
/// \brief Type definitions for time-related units and formats.
///
/// This group includes type definitions for years, timestamps (seconds, milliseconds, microseconds),
/// and other time-related units such as Julian dates and time zone offsets.
///
/// ### Key Features:
/// - Simplifies working with different time units and formats.
/// - Provides meaningful aliases for commonly used time-related data types.
///
/// ### Example Usage:
/// ```cpp
/// time_shield::ts_t timestamp = 1234567890; // Represent a Unix timestamp in seconds.
/// time_shield::year_t year = 2024; // Represent a year.
/// ```
///
/// \{

    typedef int64_t     year_t;      ///< Type for representing years as integers.
    typedef int64_t     uday_t;      ///< Type for representing Unix days as integers (days since January 1, 1970).
    typedef int64_t     ts_t;        ///< Type for representing timestamps in seconds.
    typedef int64_t     ts_ms_t;     ///< Type for representing timestamps in milliseconds.
    typedef int64_t     ts_us_t;     ///< Type for representing timestamps in microseconds.
    typedef double      fts_t;       ///< Type for representing timestamps as floating-point numbers (e.g., fractional seconds).
    typedef double      oadate_t;    ///< Type for representing Automation Date (OADate) values as floating-point numbers.
    typedef double      jd_t;        ///< Type for representing Julian Dates as floating-point numbers.
    typedef double      mjd_t;       ///< Type for representing Modified Julian Dates as floating-point numbers.
    typedef uint64_t    jdn_t;       ///< Type for representing Julian Day Numbers as unsigned integers.
    typedef int         tz_t;        ///< Type for representing time zone offsets in minutes.

    using unixday_t = uday_t; ///< Alias for Unix day type (uday_t).

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_TYPES_HPP_INCLUDED
