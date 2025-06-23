//+------------------------------------------------------------------+
//|                                     time_formatting_demo.mq5 |
//|                   Time Shield - Formatting Examples           |
//|                      Copyright 2025, NewYaroslav                |
//|              https://github.com/NewYaroslav/time-shield-cpp     |
//+------------------------------------------------------------------+
#property script_show_inputs
#property strict

#include <TimeShield.mqh>

void OnStart() {
    time_shield::init();

    datetime ts = (datetime)time_shield::to_ts(2024, time_shield::JUN, 21, 15, 30, 5);
    long ts_ms = time_shield::to_ts_ms(2024, time_shield::JUN, 21, 15, 30, 5, 123);

    Print("Custom: ", time_shield::to_string("%Y-%m-%d %H:%M:%S", ts));
    Print("ISO8601: ", time_shield::to_iso8601(ts));
    Print("ISO8601 UTC ms: ", time_shield::to_iso8601_utc_ms(ts_ms));
    Print("ISO8601 +02: ", time_shield::to_iso8601(ts, 2*3600));
    Print("MQL5: ", time_shield::to_mql5_date_time(ts));
    Print("Windows filename: ", time_shield::to_windows_filename(ts));
    Print("Human readable: ", time_shield::to_human_readable_ms(ts_ms));
}
