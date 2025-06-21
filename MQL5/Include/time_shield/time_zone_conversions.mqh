//+------------------------------------------------------------------+
//|                                        time_zone_conversions.mqh |
//|                      Time Shield - MQL5 Time Zone Conversions    |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_ZONE_CONVERSIONS_MQH__
#define __TIME_SHIELD_TIME_ZONE_CONVERSIONS_MQH__

/// \file time_zone_conversions.mqh
/// \ingroup mql5
/// \brief Helpers for converting CET and EET timestamps to GMT.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include <time_shield/time_conversions.mqh>

namespace time_shield {

    /// \ingroup time_conversions
    /// \brief Convert Central European Time to Greenwich Mean Time.
    /// \param cet Timestamp in seconds in CET.
    /// \return Timestamp in seconds in GMT.
    datetime cet_to_gmt(const datetime cet) {
       DateTimeStruct dt = to_date_time(cet);
       int max_days = num_days_in_month(dt.year, dt.mon);
       const int OLD_START_SUMMER_HOUR = 2;
       const int OLD_STOP_SUMMER_HOUR  = 3;
       const int NEW_SUMMER_HOUR       = 1;

       if(dt.year < 2002) {
          if(dt.mon > MAR && dt.mon < OCT)
             return cet - SEC_PER_HOUR * 2;
          else
          if(dt.mon == MAR) {
             for(int d = max_days; d >= dt.day; d--) {
                if(day_of_week_date(dt.year, MAR, d) == SUN) {
                   if(d == dt.day) {
                      if(dt.hour >= OLD_START_SUMMER_HOUR)
                         return cet - SEC_PER_HOUR * 2;
                      return cet - SEC_PER_HOUR;
                   }
                   return cet - SEC_PER_HOUR;
                }
             }
             return cet - SEC_PER_HOUR * 2;
          } else
          if(dt.mon == OCT) {
             for(int d = max_days; d >= dt.day; d--) {
                if(day_of_week_date(dt.year, OCT, d) == SUN) {
                   if(d == dt.day) {
                      if(dt.hour >= OLD_STOP_SUMMER_HOUR)
                         return cet - SEC_PER_HOUR;
                      return cet - SEC_PER_HOUR;
                   }
                   return cet - SEC_PER_HOUR * 2;
                }
             }
             return cet - SEC_PER_HOUR;
          }
          return cet - SEC_PER_HOUR;
       } else {
          if(dt.mon > MAR && dt.mon < OCT)
             return cet - SEC_PER_HOUR * 2;
          else
          if(dt.mon == MAR) {
             for(int d = max_days; d >= dt.day; d--) {
                if(day_of_week_date(dt.year, MAR, d) == SUN) {
                   if(d == dt.day) {
                      if(dt.hour >= (NEW_SUMMER_HOUR + 2))
                         return cet - SEC_PER_HOUR * 2;
                      return cet - SEC_PER_HOUR;
                   }
                   return cet - SEC_PER_HOUR;
                }
             }
             return cet - SEC_PER_HOUR * 2;
          } else
          if(dt.mon == OCT) {
             for(int d = max_days; d >= dt.day; d--) {
                if(day_of_week_date(dt.year, OCT, d) == SUN) {
                   if(d == dt.day) {
                      if(dt.hour >= (NEW_SUMMER_HOUR + 1))
                         return cet - SEC_PER_HOUR;
                      return cet - SEC_PER_HOUR * 2;
                   }
                   return cet - SEC_PER_HOUR * 2;
                }
             }
             return cet - SEC_PER_HOUR;
          }
          return cet - SEC_PER_HOUR;
       }
       return cet - SEC_PER_HOUR;
    }

    /// \ingroup time_conversions
    /// \brief Convert Eastern European Time to Greenwich Mean Time.
    /// \param eet Timestamp in seconds in EET.
    /// \return Timestamp in seconds in GMT.
    datetime eet_to_gmt(const datetime eet) {
       return cet_to_gmt(eet - SEC_PER_HOUR);
    }

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_ZONE_CONVERSIONS_MQH__
