//+------------------------------------------------------------------+
//|                                     time_formatting_demo.mq5 |
//|                   Time Shield - Formatting Examples           |
//|                      Copyright 2025, NewYaroslav                |
//|              https://github.com/NewYaroslav/time-shield-cpp     |
//+------------------------------------------------------------------+
#property script_show_inputs
#property strict

#include <TimeShield.mqh>

using namespace time_shield;

void OnStart()
{
    datetime ts = to_ts(2024, JUN, 21, 15, 30, 5);
    long ts_ms = to_ts_ms(2024, JUN, 21, 15, 30, 5, 123);

    Print("Custom: ", to_string("%Y-%m-%d %H:%M:%S", ts));
    Print("ISO8601: ", to_iso8601(ts));
    Print("ISO8601 UTC ms: ", to_iso8601_utc_ms(ts_ms));
    Print("ISO8601 +02: ", to_iso8601(ts, 2*3600));
    Print("MQL5: ", to_mql5_date_time(ts));
    Print("Windows filename: ", to_windows_filename(ts));
    Print("Human readable: ", to_human_readable_ms(ts_ms));
}
