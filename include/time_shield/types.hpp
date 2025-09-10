#pragma once
#ifndef _TIME_SHIELD_TYPES_HPP_INCLUDED
#define _TIME_SHIELD_TYPES_HPP_INCLUDED

/// \file types.hpp
/// \brief Type definitions for time-related units and formats.
///
/// This file defines standard type aliases used across the Time Shield library.
/// It includes representations for Unix timestamps, Julian dates, automation time,
/// time zone offsets, and other related units.

#include <cstdint>

namespace time_shield {

/// \defgroup time_types Time Types
/// \brief Fundamental type definitions for time-related data.
/// \ingroup cpp
///
/// This group defines the core time representations used throughout the library,
/// including timestamps, fractional time units, Julian dates, and time zone offsets.
///
/// ### Type Categories
/// - **Unix-based timestamps**: `ts_t`, `ts_ms_t`, `ts_us_t`
/// - **Fractional and floating-point time**: `fts_t`, `oadate_t`, `jd_t`
/// - **Julian date types**: `jd_t`, `mjd_t`, `jdn_t`
/// - **Utility units**: `year_t`, `uday_t`, `tz_t`
///
/// ### Example Usage
/// ```cpp
/// time_shield::ts_t now = 1700000000;      // Unix timestamp in seconds
/// time_shield::fts_t precise = 1700000000.123; // Time with fractional seconds
/// time_shield::jd_t julian = 2459580.5;     // Julian Date
/// time_shield::tz_t offset = 180;           // UTC+3 in minutes
/// ```

/// \{

    // --- Calendar & Year Types ---
    typedef int64_t     year_t;     ///< Year as an integer (e.g., 2024).
    typedef int64_t     uday_t;     ///< Unix day count since 1970‑01‑01 (days since epoch).
    using unixday_t = uday_t;       ///< Alias for Unix day count type.

    // --- Unix Timestamp Types ---
    typedef int64_t     ts_t;       ///< Unix timestamp in seconds since 1970‑01‑01T00:00:00Z.
    typedef int64_t     ts_ms_t;    ///< Unix timestamp in milliseconds since epoch.
    typedef int64_t     ts_us_t;    ///< Unix timestamp in microseconds since epoch.
    typedef double      fts_t;      ///< Floating-point timestamp (fractional seconds since epoch).

    // --- Automation and Julian Time ---
    typedef double      oadate_t;   ///< OLE Automation date (days since 1899‑12‑30, as `double`).
    typedef double      jd_t;       ///< Julian Date (days since -4713‑11‑24T12:00:00Z).
    typedef double      mjd_t;      ///< Modified Julian Date (JD − 2400000.5).
    typedef uint64_t    jdn_t;      ///< Julian Day Number (whole days since Julian epoch).

    // --- Time zone offset ---
    typedef int32_t     tz_t;       ///< Time zone offset in minutes from UTC (e.g., +180 = UTC+3).

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_TYPES_HPP_INCLUDED
