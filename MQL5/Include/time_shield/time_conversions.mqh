//+------------------------------------------------------------------+
//|                                            time_conversions.mqh  |
//|                      Time Shield - MQL5 Time Conversions         |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_CONVERSIONS_MQH__
#define __TIME_SHIELD_TIME_CONVERSIONS_MQH__

/// \file time_conversions.mqh
/// \ingroup mql5
/// \brief Header with helper functions for converting between
/// different time representations in MQL5.
///
/// This file contains utility functions to convert timestamps
/// between seconds, milliseconds and minutes, as well as helpers
/// for obtaining sub-second parts and working with date structures.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include <time_shield/constants.mqh>
#include <time_shield/date_time_struct.mqh>

namespace time_shield {

    /// \defgroup time_conversions Time Conversions
    /// \brief Utility functions for converting between time units.
    /// \{

    /// \brief Get the nanosecond part of a floating-point timestamp.
    /// \param ts Timestamp in seconds with fractional part.
    /// \return Nanoseconds of the second.
    int ns_of_sec(double ts) {
       double frac = ts - MathFloor(ts);
       return (int)MathRound(frac * NS_PER_SEC);
    }

    /// \brief Get the microsecond part of a floating-point timestamp.
    /// \param ts Timestamp in seconds with fractional part.
    /// \return Microseconds of the second.
    int us_of_sec(double ts) {
       double frac = ts - MathFloor(ts);
       return (int)MathRound(frac * US_PER_SEC);
    }

    /// \brief Get the millisecond part of a floating-point timestamp.
    /// \param ts Timestamp in seconds with fractional part.
    /// \return Milliseconds of the second.
    int ms_of_sec(double ts) {
       double frac = ts - MathFloor(ts);
       return (int)MathRound(frac * MS_PER_SEC);
    }

    /// \brief Get the millisecond part of a millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Milliseconds component.
    int ms_of_ts(long ts_ms) {
       return (int)(ts_ms % MS_PER_SEC);
    }

    //----------------------------------------------------------------------
    // Seconds <-> Milliseconds
    //----------------------------------------------------------------------

    /// \brief Convert seconds to milliseconds.
    /// \param sec Timestamp in seconds.
    /// \return Milliseconds value.
    long sec_to_ms(double sec) {
       return (long)MathRound(sec * MS_PER_SEC);
    }

    /// \brief Convert milliseconds to seconds (integer part).
    /// \param ms Timestamp in milliseconds.
    /// \return Seconds value.
    long ms_to_sec(long ms) {
       return ms / MS_PER_SEC;
    }

    /// \brief Convert milliseconds to floating-point seconds.
    /// \param ms Timestamp in milliseconds.
    /// \return Seconds as double.
    double ms_to_fsec(long ms) {
       return (double)ms / (double)MS_PER_SEC;
    }

    //----------------------------------------------------------------------
    // Minutes <-> Milliseconds / Seconds
    //----------------------------------------------------------------------

    /// \brief Convert minutes to milliseconds.
    /// \param min Minutes value.
    /// \return Milliseconds value.
    long min_to_ms(double min) {
       return (long)MathRound(min * MS_PER_MIN);
    }

    /// \brief Convert milliseconds to minutes (integer part).
    /// \param ms Milliseconds value.
    /// \return Minutes value.
    int ms_to_min(long ms) {
       return (int)(ms / MS_PER_MIN);
    }

    /// \brief Convert minutes to seconds.
    /// \param min Minutes value.
    /// \return Seconds value.
    long min_to_sec(double min) {
       return (long)MathRound(min * SEC_PER_MIN);
    }

    /// \brief Convert seconds to minutes (integer part).
    /// \param sec Seconds value.
    /// \return Minutes value.
    int sec_to_min(long sec) {
       return (int)(sec / SEC_PER_MIN);
    }

    /// \brief Convert minutes to floating-point seconds.
    /// \param min Minutes value.
    /// \return Seconds as double.
    double min_to_fsec(double min) {
       return min * SEC_PER_MIN;
    }

    /// \brief Convert seconds to floating-point minutes.
    /// \param sec Seconds value.
    /// \return Minutes as double.
    double sec_to_fmin(long sec) {
       return (double)sec / (double)SEC_PER_MIN;
    }

    //----------------------------------------------------------------------
    // Hours <-> Milliseconds / Seconds
    //----------------------------------------------------------------------

    /// \brief Convert hours to milliseconds.
    /// \param hr Hours value.
    /// \return Milliseconds value.
    long hour_to_ms(double hr) {
       return (long)MathRound(hr * MS_PER_HOUR);
    }

