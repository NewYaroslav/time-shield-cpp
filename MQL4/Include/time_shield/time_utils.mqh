//+------------------------------------------------------------------+
//|                                                   time_utils.mqh |
//|                                Time Shield - MQL4 Time Utilities |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_MQL4_TIME_UTILS_MQH__
#define __TIME_SHIELD_MQL4_TIME_UTILS_MQH__

/// \file time_utils.mqh
/// \brief Header with MQL4 time-related utility functions.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include <time_shield/constants.mqh>

namespace time_shield {

    long tick_count_ms() {
        return (long)GetTickCount();
    }

    long monotonic_ms() {
        static bool initialized = false;
        static ulong last_raw = 0;
        static long high = 0;

        const ulong raw = (ulong)GetTickCount();
        if(!initialized) {
            initialized = true;
            last_raw = raw;
            return (long)raw;
        }

        if(raw < last_raw) {
            high += 4294967296;
        }
        last_raw = raw;

        return high + (long)raw;
    }

    long ts() {
        return (long)TimeGMT();
    }

    long timestamp() {
        return ts();
    }

    long ts_ms() {
        static bool initialized = false;
        static long offset = 0;

        if(!initialized) {
            const long start_ts = ts();
            long next_ts = start_ts;
            while((next_ts = ts()) == start_ts) {
            }

            offset = next_ts * MS_PER_SEC - monotonic_ms();
            initialized = true;
        }

        return monotonic_ms() + offset;
    }

    long timestamp_ms() {
        return ts_ms();
    }

    long now() {
        return ts_ms();
    }

} // namespace time_shield

#endif // __TIME_SHIELD_MQL4_TIME_UTILS_MQH__
