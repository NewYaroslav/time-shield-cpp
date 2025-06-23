//+------------------------------------------------------------------+
//|                                              time_formatting.mqh |
//|                               Time Shield - MQL5 Time Formatting |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_FORMATTING_MQH__
#define __TIME_SHIELD_TIME_FORMATTING_MQH__

/// \file time_formatting.mqh
/// \ingroup mql5
/// \brief Header with helper functions for formatting timestamps and
/// date-time values in MQL5.
///
/// This file provides functions for converting timestamps to various
/// string representations including ISO8601 and MQL5 specific formats.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include "enums.mqh"
#include "date_time_struct.mqh"
#include "time_zone_struct.mqh"
#include "time_conversions.mqh"


namespace time_shield {

    /// \ingroup time_formatting
    /// \{

    /// \brief Internal helper to pad integer with leading zeros.
    /// \param value Integer value to pad.
    /// \param width Desired width of the number.
    /// \return String with left-padded digits.
    string pad_int(int value, int width) {
       string s = IntegerToString(value);
       while(StringLen(s) < width)
          s = "0" + s;
       return s;
    }

    //----------------------------------------------------------------------
    // Custom formatting
    //----------------------------------------------------------------------

   /// \brief Internal helper for custom formatting processing.
   ///
   /// This function handles individual format specifiers used by
   /// \ref to_string and \ref to_string_ms.
   ///
   /// \param last_char     Current format character.
   /// \param repeat_count  Number of repeats of the format character.
   /// \param ts            Timestamp in seconds.
   /// \param utc_offset    UTC offset in seconds.
   /// \param dt            DateTimeStruct corresponding to the timestamp.
   /// \param result        Output string being built.
   void process_format_impl(
           const char last_char,
           int repeat_count,
           long ts,
           int utc_offset,
           const DateTimeStruct &dt,
           string &result) {
       switch(last_char) {
          case 'a':
             if(repeat_count<=1)
                result += time_shield::to_str(day_of_week(dt.year, dt.mon, dt.day), time_shield::FormatType::SHORT_NAME);
             break;
          case 'A':
             if(repeat_count<=1)
                result += time_shield::to_str(day_of_week(dt.year, dt.mon, dt.day), time_shield::FormatType::FULL_NAME);
             break;
          case 'I':
             if(repeat_count==1)
                result += pad_int(hour24_to_12(dt.hour),2);
             break;
          case 'H':
             if(repeat_count<=2)
                result += pad_int(dt.hour,2);
             break;
          case 'h':
             if(repeat_count==2)
                result += pad_int(dt.hour,2);
             else
                result += to_str((Month)dt.mon, SHORT_NAME);
             break;
          case 'b':
             if(repeat_count<=1)
                result += to_str((Month)dt.mon, SHORT_NAME);
             break;
          case 'B':
             if(repeat_count<=1)
                result += to_str((Month)dt.mon, FULL_NAME);
             break;
          case 'c':
             if(repeat_count<=1) {
                result += to_str(day_of_week(dt.year, dt.mon, dt.day), SHORT_NAME)+" ";
                result += to_str((Month)dt.mon, SHORT_NAME)+" ";
                result += StringFormat("%2d ", dt.day);
                result += StringFormat("%02d:%02d:%02d ", dt.hour, dt.min, dt.sec);
                result += IntegerToString(dt.year);
             }
             break;
          case 'C':
             if(repeat_count<=1)
                result += IntegerToString(dt.year/100);
             break;
          case 'd':
             if(repeat_count<2)
                result += pad_int(dt.day,2);
             break;
          case 'D':
             if(repeat_count==1)
                result += StringFormat("%02d/%02d/%02d", dt.mon, dt.day, (int)(dt.year%100));
             else if(repeat_count==2)
                result += pad_int(dt.day,2);
             break;
          case 'e':
             if(repeat_count==1)
                result += StringFormat("%2d", dt.day);
             break;
          case 'F':
             if(repeat_count==1)
                result += StringFormat("%lld-%02d-%02d", dt.year, dt.mon, dt.day);
             break;
          case 'j':
             if(repeat_count==1)
                result += pad_int(day_of_year(ts),3);
             break;
          case 'k':
             if(repeat_count==1)
                result += StringFormat("%2d", dt.hour);
             break;
          case 'l':
             if(repeat_count==1)
                result += StringFormat("%2d", hour24_to_12(dt.hour));
             break;
          case 'm':
             if(repeat_count==1)
                result += pad_int(dt.mon,2);
             else if(repeat_count==2)
                result += pad_int(dt.min,2);
             break;
          case 'M':
             if(repeat_count==1)
                result += pad_int(dt.min,2);
             else if(repeat_count==2)
                result += pad_int(dt.mon,2);
             else if(repeat_count==3)
                result += to_str((Month)dt.mon, UPPERCASE_NAME);
             break;
          case 'n':
             result += "\n";
             break;
          case 'p':
             result += dt.hour<12?"AM":"PM";
             break;
          case 'P':
             result += dt.hour<12?"am":"pm";
             break;
          case 'r':
             if(repeat_count==1)
                result += StringFormat("%02d:%02d:%02d %s", hour24_to_12(dt.hour), dt.min, dt.sec, dt.hour<12?"AM":"PM");
             break;
          case 'R':
             if(repeat_count==1)
                result += StringFormat("%02d:%02d", dt.hour, dt.min);
             break;
          case 's':
             if(repeat_count==1)
                result += IntegerToString(ts);
             else if(repeat_count==3)
                result += IntegerToString(dt.ms);
             break;
          case 'S':
             if(repeat_count<=2)
                result += pad_int(dt.sec,2);
             if(repeat_count==3)
                result += IntegerToString(dt.ms);
             break;
          case 't':
             if(repeat_count<=1)
                result += "\t";
             break;
          case 'T':
             if(repeat_count==1)
                result += StringFormat("%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
             break;
          case 'u':
             if(repeat_count==1) {
                int dw = day_of_week(dt.year, dt.mon, dt.day);
                if(dw==0) dw=7;
                result += IntegerToString(dw);
             }
             break;
          case 'w':
             if(repeat_count==1)
                result += IntegerToString(day_of_week(dt.year, dt.mon, dt.day));
             else if(repeat_count==3)
                result += to_str(day_of_week(dt.year, dt.mon, dt.day), SHORT_NAME);
             break;
          case 'W':
             if(repeat_count==3)
                result += to_str(day_of_week(dt.year, dt.mon, dt.day), UPPERCASE_NAME);
             break;
          case 'y':
             if(repeat_count==1)
                result += IntegerToString(dt.year%100);
             break;
          case 'Y':
             if(repeat_count==1)
                result += IntegerToString(dt.year);
             break;
          case 'z':
             if(repeat_count==1) {
                TimeZoneStruct tz = to_time_zone_struct(utc_offset);
                string sign = tz.is_positive?"+":"-";
                result += sign+pad_int(tz.hour,2)+pad_int(tz.min,2);
             }
             break;
          case 'Z':
             result += "UTC";
             break;
       }
    }

   /// \brief Convert timestamp to string with custom format.
   ///
   /// This function works similarly to the C++ `strftime` implementation and
   /// supports most of its specifiers along with several custom ones.
   ///
   /// \param format_str Format string with specifiers, e.g. "%H:%M:%S".
   /// \param timestamp  Timestamp in seconds.
   /// \param utc_offset UTC offset in seconds (default 0).
   /// \return Formatted string according to \p format_str.
   string to_string(const string &format_str, long timestamp, int utc_offset=0) {
       string result="";
       if(StringLen(format_str)==0) return result;
       DateTimeStruct dt = to_date_time(timestamp);
       bool is_command=false;
       int repeat_count=0;
       char last_char = (char)format_str[0];
       if(last_char!='%') result+=CharToString(last_char);
       for(int i=0;i<StringLen(format_str);++i) {
          char current_char = (char)format_str[i];
          if(!is_command) {
             if(current_char=='%') {
                ++repeat_count;
                if(repeat_count==2) {
                   result += "%";
                   repeat_count=0;
                }
                continue;
             }
             if(!repeat_count) {
                result += CharToString(current_char);
                continue;
             }
             last_char=current_char;
             is_command=true;
             continue;
          }
          if(last_char==current_char) {
             ++repeat_count;
             continue;
          }
          process_format_impl(last_char, repeat_count, timestamp, utc_offset, dt, result);
          repeat_count=0;
          is_command=false;
          --i;
       }
       if(is_command)
          process_format_impl(last_char, repeat_count, timestamp, utc_offset, dt, result);
       return result;
    }

    /// \brief Alias for \ref to_string.
    /// \copydoc to_string
    string to_str(const string &format_str, long timestamp, int utc_offset=0) {
       return to_string(format_str, timestamp, utc_offset);
    }

    /// \brief Convert millisecond timestamp to string with custom format.
    ///
    /// Works the same as \ref to_string but accepts a timestamp in milliseconds.
    ///
    /// \param format_str Format string with specifiers.
    /// \param timestamp_ms Timestamp in milliseconds.
    /// \param utc_offset UTC offset in seconds (default 0).
    /// \return Formatted string according to \p format_str.
    string to_string_ms(const string &format_str, long timestamp_ms, int utc_offset=0) {
       string result="";
       if(StringLen(format_str)==0) return result;
       DateTimeStruct dt = to_date_time_ms(timestamp_ms);
       bool is_command=false;
       int repeat_count=0;
       char last_char = (char)format_str[0];
       if(last_char!='%') result+=CharToString(last_char);
       for(int i=0;i<StringLen(format_str);++i) {
          char current_char = (char)format_str[i];
          if(!is_command) {
             if(current_char=='%') {
                ++repeat_count;
                if(repeat_count==2) {
                   result += "%";
                   repeat_count=0;
                }
                continue;
             }
             if(!repeat_count) {
                result += CharToString(current_char);
                continue;
             }
             last_char=current_char;
             is_command=true;
             continue;
          }
          if(last_char==current_char) {
             ++repeat_count;
             continue;
          }
          process_format_impl(last_char, repeat_count, ms_to_sec(timestamp_ms), utc_offset, dt, result);
          repeat_count=0;
          is_command=false;
          --i;
       }
       if(is_command)
          process_format_impl(last_char, repeat_count, ms_to_sec(timestamp_ms), utc_offset, dt, result);
       return result;
    }

    /// \brief Alias for \ref to_string_ms.
    /// \copydoc to_string_ms
    string to_str_ms(const string &format_str, long timestamp_ms, int utc_offset=0) {
       return to_string_ms(format_str, timestamp_ms, utc_offset);
    }

    //----------------------------------------------------------------------
    // ISO8601 helpers
    //----------------------------------------------------------------------

    /// \brief Convert timestamp to ISO8601 string.
    /// \param ts Timestamp in seconds.
    /// \return ISO8601 formatted string (YYYY-MM-DDTHH:MM:SS).
    string to_iso8601(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%lld-%02d-%02dT%02d:%02d:%02d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
    }

    /// \brief Convert millisecond timestamp to ISO8601 string.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return ISO8601 formatted string with milliseconds.
    string to_iso8601_ms(long ts_ms) {
       DateTimeStruct dt = to_date_time_ms(ts_ms);
       return StringFormat("%lld-%02d-%02dT%02d:%02d:%02d.%03d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms);
    }

    /// \brief Convert timestamp to ISO8601 date string.
    /// \param ts Timestamp in seconds.
    /// \return Date in ISO8601 format (YYYY-MM-DD).
    string to_iso8601_date(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%lld-%02d-%02d", dt.year, dt.mon, dt.day);
    }

    /// \brief Convert timestamp to ISO8601 time string.
    /// \param ts Timestamp in seconds.
    /// \return Time in ISO8601 format (HH:MM:SS).
    string to_iso8601_time(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
    }

    /// \brief Convert millisecond timestamp to ISO8601 time string.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return Time in ISO8601 format with milliseconds.
    string to_iso8601_time_ms(long ts_ms) {
       DateTimeStruct dt = to_date_time_ms(ts_ms);
       return StringFormat("%02d:%02d:%02d.%03d", dt.hour, dt.min, dt.sec, dt.ms);
    }

    /// \brief Convert timestamp to ISO8601 string in UTC.
    /// \param ts Timestamp in seconds.
    /// \return ISO8601 string with trailing 'Z'.
    string to_iso8601_utc(long ts) {
       return to_iso8601(ts)+"Z";
    }

    /// \brief Convert millisecond timestamp to ISO8601 string in UTC.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return ISO8601 string with milliseconds and trailing 'Z'.
    string to_iso8601_utc_ms(long ts_ms) {
       return to_iso8601_ms(ts_ms)+"Z";
    }

    /// \brief Convert timestamp to ISO8601 string with timezone offset.
    /// \param ts Timestamp in seconds.
    /// \param utc_offset Offset from UTC in seconds.
    /// \return ISO8601 string with timezone offset.
    string to_iso8601(long ts, int utc_offset) {
       TimeZoneStruct tz = to_time_zone(utc_offset);
       string sign = tz.is_positive?"+":"-";
       return StringFormat("%s%s%.2d:%.2d", to_iso8601(ts), sign, tz.hour, tz.min);
    }

    /// \brief Convert millisecond timestamp to ISO8601 string with timezone offset.
    /// \param ts_ms Timestamp in milliseconds.
    /// \param utc_offset Offset from UTC in seconds.
    /// \return ISO8601 string with milliseconds and timezone offset.
    string to_iso8601_ms(long ts_ms, int utc_offset) {
       TimeZoneStruct tz = to_time_zone(utc_offset);
       string sign = tz.is_positive?"+":"-";
       return StringFormat("%s%s%.2d:%.2d", to_iso8601_ms(ts_ms), sign, tz.hour, tz.min);
    }

    //----------------------------------------------------------------------
    // MQL5 specific helpers
    //----------------------------------------------------------------------

    /// \brief Convert timestamp to MQL5 date-time string.
    /// \param ts Timestamp in seconds.
    /// \return String in format "YYYY.MM.DD HH:MM:SS".
    string to_mql5_date_time(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%lld.%02d.%02d %02d:%02d:%02d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
    }

    /// \brief Convert timestamp to MQL5 date string.
    /// \param ts Timestamp in seconds.
    /// \return String in format "YYYY.MM.DD".
    string to_mql5_date(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%lld.%02d.%02d", dt.year, dt.mon, dt.day);
    }

    /// \brief Convert timestamp to MQL5 time string.
    /// \param ts Timestamp in seconds.
    /// \return String in format "HH:MM:SS".
    string to_mql5_time(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
    }

    /// \brief Alias for \ref to_mql5_date_time.
    /// \copydoc to_mql5_date_time
    string to_mql5_full(long ts) { return to_mql5_date_time(ts); }

    /// \brief Convert timestamp to Windows-compatible filename.
    /// \param ts Timestamp in seconds.
    /// \return String in format "YYYY-MM-DD_HH-MM-SS".
    string to_windows_filename(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%lld-%02d-%02d_%02d-%02d-%02d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
    }

    /// \brief Convert millisecond timestamp to Windows-compatible filename.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return String in format "YYYY-MM-DD_HH-MM-SS-SSS".
    string to_windows_filename_ms(long ts_ms) {
       DateTimeStruct dt = to_date_time_ms(ts_ms);
       return StringFormat("%lld-%02d-%02d_%02d-%02d-%02d-%03d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms);
    }

    /// \brief Convert timestamp to human-readable string.
    /// \param ts Timestamp in seconds.
    /// \return String in format "YYYY-MM-DD HH:MM:SS".
    string to_human_readable(long ts) {
       DateTimeStruct dt = to_date_time(ts);
       return StringFormat("%lld-%02d-%02d %02d:%02d:%02d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
    }

    /// \brief Convert millisecond timestamp to human-readable string.
    /// \param ts_ms Timestamp in milliseconds.
    /// \return String in format "YYYY-MM-DD HH:MM:SS.SSS".
    string to_human_readable_ms(long ts_ms) {
       DateTimeStruct dt = to_date_time_ms(ts_ms);
       return StringFormat("%lld-%02d-%02d %02d:%02d:%02d.%03d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms);
    }

    /// \}

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_FORMATTING_MQH__
