//+------------------------------------------------------------------+
//|                                                   validation.mqh |
//|                          Time Shield - MQL5 Validation Functions |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_VALIDATION_MQH__
#define __TIME_SHIELD_VALIDATION_MQH__

/// \file validation.mqh
/// \ingroup mql5
/// \brief Header with validation functions for dates, times, and timestamps.
///
/// This file contains helper functions for validating dates, times,
/// time zones and timestamps in MQL5.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include "constants.mqh"
#include "enums.mqh"
#include "time_struct.mqh"
#include "date_struct.mqh"
#include "date_time_struct.mqh"
#include "time_zone_struct.mqh"

namespace time_shield {

    /// \defgroup time_validation Time Validation
    /// \brief Functions for validating dates, times and time zones.
    /// \{

    /// \brief Checks if the given year is a leap year.
    /// \param year Year to check.
    /// \return true if the year is a leap year, false otherwise.
    bool is_leap_year_date(const long year) {
       return ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0));
    }

    /// \brief Alias for is_leap_year_date.
    /// \copydoc is_leap_year_date
    bool check_leap_year(const long year) { return is_leap_year_date(year); }

    /// \brief Alias for is_leap_year_date.
    /// \copydoc is_leap_year_date
    bool leap_year(const long year) { return is_leap_year_date(year); }

    //----------------------------------------------------------------------

    /// \brief Checks if the year of the given timestamp is a leap year.
    /// \param ts Timestamp in seconds since the Unix epoch.
    /// \return true if the year is a leap year, false otherwise.
    bool is_leap_year_ts(const long ts) {
       const long BIAS_292277022000 = 9223371890843040000LL;
       const long BIAS_2000         = 946684800LL;

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
       return is_leap_year_date(y);
    }

    /// \brief Alias for is_leap_year_ts.
    /// \copydoc is_leap_year_ts
    bool leap_year_ts(const long ts) { return is_leap_year_ts(ts); }

    /// \brief Alias for is_leap_year_ts.
    /// \copydoc is_leap_year_ts
    bool check_leap_year_ts(const long ts) { return is_leap_year_ts(ts); }

    /// \brief Alias for is_leap_year_ts.
    /// \copydoc is_leap_year_ts
    bool is_leap_year(const long ts) { return is_leap_year_ts(ts); }

    //----------------------------------------------------------------------

    /// \brief Check if the time zone components are valid.
    /// \param hour The hour component of the time zone.
    /// \param min  The minute component of the time zone.
    /// \return true if the time zone is valid, false otherwise.
    bool is_valid_time_zone(const int hour, const int min) {
       if (hour < 0 || hour > 23) return false;
       if (min < 0 || min > 59) return false;
       return true;
    }

    /// \brief Alias for is_valid_time_zone.
    /// \copydoc is_valid_time_zone
    bool is_valid_tz(const int hour, const int min) {
       return is_valid_time_zone(hour, min);
    }

    /// \brief Check if the time zone structure is valid.
    /// \param time_zone Structure containing hour and minute components.
    /// \return true if the time zone is valid, false otherwise.
    bool is_valid_time_zone_offset(const TimeZoneStruct &time_zone) {
       return is_valid_time_zone(time_zone.hour, time_zone.min);
    }

    /// \brief Alias for is_valid_time_zone_offset.
    /// \copydoc is_valid_time_zone_offset
    bool is_valid_time_zone(const TimeZoneStruct &time_zone) {
       return is_valid_time_zone_offset(time_zone);
    }

    /// \brief Alias for is_valid_time_zone_offset.
    /// \copydoc is_valid_time_zone_offset
    bool is_valid_tz(const TimeZoneStruct &time_zone) {
       return is_valid_time_zone_offset(time_zone);
    }

    //----------------------------------------------------------------------

    /// \brief Checks the correctness of the specified time.
    /// \param hour Hour component.
    /// \param min  Minute component.
    /// \param sec  Second component.
    /// \param ms   Millisecond component (default is 0).
    /// \return true if the time is valid, false otherwise.
    bool is_valid_time(const int hour, const int min, const int sec, const int ms = 0) {
       if (hour < 0 || hour > 23) return false;
       if (min  < 0 || min  > 59) return false;
       if (sec  < 0 || sec  > 59) return false;
       if (ms   < 0 || ms   > 999) return false;
       return true;
    }

    /// \brief Checks the correctness of the specified time structure.
    /// \param time Time structure.
    /// \return true if the time is valid, false otherwise.
    bool is_valid_time(const TimeStruct &time) {
       return is_valid_time(time.hour, time.min, time.sec, time.ms);
    }

    /// \brief Checks the correctness of the specified date.
    /// \param year  Year component.
    /// \param month Month component.
    /// \param day   Day component.
    /// \return true if the date is valid, false otherwise.
    bool is_valid_date(const long year, const int month, const int day) {
       if (day > 31 && year <= 31)
          return is_valid_date((long)day, month, (int)year);
       if (year > MAX_YEAR) return false;
       if (month < 1 || month > 12) return false;
       if (day < 1 || day > 31) return false;
       if (month == FEB) {
          bool leap = is_leap_year_date(year);
          if (leap && day > 29) return false;
          if (!leap && day > 28) return false;
       } else {
          switch(month) {
          case 4:
          case 6:
          case 9:
          case 11:
             if (day > 30) return false;
             break;
          default:
             break;
          }
       }
       return true;
    }

    /// \brief Checks the correctness of the specified date structure.
    /// \param date Date structure.
    /// \return true if the date is valid, false otherwise.
    bool is_valid_date(const DateStruct &date) {
       return is_valid_date(date.year, date.mon, date.day);
    }

    /// \brief Checks the correctness of date and time components.
    /// \param year Year component.
    /// \param month Month component.
    /// \param day Day component.
    /// \param hour Hour component (default is 0).
    /// \param min  Minute component (default is 0).
    /// \param sec  Second component (default is 0).
    /// \param ms   Millisecond component (default is 0).
    /// \return true if the date and time are valid, false otherwise.
    bool is_valid_date_time(
            const long year,
            const int  month,
            const int  day,
            const int  hour = 0,
            const int  min  = 0,
            const int  sec  = 0,
            const int  ms   = 0) {
       if (!is_valid_date(year, month, day)) return false;
       if (!is_valid_time(hour, min, sec, ms)) return false;
       return true;
    }

    /// \brief Checks the correctness of the date-time structure.
    /// \param date_time Date-time structure.
    /// \return true if the date and time are valid, false otherwise.
    bool is_valid_date_time(const DateTimeStruct &date_time) {
       if (!is_valid_date(date_time.year, date_time.mon, date_time.day)) return false;
       if (!is_valid_time(date_time.hour, date_time.min, date_time.sec, date_time.ms)) return false;
       return true;
    }

    //----------------------------------------------------------------------

    /// \brief Check if a timestamp corresponds to a weekend day.
    /// \param ts Timestamp to check.
    /// \return true if the day is a weekend day, false otherwise.
    bool is_day_off(const long ts) {
       int wd = (int)((ts / SEC_PER_DAY + THU) % DAYS_PER_WEEK);
       return (wd == SUN || wd == SAT);
    }

    /// \brief Alias for is_day_off.
    /// \copydoc is_day_off
    bool is_weekend(const long ts) { return is_day_off(ts); }

    /// \brief Check if a Unix day corresponds to a weekend day.
    /// \param unix_day Number of days since Unix epoch.
    /// \return true if the day is a weekend day, false otherwise.
    bool is_day_off_unix_day(const long unix_day) {
       int wd = (int)((unix_day + THU) % DAYS_PER_WEEK);
       return (wd == SUN || wd == SAT);
    }

    /// \brief Alias for is_day_off_unix_day.
    /// \copydoc is_day_off_unix_day
    bool is_weekend_unix_day(const long unix_day) {
       return is_day_off_unix_day(unix_day);
    }

    /// \}

}; // namespace time_shield

#endif // __TIME_SHIELD_VALIDATION_MQH__
