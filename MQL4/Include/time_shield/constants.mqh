//+------------------------------------------------------------------+
//|                                                    constants.mqh |
//|                                     Time Shield - MQL4 Constants |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_MQL4_CONSTANTS_MQH__
#define __TIME_SHIELD_MQL4_CONSTANTS_MQH__

/// \file constants.mqh
/// \brief Header file with MQL4 time-related constants.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

namespace time_shield {

    const long MS_PER_SEC  = 1000;     ///< Milliseconds per second.
    const long US_PER_SEC  = 1000000;  ///< Microseconds per second.
    const long SEC_PER_MIN = 60;       ///< Seconds per minute.
    const long SEC_PER_HOUR = 3600;    ///< Seconds per hour.
    const long SEC_PER_DAY = 86400;    ///< Seconds per day.

} // namespace time_shield

#endif // __TIME_SHIELD_MQL4_CONSTANTS_MQH__
