#pragma once
/// \file constants.hpp
/// \brief Header file with time-related constants.
///
/// This file contains various constants used for time calculations and conversions.
#ifndef _TIME_SHIELD_CONSTANTS_HPP_INCLUDED
#define _TIME_SHIELD_CONSTANTS_HPP_INCLUDED

#include <cstdint>
#include <limits>

namespace time_shield {

/// \defgroup time_constants Time Constants
/// \brief A collection of constants for time calculations and conversions.
///
/// This group includes constants for time units (nanoseconds, microseconds, milliseconds, seconds, minutes, hours, days),
/// and other values related to the representation of time, such as UNIX and OLE epochs.
///
/// ### Key Features:
/// - Provides constants for common time conversions.
/// - Includes limits and special values like MAX_YEAR and ERROR_YEAR.
///
/// ### Example Usage:
/// ```cpp
/// int64_t milliseconds_in_a_day = time_shield::MS_PER_DAY;
/// ```
///
/// \{

    // Nanoseconds and microseconds
    constexpr int64_t NS_PER_US     = 1000;         ///< Nanoseconds per microsecond
    constexpr int64_t NS_PER_MS     = 1000000;      ///< Nanoseconds per millisecond
    constexpr int64_t NS_PER_SEC    = 1000000000;   ///< Nanoseconds per second

    // Microseconds and milliseconds
    constexpr int64_t US_PER_SEC        = 1000000;  ///< Microseconds per second
    constexpr int64_t MS_PER_SEC        = 1000;     ///< Milliseconds per second
    constexpr int64_t MS_PER_5_SEC      = 5000;     ///< Milliseconds per 5 second
    constexpr int64_t MS_PER_10_SEC     = 5000;     ///< Milliseconds per 10 second
    constexpr int64_t MS_PER_15_SEC     = 15000;    ///< Milliseconds per 15 second
    constexpr int64_t MS_PER_30_SEC     = 30000;    ///< Milliseconds per 30 second
    constexpr int64_t MS_PER_MIN        = 60000;    ///< Milliseconds per minute
    constexpr int64_t MS_PER_5_MIN      = 300000;   ///< Milliseconds per 5 minute
    constexpr int64_t MS_PER_10_MIN     = 600000;   ///< Milliseconds per 10 minute
    constexpr int64_t MS_PER_15_MIN     = 900000;   ///< Milliseconds per 15 minute
    constexpr int64_t MS_PER_30_MIN     = 1800000;  ///< Milliseconds per 30 minute
    constexpr int64_t MS_PER_HALF_HOUR  = 1800000;  ///< Milliseconds per half hour
    constexpr int64_t MS_PER_HOUR       = 3600000;  ///< Milliseconds per hour
    constexpr int64_t MS_PER_DAY        = 86400000; ///< Milliseconds per day

    // Seconds
    constexpr int64_t SEC_PER_MIN       = 60;       ///< Seconds per minute
    constexpr int64_t SEC_PER_3_MIN     = 180;      ///< Seconds per 3 minute
    constexpr int64_t SEC_PER_5_MIN     = 300;      ///< Seconds per 5 minute
    constexpr int64_t SEC_PER_10_MIN    = 600;      ///< Seconds per 10 minute
    constexpr int64_t SEC_PER_15_MIN    = 900;      ///< Seconds per 15 minute
    constexpr int64_t SEC_PER_HALF_HOUR = 1800;     ///< Seconds per half hour
    constexpr int64_t SEC_PER_HOUR      = 3600;     ///< Seconds per hour
    constexpr int64_t SEC_PER_DAY       = 86400;    ///< Seconds per day
    constexpr int64_t SEC_PER_YEAR      = 31536000; ///< Seconds per year (365 days)
    constexpr int64_t AVG_SEC_PER_YEAR  = 31557600; ///< Average seconds per year (365.25 days)
    constexpr int64_t SEC_PER_LEAP_YEAR = 31622400; ///< Seconds per leap year (366 days)
    constexpr int64_t SEC_PER_4_YEARS   = 126230400;///< Seconds per 4 years
    constexpr int64_t SEC_PER_FIRST_100_YEARS = 3155760000; ///< Seconds per first 100 years
    constexpr int64_t SEC_PER_100_YEARS = 3155673600;   ///< Seconds per 100 years
    constexpr int64_t SEC_PER_400_YEARS = 12622780800;  ///< Seconds per 400 years
    constexpr int64_t MAX_SEC_PER_DAY   = 86399;    ///< Maximum seconds per day

    // Minutes
    constexpr int64_t MIN_PER_HOUR      = 60;       ///< Minutes per hour
    constexpr int64_t MIN_PER_DAY       = 1440;     ///< Minutes per day
    constexpr int64_t MIN_PER_2_DAY     = 2*1440;   ///< Minutes per 2 day
    constexpr int64_t MIN_PER_5_DAY     = 5*1440;   ///< Minutes per 5 day
    constexpr int64_t MIN_PER_7_DAY     = 7*1440;   ///< Minutes per 7 day
    constexpr int64_t MIN_PER_WEEK      = 10080;    ///< Minutes per week
    constexpr int64_t MIN_PER_10_DAY    = 10*1440;  ///< Minutes per 10 day
    constexpr int64_t MIN_PER_15_DAY    = 15*1440;  ///< Minutes per 15 day
    constexpr int64_t MIN_PER_30_DAY    = 15*1440;  ///< Minutes per 30 day
    constexpr int64_t MIN_PER_MONTH     = 40320;    ///< Minutes per month (28 days)
    constexpr int64_t MAX_MOON_MIN      = 42523;    ///< Maximum lunar minutes

    // Hours and days
    constexpr int64_t HOURS_PER_DAY     = 24;       ///< Hours per day
    constexpr int64_t DAYS_PER_WEEK     = 7;        ///< Days per week
    constexpr int64_t DAYS_PER_LEAP_YEAR = 366;     ///< Days per leap year
    constexpr int64_t DAYS_PER_YEAR     = 365;      ///< Days per year
    constexpr int64_t DAYS_PER_4_YEARS  = 1461;     ///< Days per 4 years

    // Months and years
    const int64_t MONTHS_PER_YEAR       = 12;       ///< Months per year
    const int64_t MAX_DAYS_PER_MONTH    = 31;       ///< Maximum days per month
    const int64_t LEAP_YEAR_PER_100_YEAR = 24;      ///< Leap years per 100 years
    const int64_t LEAP_YEAR_PER_400_YEAR = 97;      ///< Leap years per 400 years

    // Epoch and maximum values
    constexpr int64_t UNIX_EPOCH        = 1970;     ///< Start year of UNIX time
    constexpr int64_t OLE_EPOCH         = 25569;    ///< OLE automation date since UNIX epoch
    constexpr int64_t MAX_YEAR          = 292277022000LL;   ///< Maximum representable year
    constexpr int64_t MIN_YEAR          = -2967369602200LL; ///< Minimum representable year
    constexpr int64_t ERROR_YEAR        = 9223372036854770000LL; ///< Error year value
    constexpr int64_t MAX_TIMESTAMP     = 9223371890843040000LL; ///< Maximum timestamp value
    constexpr int64_t ERROR_TIMESTAMP   = 9223372036854770000LL; ///< Error timestamp value
    constexpr double MAX_OADATE         = std::numeric_limits<double>::max();   ///< Maximum OLE automation date
    constexpr double AVG_DAYS_PER_YEAR  = 365.25;   ///< Average days per year

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_CONSTANTS_HPP_INCLUDED
