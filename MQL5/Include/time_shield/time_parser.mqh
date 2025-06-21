//+------------------------------------------------------------------+
//|                                            time_parser.mqh       |
//|                        Time Shield - MQL5 Time Parser            |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_PARSER_MQH__
#define __TIME_SHIELD_TIME_PARSER_MQH__

/// \file time_parser.mqh
/// \ingroup mql5
/// \brief Header with functions for parsing ISO8601 strings
/// and converting them to timestamps.
///
/// This file contains utilities to parse ISO8601 date and time strings,
/// extract month numbers and convert parsed values into different
/// timestamp representations.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include <time_shield/enums.mqh>
#include <time_shield/constants.mqh>
#include <time_shield/date_time_struct.mqh>
#include <time_shield/time_zone_struct.mqh>
#include <time_shield/validation.mqh>
#include <time_shield/time_conversions.mqh>

namespace time_shield {

    /// \defgroup time_parsing Time Parsing
    /// \brief A comprehensive set of functions for parsing and converting date and time strings.
    /// \{

    /// \brief Get the month number by name.
    /// \param month The name of the month as a string.
    /// \return The month number corresponding to the given name, or 0 on error.
    int get_month_number(const string &month)
    {
       if(StringLen(month)==0)
          return 0;
       string month_copy = month;
       string tmp = "";
       StringConcatenate(tmp,
                        StringUpper(StringSubstr(month_copy,0,1)),
                        StringLower(StringSubstr(month_copy,1)));
       month_copy = tmp;
       static const string short_names[] = {"Jan","Feb","Mar","Apr","May","Jun",
                                            "Jul","Aug","Sep","Oct","Nov","Dec"};
       static const string full_names[]  = {"January","February","March","April","May","June",
                                            "July","August","September","October","November","December"};
       for(int i=0;i<MONTHS_PER_YEAR;++i)
       {
          if(month_copy==short_names[i] || month_copy==full_names[i])
             return i+1;
       }
       return 0;
    }

    /// \brief Alias for get_month_number function.
    /// \copydoc get_month_number
    int month_of_year(const string &month) { return get_month_number(month); }

//------------------------------------------------------------------------------

    /// \brief Get the month number by name, with output parameter.
    /// \param month The name of the month as a string.
    /// \param value Reference to store the month number if found.
    /// \return True if the month name is valid, false otherwise.
    bool try_get_month_number(const string &month, int &value)
    {
       int res = get_month_number(month);
       if(res==0)
          return false;
       value = res;
       return true;
    }

    /// \brief Alias for try_get_month_number function.
    /// \copydoc try_get_month_number
    bool get_month_number(const string &month, int &value) { return try_get_month_number(month,value); }

    /// \brief Alias for try_get_month_number function.
    /// \copydoc try_get_month_number
    bool month_of_year(const string &month, int &value) { return try_get_month_number(month,value); }

//------------------------------------------------------------------------------

    /// \brief Parse a time zone string into a TimeZoneStruct.
    /// \details Parses a time zone string in the format "+hh:mm", "-hh:mm" or "Z".
    /// If the string is empty or "Z", UTC is assumed.
    /// \param tz_str The time zone string.
    /// \param tz The TimeZoneStruct to be filled.
    /// \return True if the parsing is successful and the time zone is valid, false otherwise.
    bool parse_time_zone(const string &tz_str, TimeZoneStruct &tz)
    {
       if(StringLen(tz_str)==0 || tz_str=="Z")
       {
          tz.hour=0; tz.min=0; tz.is_positive=true; return true;
       }
       tz.is_positive = (StringGetChar(tz_str,0)=='+');
       tz.hour = (int)StringToInteger(StringSubstr(tz_str,1,2));
       tz.min  = (int)StringToInteger(StringSubstr(tz_str,4,2));
       return is_valid_time_zone(tz);
    }

    /// \brief Alias for parse_time_zone function.
    /// \copydoc parse_time_zone
    bool parse_tz(const string &tz_str, TimeZoneStruct &tz) { return parse_time_zone(tz_str, tz); }

//------------------------------------------------------------------------------

