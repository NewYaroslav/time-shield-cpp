//+------------------------------------------------------------------+
//|                                            test_eet_to_gmt.mq5 |
//|                      Time Shield - Function Test                |
//|                      Copyright 2025, NewYaroslav                |
//|              https://github.com/NewYaroslav/time-shield-cpp     |
//+------------------------------------------------------------------+
#property script_show_inputs
#property strict

#include <TimeShield.mqh>

using namespace time_shield;

void OnStart()
{
    // Winter timestamp: 2024-01-15 12:00:00 EET
    datetime winter_eet = to_ts(2024, JAN, 15, 12, 0, 0);
    datetime winter_expected = to_ts(2024, JAN, 15, 10, 0, 0);
    datetime winter_gmt = eet_to_gmt(winter_eet);

    if(winter_gmt == winter_expected)
        Print("Winter conversion passed");
    else
        Print("Winter conversion failed: ", winter_gmt, " != ", winter_expected);

    // Summer timestamp: 2024-07-15 12:00:00 EET (EEST)
    datetime summer_eet = to_ts(2024, JUL, 15, 12, 0, 0);
    datetime summer_expected = to_ts(2024, JUL, 15, 9, 0, 0);
    datetime summer_gmt = eet_to_gmt(summer_eet);

    if(summer_gmt == summer_expected)
        Print("Summer conversion passed");
    else
        Print("Summer conversion failed: ", summer_gmt, " != ", summer_expected);
}

