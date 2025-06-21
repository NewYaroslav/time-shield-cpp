//+------------------------------------------------------------------+
//|                                            constants.mqh         |
//|                     Time Shield - MQL5 Constants                 |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_CONSTANTS_MQH__
#define __TIME_SHIELD_CONSTANTS_MQH__

/// \file constants.mqh
/// \ingroup mql5
/// \brief Header file with time-related constants.
///
/// This file contains various constants used for time calculations and conversions.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

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
    /// \code
    /// long milliseconds_in_a_day = time_shield::MS_PER_DAY;
    /// \endcode
    ///
    /// \{

    // Nanoseconds and microseconds
    const long NS_PER_US     = 1000;         ///< Nanoseconds per microsecond
    const long NS_PER_MS     = 1000000;      ///< Nanoseconds per millisecond
    const long NS_PER_SEC    = 1000000000;   ///< Nanoseconds per second

    // Microseconds and milliseconds
    const long US_PER_SEC        = 1000000;  ///< Microseconds per second
    const long MS_PER_SEC        = 1000;     ///< Milliseconds per second
    const long MS_PER_1_SEC      = 1000;     ///< Milliseconds per 1 second
    const long MS_PER_5_SEC      = 5000;     ///< Milliseconds per 5 second
    const long MS_PER_10_SEC     = 10000;    ///< Milliseconds per 10 seconds
    const long MS_PER_15_SEC     = 15000;    ///< Milliseconds per 15 second
    const long MS_PER_30_SEC     = 30000;    ///< Milliseconds per 30 second
    const long MS_PER_MIN        = 60000;    ///< Milliseconds per minute
    const long MS_PER_1_MIN      = 60000;    ///< Milliseconds per 1 minute
    const long MS_PER_5_MIN      = 300000;   ///< Milliseconds per 5 minute
    const long MS_PER_10_MIN     = 600000;   ///< Milliseconds per 10 minute
    const long MS_PER_15_MIN     = 900000;   ///< Milliseconds per 15 minute
    const long MS_PER_30_MIN     = 1800000;  ///< Milliseconds per 30 minute
    const long MS_PER_HALF_HOUR  = 1800000;  ///< Milliseconds per half hour
    const long MS_PER_HOUR       = 3600000;  ///< Milliseconds per hour
    const long MS_PER_1_HOUR     = 3600000;  ///< Milliseconds per 1 hour
    const long MS_PER_2_HOUR     = 7200000;  ///< Milliseconds per 2 hour
    const long MS_PER_4_HOUR     = 14400000; ///< Milliseconds per 4 hour
    const long MS_PER_5_HOUR     = 18000000; ///< Milliseconds per 5 hour
    const long MS_PER_8_HOUR     = 28800000; ///< Milliseconds per 8 hour
    const long MS_PER_12_HOUR    = 43200000; ///< Milliseconds per 12 hour
    const long MS_PER_DAY        = 86400000; ///< Milliseconds per day

    // Seconds
    const long SEC_PER_MIN       = 60;       ///< Seconds per minute
    const long SEC_PER_1_MIN     = 60;       ///< Seconds per 1 minute
    const long SEC_PER_3_MIN     = 180;      ///< Seconds per 3 minute
    const long SEC_PER_5_MIN     = 300;      ///< Seconds per 5 minute
    const long SEC_PER_10_MIN    = 600;      ///< Seconds per 10 minute
    const long SEC_PER_15_MIN    = 900;      ///< Seconds per 15 minute
    const long SEC_PER_HALF_HOUR = 1800;     ///< Seconds per half hour
    const long SEC_PER_HOUR      = 3600;     ///< Seconds per hour
    const long SEC_PER_1_HOUR    = 3600;     ///< Seconds per 1 hour
    const long SEC_PER_2_HOUR    = 7200;     ///< Seconds per 2 hour
    const long SEC_PER_4_HOUR    = 14400;    ///< Seconds per 4 hour
    const long SEC_PER_5_HOUR    = 18000;    ///< Seconds per 5 hour
    const long SEC_PER_8_HOUR    = 28800;    ///< Seconds per 8 hour
    const long SEC_PER_12_HOUR   = 43200;    ///< Seconds per 12 hour
    const long SEC_PER_DAY       = 86400;    ///< Seconds per day
    const long SEC_PER_YEAR      = 31536000; ///< Seconds per year (365 days)
    const long AVG_SEC_PER_YEAR  = 31557600; ///< Average seconds per year (365.25 days)
    const long SEC_PER_LEAP_YEAR = 31622400; ///< Seconds per leap year (366 days)
    const long SEC_PER_4_YEARS   = 126230400; ///< Seconds per 4 years
    const long SEC_PER_FIRST_100_YEARS = 3155760000; ///< Seconds per first 100 years
    const long SEC_PER_100_YEARS = 3155673600;   ///< Seconds per 100 years
    const long SEC_PER_400_YEARS = 12622780800;  ///< Seconds per 400 years
    const long MAX_SEC_PER_DAY   = 86399;    ///< Maximum seconds per day

    // Minutes
    const long MIN_PER_HOUR      = 60;       ///< Minutes per hour
    const long MIN_PER_DAY       = 1440;     ///< Minutes per day
    const long MIN_PER_1_DAY     = 1440;     ///< Minutes per 1 day
    const long MIN_PER_2_DAY     = 2*1440;   ///< Minutes per 2 day
    const long MIN_PER_5_DAY     = 5*1440;   ///< Minutes per 5 day
    const long MIN_PER_7_DAY     = 7*1440;   ///< Minutes per 7 day
    const long MIN_PER_WEEK      = 10080;    ///< Minutes per week
    const long MIN_PER_10_DAY    = 10*1440;  ///< Minutes per 10 day
    const long MIN_PER_15_DAY    = 15*1440;  ///< Minutes per 15 day
    const long MIN_PER_30_DAY    = 15*1440;  ///< Minutes per 30 day
    const long MIN_PER_MONTH     = 40320;    ///< Minutes per month (28 days)
    const long MAX_MOON_MIN      = 42523;    ///< Maximum lunar minutes

    // Hours and days
    const long HOURS_PER_DAY     = 24;       ///< Hours per day
    const long DAYS_PER_WEEK     = 7;        ///< Days per week
    const long DAYS_PER_LEAP_YEAR = 366;     ///< Days per leap year
    const long DAYS_PER_YEAR     = 365;      ///< Days per year
    const long DAYS_PER_4_YEARS  = 1461;     ///< Days per 4 years

    // Months and years
    const long MONTHS_PER_YEAR       = 12;   ///< Months per year
    const long MAX_DAYS_PER_MONTH    = 31;   ///< Maximum days per month
    const long LEAP_YEAR_PER_100_YEAR = 24;  ///< Leap years per 100 years
    const long LEAP_YEAR_PER_400_YEAR = 97;  ///< Leap years per 400 years

    // Epoch and maximum values
    const long UNIX_EPOCH        = 1970;     ///< Start year of UNIX time
    const long OLE_EPOCH         = 25569;    ///< OLE automation date since UNIX epoch
    const long MAX_YEAR          = 292277022000;   ///< Maximum representable year
    const long MIN_YEAR          = -2967369602200; ///< Minimum representable year
    const long ERROR_YEAR        = 9223372036854770000; ///< Error year value
    const long MAX_TIMESTAMP     = 9223371890843040000; ///< Maximum timestamp value
    const long ERROR_TIMESTAMP   = 9223372036854770000; ///< Error timestamp value
    const double MAX_OADATE      = 1.7976931348623158e+308; ///< Maximum OLE automation date
    const double AVG_DAYS_PER_YEAR  = 365.25;   ///< Average days per year

    /// \}

}; // namespace time_shield

#endif // __TIME_SHIELD_CONSTANTS_MQH__
