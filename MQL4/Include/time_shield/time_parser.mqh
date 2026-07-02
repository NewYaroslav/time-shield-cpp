//+------------------------------------------------------------------+
//|                                                  time_parser.mqh |
//|                                   Time Shield - MQL4 Time Parser |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_MQL4_TIME_PARSER_MQH__
#define __TIME_SHIELD_MQL4_TIME_PARSER_MQH__

/// \file time_parser.mqh
/// \brief Header with MQL4 time parsing helpers.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include <time_shield/constants.mqh>
#include <time_shield/time_conversions.mqh>

namespace time_shield {

    bool is_valid_time(const int hour, const int minute, const int second) {
        return hour >= 0 && hour < 24
            && minute >= 0 && minute < 60
            && second >= 0 && second < 60;
    }

    int sec_of_day(const string value) {
        string parts[];
        const ushort separator = StringGetCharacter(":", 0);
        const int count = StringSplit(value, separator, parts);

        if(count < 1 || count > 3) {
            ArrayFree(parts);
            return (int)SEC_PER_DAY;
        }

        int hour = 0;
        int minute = 0;
        int second = 0;

        hour = (int)StringToInteger(parts[0]);
        if(count > 1) {
            minute = (int)StringToInteger(parts[1]);
        }
        if(count > 2) {
            second = (int)StringToInteger(parts[2]);
        }

        ArrayFree(parts);

        if(!is_valid_time(hour, minute, second)) {
            return (int)SEC_PER_DAY;
        }

        return sec_of_day(hour, minute, second);
    }

} // namespace time_shield

#endif // __TIME_SHIELD_MQL4_TIME_PARSER_MQH__
