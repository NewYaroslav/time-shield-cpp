//+------------------------------------------------------------------+
//|                                             time_conversions.mqh |
//|                           Time Shield - MQL4 Time Conversions    |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_MQL4_TIME_CONVERSIONS_MQH__
#define __TIME_SHIELD_MQL4_TIME_CONVERSIONS_MQH__

/// \file time_conversions.mqh
/// \brief Header with MQL4 time conversion helpers.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include <time_shield/constants.mqh>

namespace time_shield {

    int sec_of_day(const long timestamp) {
        return (int)(timestamp % SEC_PER_DAY);
    }

    int sec_of_day_ms(const long timestamp_ms) {
        return sec_of_day(timestamp_ms / MS_PER_SEC);
    }

    int sec_of_day(const int hour, const int minute, const int second) {
        return hour * (int)SEC_PER_HOUR + minute * (int)SEC_PER_MIN + second;
    }

} // namespace time_shield

#endif // __TIME_SHIELD_MQL4_TIME_CONVERSIONS_MQH__