    /// \brief Convert milliseconds to hours (integer part).
    /// \param ms Milliseconds value.
    /// \return Hours value.
    int ms_to_hour(long ms) {
       return (int)(ms / MS_PER_HOUR);
    }

    /// \brief Convert hours to seconds.
    /// \param hr Hours value.
    /// \return Seconds value.
    long hour_to_sec(double hr) {
       return (long)MathRound(hr * SEC_PER_HOUR);
    }

    /// \brief Convert seconds to hours (integer part).
    /// \param sec Seconds value.
    /// \return Hours value.
    int sec_to_hour(long sec) {
       return (int)(sec / SEC_PER_HOUR);
    }

    /// \brief Convert hours to floating-point seconds.
    /// \param hr Hours value.
    /// \return Seconds as double.
    double hour_to_fsec(double hr) {
       return hr * SEC_PER_HOUR;
    }

    /// \brief Convert seconds to floating-point hours.
    /// \param sec Seconds value.
    /// \return Hours as double.
    double sec_to_fhour(long sec) {
       return (double)sec / (double)SEC_PER_HOUR;
    }

    //----------------------------------------------------------------------
    // DateTime conversions
    //----------------------------------------------------------------------

    /// \brief Convert a timestamp to a DateTimeStruct.
    /// \param ts Timestamp in seconds since UNIX epoch.
    /// \return Filled DateTimeStruct.
    DateTimeStruct to_date_time(long ts) {
       MqlDateTime tmp; 
       TimeToStruct((datetime)ts, tmp);
       DateTimeStruct dt;
       dt.year = tmp.year;
       dt.mon  = tmp.mon;
       dt.day  = tmp.day;
       dt.hour = tmp.hour;
       dt.min  = tmp.min;
       dt.sec  = tmp.sec;
       dt.ms   = 0;
       return dt;
    }

    /// \brief Alias for to_date_time.
    /// \copydoc to_date_time
    DateTimeStruct to_dt(long ts) { return to_date_time(ts); }

    /// \brief Convert a DateTimeStruct to timestamp.
    /// \param dt Structure with date and time fields.
    /// \return Timestamp in seconds.
    long dt_to_timestamp(const DateTimeStruct &dt) {
       MqlDateTime tmp;
       tmp.year = (int)dt.year;
       tmp.mon  = dt.mon;
       tmp.day  = dt.day;
       tmp.hour = dt.hour;
       tmp.min  = dt.min;
       tmp.sec  = dt.sec;
       return (long)StructToTime(tmp);
    }

    /// \brief Alias for dt_to_timestamp.
    /// \copydoc dt_to_timestamp
    long to_timestamp(const DateTimeStruct &dt) { return dt_to_timestamp(dt); }

    //----------------------------------------------------------------------
    // Start/End of intervals
    //----------------------------------------------------------------------

    /// \brief Get the start of the day for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 00:00:00 of the same day.
    long start_of_day(long ts) {
       return ts - (ts % SEC_PER_DAY);
    }

    /// \brief Get the end of the day for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 23:59:59 of the same day.
    long end_of_day(long ts) {
       return ts - (ts % SEC_PER_DAY) + SEC_PER_DAY - 1;
    }

    /// \brief Get the start of the year for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 00:00:00 on January 1st of the year.
    long start_of_year(long ts) {
       MqlDateTime dt; TimeToStruct((datetime)ts, dt);
       dt.mon = 1; dt.day = 1; dt.hour = 0; dt.min = 0; dt.sec = 0;
       return (long)StructToTime(dt);
    }

    /// \brief Get the end of the year for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp of 23:59:59 on December 31st of the year.
    long end_of_year(long ts) {
       MqlDateTime dt; TimeToStruct((datetime)ts, dt);
       dt.year++; dt.mon = 1; dt.day = 1; dt.hour = 0; dt.min = 0; dt.sec = 0;
       return (long)StructToTime(dt) - 1;
    }

    /// \brief Get the start of a period.
    /// \param p Period length in seconds.
    /// \param ts Timestamp (default current). Use time_utils.ts().
    /// \return Timestamp of the start of the period.
    long start_of_period(int p, long ts) {
       return ts - (ts % p);
    }

    /// \brief Get the end of a period.
    /// \param p Period length in seconds.
    /// \param ts Timestamp (default current).
    /// \return Timestamp of the end of the period.
    long end_of_period(int p, long ts) {
       return ts - (ts % p) + p - 1;
    }

    /// \}

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_CONVERSIONS_MQH__
