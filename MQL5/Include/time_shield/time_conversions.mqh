//+------------------------------------------------------------------+
//|                                             time_conversions.mqh |
//|                              Time Shield - MQL5 Time Conversions |
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
#include <time_shield/enums.mqh>
#include <time_shield/validation.mqh>
#include <time_shield/time_zone_struct.mqh>

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

    /// \brief Convert a 24-hour format hour to a 12-hour format.
    /// \param hour Hour value in 24-hour format.
    /// \return Hour value in 12-hour format.
    int hour24_to_12(int hour) {
       if(hour == 0 || hour > 12) return 12;
       return hour;
    }

    /// \brief Alias for hour24_to_12.
    /// \copydoc hour24_to_12
    int h24_to_h12(int hour) { return hour24_to_12(hour); }

    //----------------------------------------------------------------------
    // Timestamp <-> Year
    //----------------------------------------------------------------------

    /// \brief Convert a UNIX timestamp to a year.
    /// \param ts UNIX timestamp in seconds.
    /// \return Year corresponding to the given timestamp.
    long get_unix_year(long ts) {
       const long BIAS_292277022000 = 9223371890843040000;
       const long BIAS_2000         = 946684800;

       long y    = MAX_YEAR;
       long secs = -((ts - BIAS_2000) - BIAS_292277022000);

       long n_400_years = secs / SEC_PER_400_YEARS;
       secs -= n_400_years * SEC_PER_400_YEARS;
       y    -= n_400_years * 400;

       long n_100_years = secs / SEC_PER_100_YEARS;
       secs -= n_100_years * SEC_PER_100_YEARS;
       y    -= n_100_years * 100;

       long n_4_years = secs / SEC_PER_4_YEARS;
       secs -= n_4_years * SEC_PER_4_YEARS;
       y    -= n_4_years * 4;

       long n_1_years = secs / SEC_PER_YEAR;
       secs -= n_1_years * SEC_PER_YEAR;
       y    -= n_1_years;

       y = secs == 0 ? y : y - 1;
       return y - UNIX_EPOCH;
    }

    /// \brief Alias for get_unix_year function.
    /// \copydoc get_unix_year
    long unix_year(long ts) { return get_unix_year(ts); }

    /// \brief Alias for get_unix_year function.
    /// \copydoc get_unix_year
    long to_unix_year(long ts) { return get_unix_year(ts); }

    /// \brief Get the year from a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Year of the given timestamp.
    long get_year(long ts) {
       return get_unix_year(ts) + UNIX_EPOCH;
    }

    /// \brief Alias for get_year.
    /// \copydoc get_year
    long year(long ts) { return get_year(ts); }

    /// \brief Alias for get_year.
    /// \copydoc get_year
    long to_year(long ts) { return get_year(ts); }

    /// \brief Get the year from a millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Year of the given timestamp.
    long get_year_ms(long ts_ms) {
       return get_year(ms_to_sec(ts_ms));
    }

    /// \brief Alias for get_year_ms.
    /// \copydoc get_year_ms
    long year_ms(long ts_ms) { return get_year_ms(ts_ms); }

    /// \brief Alias for get_year_ms.
    /// \copydoc get_year_ms
    long to_year_ms(long ts_ms) { return get_year_ms(ts_ms); }

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

    /// \brief Convert a timestamp to the standard MqlDateTime structure.
    /// \param ts Timestamp in seconds since UNIX epoch.
    /// \return Filled MqlDateTime structure.
    MqlDateTime to_date_time_mql(long ts) {
       MqlDateTime dt;
       TimeToStruct((datetime)ts, dt);
       return dt;
    }

    /// \brief Alias for to_date_time_mql.
    /// \copydoc to_date_time_mql
    MqlDateTime to_mql_dt(long ts) { return to_date_time_mql(ts); }

    /// \brief Convert a timestamp in milliseconds to DateTimeStruct.
    /// \param ts_ms Timestamp in milliseconds since UNIX epoch.
    /// \return Filled DateTimeStruct with millisecond part.
    DateTimeStruct to_date_time_ms(long ts_ms) {
       DateTimeStruct dt = to_date_time(ms_to_sec(ts_ms));
       dt.ms = ms_of_ts(ts_ms);
       return dt;
    }

    /// \brief Alias for to_date_time_ms.
    /// \copydoc to_date_time_ms
    DateTimeStruct to_dt_ms(long ts_ms) { return to_date_time_ms(ts_ms); }

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

    /// \brief Convert an MqlDateTime structure to timestamp.
    /// \param dt Standard MqlDateTime structure.
    /// \return Timestamp in seconds.
    long dt_to_timestamp(const MqlDateTime &dt) {
       return (long)StructToTime(dt);
    }

    /// \brief Alias for dt_to_timestamp.
    /// \copydoc dt_to_timestamp
    long to_timestamp(const DateTimeStruct &dt) { return dt_to_timestamp(dt); }

    /// \brief Alias for dt_to_timestamp taking MqlDateTime.
    /// \copydoc dt_to_timestamp
    long to_timestamp(const MqlDateTime &dt) { return dt_to_timestamp(dt); }

    /// \brief Convert date and time values to a timestamp.
    /// \param year Year value.
    /// \param mon  Month value.
    /// \param day  Day value.
    /// \param hour Hour value.
    /// \param min  Minute value.
    /// \param sec  Second value.
    /// \return Timestamp in seconds.
    long to_timestamp(long year, int mon, int day, int hour=0, int min=0, int sec=0) {
       MqlDateTime dt;
       dt.year = (int)year;
       dt.mon  = mon;
       dt.day  = day;
       dt.hour = hour;
       dt.min  = min;
       dt.sec  = sec;
       return (long)StructToTime(dt);
    }

    /// \brief Alias for to_timestamp with explicit date fields.
    /// \copydoc to_timestamp
    long to_ts(long year, int mon, int day, int hour=0, int min=0, int sec=0) {
       return to_timestamp(year, mon, day, hour, min, sec);
    }

    /// \brief Convert date and time values to a timestamp in milliseconds.
    /// \param year Year value.
    /// \param mon  Month value.
    /// \param day  Day value.
    /// \param hour Hour value.
    /// \param min  Minute value.
    /// \param sec  Second value.
    /// \param ms   Millisecond value.
    /// \return Timestamp in milliseconds.
    long to_timestamp_ms(long year, int mon, int day, int hour=0, int min=0, int sec=0, int ms=0) {
       return sec_to_ms(to_timestamp(year, mon, day, hour, min, sec)) + ms;
    }

    /// \brief Alias for to_timestamp_ms.
    /// \copydoc to_timestamp_ms
    long to_ts_ms(long year, int mon, int day, int hour=0, int min=0, int sec=0, int ms=0) {
       return to_timestamp_ms(year, mon, day, hour, min, sec, ms);
    }

    /// \brief Alias for to_timestamp_ms.
    /// \copydoc to_timestamp_ms
    long ts_ms(long year, int mon, int day, int hour=0, int min=0, int sec=0, int ms=0) {
       return to_timestamp_ms(year, mon, day, hour, min, sec, ms);
    }

    /// \brief Convert a DateTimeStruct to a timestamp in milliseconds.
    /// \param dt Structure with date and time fields.
    /// \return Timestamp in milliseconds.
    long dt_to_timestamp_ms(const DateTimeStruct &dt) {
       return sec_to_ms(dt_to_timestamp(dt)) + dt.ms;
    }

    /// \brief Convert an MqlDateTime structure to a timestamp in milliseconds.
    /// \param dt Standard MqlDateTime structure.
    /// \return Timestamp in milliseconds.
    long dt_to_timestamp_ms(const MqlDateTime &dt) {
       return sec_to_ms(dt_to_timestamp(dt));
    }

    /// \brief Alias for dt_to_timestamp_ms.
    /// \copydoc dt_to_timestamp_ms
    long to_timestamp_ms(const DateTimeStruct &dt) { return dt_to_timestamp_ms(dt); }

    /// \brief Alias for dt_to_timestamp_ms.
    /// \copydoc dt_to_timestamp_ms
    long to_timestamp_ms(const MqlDateTime &dt) { return dt_to_timestamp_ms(dt); }

    /// \brief Convert a DateTimeStruct to floating-point timestamp.
    /// \param dt Structure with date and time fields.
    /// \return Floating-point timestamp.
    double dt_to_ftimestamp(const DateTimeStruct &dt) {
       return (double)dt_to_timestamp(dt) + (double)dt.ms / (double)MS_PER_SEC;
    }

    /// \brief Convert an MqlDateTime structure to floating-point timestamp.
    /// \param dt Standard MqlDateTime structure.
    /// \return Floating-point timestamp.
    double dt_to_ftimestamp(const MqlDateTime &dt) {
       return (double)dt_to_timestamp(dt);
    }

    /// \brief Alias for dt_to_ftimestamp.
    /// \copydoc dt_to_ftimestamp
    double to_ftimestamp(const DateTimeStruct &dt) { return dt_to_ftimestamp(dt); }

    /// \brief Alias for dt_to_ftimestamp.
    /// \copydoc dt_to_ftimestamp
    double to_ftimestamp(const MqlDateTime &dt) { return dt_to_ftimestamp(dt); }

    //----------------------------------------------------------------------
    // Start/End of intervals
    //----------------------------------------------------------------------

    /// \brief Get the start of the day for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 00:00:00 of the same day.
    long start_of_day(long ts) {
       return ts - (ts % SEC_PER_DAY);
    }

    /// \brief Alias for start_of_day.
    /// \copydoc start_of_day
    long day_start(long ts) { return start_of_day(ts); }

    /// \brief Get the start of the previous day.
    /// \param ts   Timestamp in seconds.
    /// \param days Number of days to go back (default 1).
    /// \return Timestamp at 00:00:00 of the previous day.
    long start_of_prev_day(long ts, int days=1) {
       return start_of_day(ts) - days * SEC_PER_DAY;
    }

    /// \brief Alias for start_of_prev_day.
    /// \copydoc start_of_prev_day
    long previous_day_start(long ts, int days=1) { return start_of_prev_day(ts, days); }

    /// \brief Get the start of the day in seconds from milliseconds timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at 00:00:00 of the same day in seconds.
    long start_of_day_sec(long ts_ms) {
       return start_of_day(ms_to_sec(ts_ms));
    }

    /// \brief Alias for start_of_day_sec.
    /// \copydoc start_of_day_sec
    long day_start_sec(long ts_ms) { return start_of_day_sec(ts_ms); }

    /// \brief Get the start of the day for a millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at 00:00:00.000 of the same day.
    long start_of_day_ms(long ts_ms) {
       return ts_ms - (ts_ms % MS_PER_DAY);
    }

    /// \brief Alias for start_of_day_ms.
    /// \copydoc start_of_day_ms
    long day_start_ms(long ts_ms) { return start_of_day_ms(ts_ms); }

    /// \brief Get the start of the next day.
    /// \param ts   Timestamp in seconds.
    /// \param days Number of days ahead (default 1).
    /// \return Timestamp at 00:00:00 of the next day.
    long start_of_next_day(long ts, int days=1) {
       return start_of_day(ts) + days * SEC_PER_DAY;
    }

    /// \brief Alias for start_of_next_day.
    /// \copydoc start_of_next_day
    long next_day_start(long ts, int days=1) { return start_of_next_day(ts, days); }

    /// \brief Get the start of the next day in milliseconds.
    /// \param ts_ms Timestamp in milliseconds.
    /// \param days  Number of days ahead (default 1).
    /// \return Timestamp at 00:00:00.000 of the next day in milliseconds.
    long start_of_next_day_ms(long ts_ms, int days=1) {
       return start_of_day_ms(ts_ms) + days * MS_PER_DAY;
    }

    /// \brief Alias for start_of_next_day_ms.
    /// \copydoc start_of_next_day_ms
    long next_day_start_ms(long ts_ms, int days=1) { return start_of_next_day_ms(ts_ms, days); }

    /// \brief Add days to a timestamp without adjusting to start of day.
    /// \param ts   Timestamp in seconds.
    /// \param days Number of days to add (default 1).
    /// \return Timestamp shifted forward by the given days.
    long next_day(long ts, int days=1) {
       return ts + days * SEC_PER_DAY;
    }

    /// \brief Add days to a millisecond timestamp without adjusting to start of day.
    /// \param ts_ms Timestamp in milliseconds.
    /// \param days  Number of days to add (default 1).
    /// \return Timestamp shifted forward by the given days in milliseconds.
    long next_day_ms(long ts_ms, int days=1) {
       return ts_ms + days * MS_PER_DAY;
    }

    /// \brief Get the end of the day for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 23:59:59 of the same day.
    long end_of_day(long ts) {
       return ts - (ts % SEC_PER_DAY) + SEC_PER_DAY - 1;
    }

    /// \brief Alias for end_of_day.
    /// \copydoc end_of_day
    long day_end(long ts) { return end_of_day(ts); }

    /// \brief Get the end of the day in seconds from milliseconds timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at 23:59:59 of the same day in seconds.
    long end_of_day_sec(long ts_ms) {
       return end_of_day(ms_to_sec(ts_ms));
    }

    /// \brief Alias for end_of_day_sec.
    /// \copydoc end_of_day_sec
    long day_end_sec(long ts_ms) { return end_of_day_sec(ts_ms); }

    /// \brief Get the end of the day for a millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at 23:59:59.999 of the same day.
    long end_of_day_ms(long ts_ms) {
       return ts_ms - (ts_ms % MS_PER_DAY) + MS_PER_DAY - 1;
    }

    /// \brief Alias for end_of_day_ms.
    /// \copydoc end_of_day_ms
    long day_end_ms(long ts_ms) { return end_of_day_ms(ts_ms); }

    /// \brief Get the day of the week for a date.
    /// \param year  Year value.
    /// \param month Month value.
    /// \param day   Day of the month.
    /// \return Weekday (SUN=0, MON=1, ... SAT=6).
    int day_of_week_date(long year, int month, int day) {
       long a = (14 - month) / MONTHS_PER_YEAR;
       long y = year - a;
       long m = month + MONTHS_PER_YEAR * a - 2;
       long r = 7000 + day + y + (y / 4) - (y / 100) + (y / 400) + (31 * m) / MONTHS_PER_YEAR;
       return (int)(r % DAYS_PER_WEEK);
    }

    /// \brief Alias for day_of_week_date.
    /// \copydoc day_of_week_date
    int get_weekday(long year, int month, int day) { return day_of_week_date(year, month, day); }

    /// \brief Alias for day_of_week_date.
    /// \copydoc day_of_week_date
    int day_of_week(long year, int month, int day) { return day_of_week_date(year, month, day); }

    /// \brief Get weekday from a DateTimeStruct.
    /// \param dt Date structure with fields year, mon, day.
    /// \return Weekday (SUN=0, MON=1, ... SAT=6).
    int get_weekday_from_date(const DateTimeStruct &dt) {
       return day_of_week_date(dt.year, dt.mon, dt.day);
    }

    /// \brief Get weekday from a MqlDateTime structure.
    /// \param dt Standard MqlDateTime structure.
    /// \return Weekday (SUN=0, MON=1, ... SAT=6).
    int get_weekday_from_date(const MqlDateTime &dt) {
       return day_of_week_date(dt.year, dt.mon, dt.day);
    }

    /// \brief Alias for get_weekday_from_date with DateTimeStruct.
    /// \copydoc get_weekday_from_date(const DateTimeStruct&)
    int day_of_week_dt(const DateTimeStruct &dt) { return get_weekday_from_date(dt); }

    /// \brief Alias for get_weekday_from_date with DateTimeStruct.
    /// \copydoc get_weekday_from_date(const DateTimeStruct&)
    int day_of_week(const DateTimeStruct &dt) { return get_weekday_from_date(dt); }

    /// \brief Alias for get_weekday_from_date with MqlDateTime.
    /// \copydoc get_weekday_from_date(const MqlDateTime&)
    int day_of_week_dt(const MqlDateTime &dt) { return get_weekday_from_date(dt); }

    /// \brief Alias for get_weekday_from_date with MqlDateTime.
    /// \copydoc get_weekday_from_date(const MqlDateTime&)
    int day_of_week(const MqlDateTime &dt) { return get_weekday_from_date(dt); }

    /// \brief Get weekday from timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Weekday (SUN=0, MON=1, ... SAT=6).
    int get_weekday_from_ts(long ts) {
       return (int)((ts / SEC_PER_DAY + THU) % DAYS_PER_WEEK);
    }

    /// \brief Alias for get_weekday_from_ts.
    /// \copydoc get_weekday_from_ts
    int day_of_week(long ts) { return get_weekday_from_ts(ts); }

    /// \brief Get weekday from millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Weekday (SUN=0, MON=1, ... SAT=6).
    int get_weekday_from_ts_ms(long ts_ms) {
       return get_weekday_from_ts(ms_to_sec(ts_ms));
    }

    /// \brief Alias for get_weekday_from_ts_ms.
    /// \copydoc get_weekday_from_ts_ms
    int day_of_week_ms(long ts_ms) { return get_weekday_from_ts_ms(ts_ms); }

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

    /// \brief Alias for start_of_year.
    /// \copydoc start_of_year
    long year_start(long ts) { return start_of_year(ts); }

    /// \brief Alias for start_of_year.
    /// \copydoc start_of_year
    long year_begin(long ts) { return start_of_year(ts); }

    /// \brief Get the start of the year in milliseconds.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at 00:00:00 on January 1st in milliseconds.
    long start_of_year_ms(long ts_ms) {
       return sec_to_ms(start_of_year(ms_to_sec(ts_ms)));
    }

    /// \brief Alias for start_of_year_ms.
    /// \copydoc start_of_year_ms
    long year_start_ms(long ts_ms) { return start_of_year_ms(ts_ms); }

    /// \brief Alias for start_of_year_ms.
    /// \copydoc start_of_year_ms
    long year_begin_ms(long ts_ms) { return start_of_year_ms(ts_ms); }

    /// \brief Get the timestamp for the start of the specified year.
    /// \param year Year value.
    /// \return Timestamp at 00:00:00 on January 1st of the given year.
    long start_of_year_date(long year) {
       if(year < 2100) {
          long year_diff      = year >= UNIX_EPOCH ? year - UNIX_EPOCH : UNIX_EPOCH - year;
          long year_start_ts  = (year_diff / 4) * SEC_PER_4_YEARS;
          long year_remainder = year_diff % 4;
          long SEC_PER_YEAR_X2 = 2 * SEC_PER_YEAR;
          long SEC_PER_YEAR_V2 = SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR;
          switch(year_remainder) {
             case 0: return year_start_ts;
             case 1: return year_start_ts + SEC_PER_YEAR;
             case 2: return year_start_ts + SEC_PER_YEAR_X2;
             default: return year_start_ts + SEC_PER_YEAR_V2;
          }
          return year_start_ts + SEC_PER_YEAR_V2;
       }
       return to_timestamp(year, 1, 1);
    }

    /// \brief Alias for start_of_year_date.
    /// \copydoc start_of_year_date
    long year_start_date(long year) { return start_of_year_date(year); }

    /// \brief Alias for start_of_year_date.
    /// \copydoc start_of_year_date
    long year_begin_date(long year) { return start_of_year_date(year); }

    /// \brief Get the timestamp in milliseconds for the start of the specified year.
    /// \param year Year value.
    /// \return Timestamp at 00:00:00 on January 1st in milliseconds.
    long start_of_year_date_ms(long year) {
       return sec_to_ms(start_of_year_date(year));
    }

    /// \brief Alias for start_of_year_date_ms.
    /// \copydoc start_of_year_date_ms
    long year_start_date_ms(long year) { return start_of_year_date_ms(year); }

    /// \brief Alias for start_of_year_date_ms.
    /// \copydoc start_of_year_date_ms
    long year_begin_date_ms(long year) { return start_of_year_date_ms(year); }

    /// \brief Get the end of the year in milliseconds.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp of 23:59:59.999 on December 31st of the year.
    long end_of_year_ms(long ts_ms) {
       return sec_to_ms(end_of_year(ms_to_sec(ts_ms)));
    }

    /// \brief Alias for end_of_year_ms.
    /// \copydoc end_of_year_ms
    long year_end_ms(long ts_ms) { return end_of_year_ms(ts_ms); }

    /// \brief Get the day of the year.
    /// \param ts Timestamp in seconds.
    /// \return Day of the year (1-366).
    int day_of_year(long ts) {
       return (int)((ts - start_of_year(ts)) / SEC_PER_DAY) + 1;
    }

    /// \brief Get the month of the year.
    /// \param ts Timestamp in seconds.
    /// \return Month of the year.
    Month month_of_year(long ts) {
       const int JAN_AND_FEB_DAY_LEAP_YEAR = 60;
       static const int TABLE_MONTH_OF_YEAR[] = {
           0,
           1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
           2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
           3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
           4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
           5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
           6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
           8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
           9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
           10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
           11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
           12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
       };
       int dy = day_of_year(ts);
       if(is_leap_year(ts) && dy >= JAN_AND_FEB_DAY_LEAP_YEAR)
          return (Month)TABLE_MONTH_OF_YEAR[dy - 1];
       return (Month)TABLE_MONTH_OF_YEAR[dy];
    }

    /// \brief Get the day of the month.
    /// \param ts Timestamp in seconds.
    /// \return Day of the month.
    int day_of_month(long ts) {
       const int JAN_AND_FEB_DAY_LEAP_YEAR = 60;
       static const int TABLE_DAY_OF_YEAR[] = {
           0,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
           1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
       };
       int dy = day_of_year(ts);
       if(is_leap_year(ts)) {
          if(dy == JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1] + 1;
          if(dy > JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1];
       }
       return TABLE_DAY_OF_YEAR[dy];
    }

    /// \brief Get the number of days in a month.
    /// \param year Year value.
    /// \param month Month value.
    /// \return Number of days in the month.
    int num_days_in_month(long year, int month) {
       if(month > MONTHS_PER_YEAR || month < 0) return 0;
       static const int num_days[13] = {0,31,30,31,30,31,30,31,31,30,31,30,31};
       if(month == FEB) {
          if(is_leap_year_date(year)) return 29;
          return 28;
       }
       return num_days[month];
    }

    /// \brief Alias for num_days_in_month.
    /// \copydoc num_days_in_month
    int days_in_month(long year, int month) { return num_days_in_month(year, month); }

    /// \brief Get the number of days in the month of a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Number of days in that month.
    int num_days_in_month_ts(long ts) {
       static const int num_days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
       int month = month_of_year(ts);
       if(month == FEB) {
          return is_leap_year(ts) ? 29 : 28;
       }
       return num_days[month];
    }

    /// \brief Alias for num_days_in_month_ts.
    /// \copydoc num_days_in_month_ts
    int num_days_in_month(long ts) { return num_days_in_month_ts(ts); }

    /// \brief Alias for num_days_in_month_ts.
    /// \copydoc num_days_in_month_ts
    int days_in_month(long ts) { return num_days_in_month_ts(ts); }

    /// \brief Get number of days in a year.
    /// \param year Year value.
    /// \return Days in the year.
    int num_days_in_year(long year) {
       if(is_leap_year_date(year)) return DAYS_PER_LEAP_YEAR;
       return DAYS_PER_YEAR;
    }

    /// \brief Alias for num_days_in_year.
    /// \copydoc num_days_in_year
    int days_in_year(long year) { return num_days_in_year(year); }

    /// \brief Get number of days in the year of the timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Days in the year of the timestamp.
    int num_days_in_year_ts(long ts) {
       if(is_leap_year_ts(ts)) return DAYS_PER_LEAP_YEAR;
       return DAYS_PER_YEAR;
    }

    /// \brief Alias for num_days_in_year_ts.
    /// \copydoc num_days_in_year_ts
    int days_in_year_ts(long ts) { return num_days_in_year_ts(ts); }

    /// \brief Get the timestamp at the start of the month.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 00:00:00 on the first day of the month.
    long start_of_month(long ts) {
       return start_of_day(ts) - (day_of_month(ts) - 1) * SEC_PER_DAY;
    }

    /// \brief Alias for start_of_month.
    /// \copydoc start_of_month
    long month_begin(long ts) { return start_of_month(ts); }

    /// \brief Get the timestamp at the end of the month.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 23:59:59 on the last day of the month.
    long end_of_month(long ts) {
       return end_of_day(ts) + (num_days_in_month(ts) - day_of_month(ts)) * SEC_PER_DAY;
    }

    /// \brief Alias for end_of_month.
    /// \copydoc end_of_month
    long last_day_of_month(long ts) { return end_of_month(ts); }

    /// \brief Get the timestamp of the last Sunday of the month.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 00:00:00 of the last Sunday of the month.
    long last_sunday_of_month(long ts) {
       return end_of_month(ts) - day_of_week(ts) * SEC_PER_DAY;
    }

    /// \brief Alias for last_sunday_of_month.
    /// \copydoc last_sunday_of_month
    long final_sunday_of_month(long ts) { return last_sunday_of_month(ts); }

    /// \brief Get the day of the last Sunday for the given month and year.
    /// \param year Year value.
    /// \param month Month value.
    /// \return Day of the last Sunday of that month.
    int last_sunday_month_day(long year, int month) {
       int days = num_days_in_month(year, month);
       return days - day_of_week_date(year, month, days);
    }

    /// \brief Alias for last_sunday_month_day.
    /// \copydoc last_sunday_month_day
    int final_sunday_month_day(long year, int month) { return last_sunday_month_day(year, month); }

    /// \brief Get the start of the hour for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at HH:00:00 of the same hour.
    long start_of_hour(long ts) {
       return ts - (ts % SEC_PER_HOUR);
    }

    /// \brief Alias for start_of_hour.
    /// \copydoc start_of_hour
    long hour_begin(long ts) { return start_of_hour(ts); }

    /// \brief Get the start of the hour in seconds from milliseconds timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at HH:00:00 of the hour in seconds.
    long start_of_hour_sec(long ts_ms) {
       return start_of_hour(ms_to_sec(ts_ms));
    }

    /// \brief Alias for start_of_hour_sec.
    /// \copydoc start_of_hour_sec
    long hour_begin_sec(long ts_ms) { return start_of_hour_sec(ts_ms); }

    /// \brief Get the start of the hour for a millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at HH:00:00.000 of the hour.
    long start_of_hour_ms(long ts_ms) {
       return ts_ms - (ts_ms % MS_PER_HOUR);
    }

    /// \brief Alias for start_of_hour_ms.
    /// \copydoc start_of_hour_ms
    long hour_begin_ms(long ts_ms) { return start_of_hour_ms(ts_ms); }

    /// \brief Get the end of the hour for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at HH:59:59 of the same hour.
    long end_of_hour(long ts) {
       return ts - (ts % SEC_PER_HOUR) + SEC_PER_HOUR - 1;
    }

    /// \brief Alias for end_of_hour.
    /// \copydoc end_of_hour
    long finish_of_hour(long ts) { return end_of_hour(ts); }

    /// \brief Get the end of the hour in seconds from milliseconds timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at HH:59:59 of the hour in seconds.
    long end_of_hour_sec(long ts_ms) {
       return end_of_hour(ms_to_sec(ts_ms));
    }

    /// \brief Alias for end_of_hour_sec.
    /// \copydoc end_of_hour_sec
    long finish_of_hour_sec(long ts_ms) { return end_of_hour_sec(ts_ms); }

    /// \brief Get the end of the hour for a millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Timestamp at HH:59:59.999 of the hour.
    long end_of_hour_ms(long ts_ms) {
       return ts_ms - (ts_ms % MS_PER_HOUR) + MS_PER_HOUR - 1;
    }

    /// \brief Alias for end_of_hour_ms.
    /// \copydoc end_of_hour_ms
    long finish_of_hour_ms(long ts_ms) { return end_of_hour_ms(ts_ms); }

    /// \brief Get the hour of the day.
    /// \param ts Timestamp in seconds.
    /// \return Hour value 0-23.
    int hour_of_day(long ts) {
       return (int)((ts / SEC_PER_HOUR) % HOURS_PER_DAY);
    }

    /// \brief Alias for hour_of_day.
    /// \copydoc hour_of_day
    int hour_in_day(long ts) { return hour_of_day(ts); }

    /// \brief Get the start of the week (Sunday).
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 00:00:00 on Sunday of the current week.
    long start_of_week(long ts) {
       return start_of_day(ts) - day_of_week(ts) * SEC_PER_DAY;
    }

    /// \brief Alias for start_of_week.
    /// \copydoc start_of_week
    long week_begin(long ts) { return start_of_week(ts); }

    /// \brief Get the end of the week (Saturday end).
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 23:59:59 on Saturday of the current week.
    long end_of_week(long ts) {
       return start_of_day(ts) + (DAYS_PER_WEEK - day_of_week(ts)) * SEC_PER_DAY - 1;
    }

    /// \brief Alias for end_of_week.
    /// \copydoc end_of_week
    long finish_of_week(long ts) { return end_of_week(ts); }

    /// \brief Get the start of Saturday for the week of the timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at 00:00:00 on Saturday of the current week.
    long start_of_saturday(long ts) {
       return start_of_day(ts) + (SAT - day_of_week(ts)) * SEC_PER_DAY;
    }

    /// \brief Alias for start_of_saturday.
    /// \copydoc start_of_saturday
    long saturday_begin(long ts) { return start_of_saturday(ts); }

    /// \brief Get the start of the minute for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at mm:00 of the same minute.
    long start_of_min(long ts) {
       return ts - (ts % SEC_PER_MIN);
    }

    /// \brief Alias for start_of_min.
    /// \copydoc start_of_min
    long min_begin(long ts) { return start_of_min(ts); }

    /// \brief Get the end of the minute for a timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Timestamp at mm:59 of the same minute.
    long end_of_min(long ts) {
       return ts - (ts % SEC_PER_MIN) + SEC_PER_MIN - 1;
    }

    /// \brief Alias for end_of_min.
    /// \copydoc end_of_min
    long finish_of_min(long ts) { return end_of_min(ts); }

    /// \brief Get the minute of the day.
    /// \param ts Timestamp in seconds.
    /// \return Minute of the day (0-1439).
    int min_of_day(long ts) {
       return (int)((ts / SEC_PER_MIN) % MIN_PER_DAY);
    }

    /// \brief Get the minute of the hour.
    /// \param ts Timestamp in seconds.
    /// \return Minute of the hour (0-59).
    int min_of_hour(long ts) {
       return (int)((ts / SEC_PER_MIN) % MIN_PER_HOUR);
    }

    /// \brief Alias for min_of_hour.
    /// \copydoc min_of_hour
    int min_in_hour(long ts) { return min_of_hour(ts); }

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

    //----------------------------------------------------------------------
    // UNIX day and minute helpers
    //----------------------------------------------------------------------

    /// \brief Get UNIX day from timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Number of days since UNIX epoch.
    long get_unix_day(long ts) {
       return ts / SEC_PER_DAY;
    }

    /// \brief Alias for get_unix_day.
    /// \copydoc get_unix_day
    long unix_day(long ts) { return get_unix_day(ts); }

    /// \brief Alias for get_unix_day.
    /// \copydoc get_unix_day
    long get_unixday(long ts) { return get_unix_day(ts); }

    /// \brief Alias for get_unix_day.
    /// \copydoc get_unix_day
    long unixday(long ts) { return get_unix_day(ts); }

    /// \brief Alias for get_unix_day.
    /// \copydoc get_unix_day
    long uday(long ts) { return get_unix_day(ts); }

    /// \brief Get number of days between two timestamps.
    /// \param start Start timestamp in seconds.
    /// \param stop  End timestamp in seconds.
    /// \return Difference in days.
    int get_days_difference(long start, long stop) {
       return (int)((stop - start) / SEC_PER_DAY);
    }

    /// \brief Alias for get_days_difference.
    /// \copydoc get_days_difference
    int get_days(long start, long stop) { return get_days_difference(start, stop); }

    /// \brief Alias for get_days_difference.
    /// \copydoc get_days_difference
    int days(long start, long stop) { return get_days_difference(start, stop); }

    /// \brief Get UNIX day from milliseconds timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Number of days since UNIX epoch.
    long get_unix_day_ms(long ts_ms) {
       return get_unix_day(ms_to_sec(ts_ms));
    }

    /// \brief Alias for get_unix_day_ms.
    /// \copydoc get_unix_day_ms
    long unix_day_ms(long ts_ms) { return get_unix_day_ms(ts_ms); }

    /// \brief Alias for get_unix_day_ms.
    /// \copydoc get_unix_day_ms
    long get_unixday_ms(long ts_ms) { return get_unix_day_ms(ts_ms); }

    /// \brief Alias for get_unix_day_ms.
    /// \copydoc get_unix_day_ms
    long unixday_ms(long ts_ms) { return get_unix_day_ms(ts_ms); }

    /// \brief Alias for get_unix_day_ms.
    /// \copydoc get_unix_day_ms
    long uday_ms(long ts_ms) { return get_unix_day_ms(ts_ms); }

    /// \brief Convert UNIX day to timestamp in seconds.
    /// \param uday UNIX day value.
    /// \return Timestamp at start of the day.
    long unix_day_to_timestamp(long uday) {
       return uday * SEC_PER_DAY;
    }

    /// \brief Alias for unix_day_to_timestamp.
    /// \copydoc unix_day_to_timestamp
    long unix_day_to_ts(long uday) { return unix_day_to_timestamp(uday); }

    /// \brief Alias for unix_day_to_timestamp.
    /// \copydoc unix_day_to_timestamp
    long unixday_to_ts(long uday) { return unix_day_to_timestamp(uday); }

    /// \brief Alias for unix_day_to_timestamp.
    /// \copydoc unix_day_to_timestamp
    long uday_to_ts(long uday) { return unix_day_to_timestamp(uday); }

    /// \brief Alias for unix_day_to_timestamp.
    /// \copydoc unix_day_to_timestamp
    long start_of_day_from_unix_day(long uday) { return unix_day_to_timestamp(uday); }

    /// \brief Convert UNIX day to timestamp in milliseconds.
    /// \param uday UNIX day value.
    /// \return Timestamp at start of the day in milliseconds.
    long unix_day_to_timestamp_ms(long uday) {
       return uday * MS_PER_DAY;
    }

    /// \brief Alias for unix_day_to_timestamp_ms.
    /// \copydoc unix_day_to_timestamp_ms
    long unix_day_to_ts_ms(long uday) { return unix_day_to_timestamp_ms(uday); }

    /// \brief Alias for unix_day_to_timestamp_ms.
    /// \copydoc unix_day_to_timestamp_ms
    long unixday_to_ts_ms(long uday) { return unix_day_to_timestamp_ms(uday); }

    /// \brief Alias for unix_day_to_timestamp_ms.
    /// \copydoc unix_day_to_timestamp_ms
    long uday_to_ts_ms(long uday) { return unix_day_to_timestamp_ms(uday); }

    /// \brief Alias for unix_day_to_timestamp_ms.
    /// \copydoc unix_day_to_timestamp_ms
    long start_of_day_from_unix_day_ms(long uday) { return unix_day_to_timestamp_ms(uday); }

    /// \brief Get end of day timestamp from UNIX day.
    /// \param uday UNIX day value.
    /// \return Timestamp at 23:59:59 of the specified day.
    long end_of_day_from_unix_day(long uday) {
       return uday * SEC_PER_DAY + SEC_PER_DAY - 1;
    }

    /// \brief Get end of day timestamp in ms from UNIX day.
    /// \param uday UNIX day value.
    /// \return Timestamp at 23:59:59.999 of the specified day.
    long end_of_day_from_unix_day_ms(long uday) {
       return uday * MS_PER_DAY + MS_PER_DAY - 1;
    }

    /// \brief Alias for end_of_day_from_unix_day.
    /// \copydoc end_of_day_from_unix_day
    long eod_from_unix_day(long uday) { return end_of_day_from_unix_day(uday); }

    /// \brief Alias for end_of_day_from_unix_day_ms.
    /// \copydoc end_of_day_from_unix_day_ms
    long eod_from_unix_day_ms(long uday) { return end_of_day_from_unix_day_ms(uday); }

    /// \brief Get start of next day timestamp from UNIX day.
    /// \param uday UNIX day value.
    /// \return Timestamp at start of next day in seconds.
    long start_of_next_day_from_unix_day(long uday) {
       return uday * SEC_PER_DAY + SEC_PER_DAY;
    }

    /// \brief Get start of next day timestamp in ms from UNIX day.
    /// \param uday UNIX day value.
    /// \return Timestamp at start of next day in milliseconds.
    long start_of_next_day_from_unix_day_ms(long uday) {
       return uday * MS_PER_DAY + MS_PER_DAY;
    }

    /// \brief Alias for start_of_next_day_from_unix_day.
    /// \copydoc start_of_next_day_from_unix_day
    long next_day_unix_day(long uday) { return start_of_next_day_from_unix_day(uday); }

    /// \brief Alias for start_of_next_day_from_unix_day.
    /// \copydoc start_of_next_day_from_unix_day
    long next_day_unixday(long uday) { return start_of_next_day_from_unix_day(uday); }

    /// \brief Alias for start_of_next_day_from_unix_day_ms.
    /// \copydoc start_of_next_day_from_unix_day_ms
    long next_day_unix_day_ms(long uday) { return start_of_next_day_from_unix_day_ms(uday); }

    /// \brief Alias for start_of_next_day_from_unix_day_ms.
    /// \copydoc start_of_next_day_from_unix_day_ms
    long next_day_unixday_ms(long uday) { return start_of_next_day_from_unix_day_ms(uday); }

    /// \brief Alias for start_of_next_day_from_unix_day.
    /// \copydoc start_of_next_day_from_unix_day
    long next_day_from_unix_day(long uday) { return start_of_next_day_from_unix_day(uday); }

    /// \brief Get UNIX minute from timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Minutes since UNIX epoch.
    long get_unix_min(long ts) {
       return ts / SEC_PER_MIN;
    }

    /// \brief Alias for get_unix_min.
    /// \copydoc get_unix_min
    long unix_min(long ts) { return get_unix_min(ts); }

    /// \brief Alias for get_unix_min.
    /// \copydoc get_unix_min
    long to_unix_min(long ts) { return get_unix_min(ts); }

    /// \brief Alias for get_unix_min.
    /// \copydoc get_unix_min
    long umin(long ts) { return get_unix_min(ts); }

    /// \brief Get second of day from timestamp.
    /// \param ts Timestamp in seconds.
    /// \return Second of the day.
    int sec_of_day(long ts) {
       return (int)(ts % SEC_PER_DAY);
    }

    /// \brief Get second of day from milliseconds timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Second of the day.
    int sec_of_day_ms(long ts_ms) {
       return sec_of_day(ms_to_sec(ts_ms));
    }

    /// \brief Get second of day from hours, minutes and seconds.
    /// \param hour Hour value.
    /// \param min  Minute value.
    /// \param sec  Second value.
    /// \return Second of the day.
    int sec_of_day(int hour, int min, int sec) {
       return hour * SEC_PER_HOUR + min * SEC_PER_MIN + sec;
    }

    /// \brief Convert an integer offset to a TimeZoneStruct.
    /// \param offset Offset in seconds.
    /// \return TimeZoneStruct represented by the offset.
    TimeZoneStruct to_time_zone(int offset) {
       return to_time_zone_struct(offset);
    }

    /// \brief Alias for to_time_zone.
    /// \copydoc to_time_zone
    TimeZoneStruct to_tz_struct(int offset) { return to_time_zone(offset); }

    /// \}

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_CONVERSIONS_MQH__