    /// \brief Parse a date and time string in ISO8601 format.
    /// \param input The input string in ISO8601 format.
    /// \param dt The DateTimeStruct to be filled with parsed values.
    /// \param tz The TimeZoneStruct to be filled with parsed time zone.
    /// \return True if parsing succeeds and the date and time values are valid, false otherwise.
    bool parse_iso8601(const string &input, DateTimeStruct &dt, TimeZoneStruct &tz)
    {
       dt = create_date_time_struct(0);
       tz = create_time_zone_struct(0,0);

       string date_part=input;
       string time_part="";
       int posT=StringFind(input,"T");
       if(posT<0) posT=StringFind(input," ");
       if(posT>=0)
       {
          date_part=StringSubstr(input,0,posT);
          time_part=StringSubstr(input,posT+1);
       }

       string parts[];
       int cnt=StringSplit(date_part,'-',parts);
       if(cnt<3)
       {
          cnt=StringSplit(date_part,'/',parts);
          if(cnt<3)
             cnt=StringSplit(date_part,'.',parts);
       }
       if(cnt<3)
          return false;
       dt.year=(long)StringToInteger(parts[0]);
       dt.mon =(int)StringToInteger(parts[1]);
       dt.day =(int)StringToInteger(parts[2]);

       if(StringLen(time_part)>0)
       {
          string tz_str="";
          int zpos=StringFind(time_part,"Z");
          int ppos=StringFind(time_part,"+");
          int npos=StringFind(time_part,"-");
          int tzpos=-1;
          if(zpos>=0){ tzpos=zpos; tz_str=StringSubstr(time_part,zpos); }
          else if(ppos>=0){ tzpos=ppos; tz_str=StringSubstr(time_part,ppos); }
          else if(npos>0){ tzpos=npos; tz_str=StringSubstr(time_part,npos); }
          if(tzpos>=0)
             time_part=StringSubstr(time_part,0,tzpos);

          string tparts[];
          int tcnt=StringSplit(time_part,':',tparts);
          if(tcnt<2)
             return is_valid_date_time(dt);
          dt.hour=(int)StringToInteger(tparts[0]);
          dt.min =(int)StringToInteger(tparts[1]);
          if(tcnt>=3)
          {
             string sec_part=tparts[2];
             int dot=StringFind(sec_part,'.');
             if(dot>=0)
             {
                dt.sec=(int)StringToInteger(StringSubstr(sec_part,0,dot));
                dt.ms =(int)StringToInteger(StringSubstr(sec_part,dot+1));
             }
             else
                dt.sec=(int)StringToInteger(sec_part);
          }
          if(StringLen(tz_str)>0)
             if(!parse_time_zone(tz_str,tz)) return false;
       }
       return is_valid_date_time(dt);
    }

    /// \brief Convert an ISO8601 string to a timestamp (ts_t).
    /// \param str The ISO8601 string.
    /// \param ts The timestamp to be filled.
    /// \return True if the parsing and conversion are successful, false otherwise.
    bool str_to_ts(const string &str, long &ts)
    {
       DateTimeStruct dt; TimeZoneStruct tz;
       if(!parse_iso8601(str, dt, tz)) return false;
       ts = to_timestamp(dt) + to_offset(tz);
       return true;
    }

    /// \brief Convert an ISO8601 string to a millisecond timestamp (ts_ms_t).
    /// \param str The ISO8601 string.
    /// \param ts The millisecond timestamp to be filled.
    /// \return True if the parsing and conversion are successful, false otherwise.
    bool str_to_ts_ms(const string &str, long &ts)
    {
       DateTimeStruct dt; TimeZoneStruct tz;
       if(!parse_iso8601(str, dt, tz)) return false;
       ts = to_timestamp_ms(dt) + sec_to_ms(to_offset(tz));
       return true;
    }

    /// \brief Convert an ISO8601 string to a floating-point timestamp (fts_t).
    /// \param str The ISO8601 string.
    /// \param ts The floating-point timestamp to be filled.
    /// \return True if the parsing and conversion are successful, false otherwise.
    bool str_to_fts(const string &str, double &ts)
    {
       DateTimeStruct dt; TimeZoneStruct tz;
       if(!parse_iso8601(str, dt, tz)) return false;
       ts = to_ftimestamp(dt) + (double)to_offset(tz);
       return true;
    }

    /// \brief Convert an ISO8601 string to a timestamp (ts_t).
    /// \details Returns 0 if parsing fails.
    /// \param str The ISO8601 string.
    /// \return Timestamp value or 0 on error.
    long ts(const string &str)
    {
       long v=0;
       str_to_ts(str,v);
       return v;
    }

    /// \brief Convert an ISO8601 string to a millisecond timestamp (ts_ms_t).
    /// \details Returns 0 if parsing fails.
    /// \param str The ISO8601 string.
    /// \return Millisecond timestamp value or 0 on error.
    long ts_ms(const string &str)
    {
       long v=0;
       str_to_ts_ms(str,v);
       return v;
    }

    /// \brief Convert an ISO8601 string to a floating-point timestamp (fts_t).
    /// \details Returns 0.0 if parsing fails.
    /// \param str The ISO8601 string.
    /// \return Floating-point timestamp or 0.0 on error.
    double fts(const string &str)
    {
       double v=0.0;
       str_to_fts(str,v);
       return v;
    }

    /// \}

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_PARSER_MQH__
