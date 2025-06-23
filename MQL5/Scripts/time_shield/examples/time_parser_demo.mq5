//+------------------------------------------------------------------+
//|                                     time_parser_demo.mq5 |
//|                   Time Shield - Parsing Examples         |
//|                      Copyright 2025, NewYaroslav          |
//|              https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#property script_show_inputs
#property strict

#include <TimeShield.mqh>

void OnStart() {
    time_shield::init();

    string iso = "2024-11-25T14:30:00+01:00";

    datetime ts;
    if (time_shield::str_to_ts(iso, ts))
        Print("ts: ", ts, " -> ", time_shield::to_iso8601(ts));
    else
        Print("Failed to parse: ", iso);

    long ts_ms;
    if (time_shield::str_to_ts_ms(iso, ts_ms))
        Print("ts_ms: ", ts_ms);

    double fts_val;
    if (time_shield::str_to_fts(iso, fts_val))
        Print("fts: ", DoubleToString(fts_val, 2));

    time_shield::DateTimeStruct dt;
    time_shield::TimeZoneStruct tz;
    if (time_shield::parse_iso8601(iso, dt, tz)) {
        string sign = tz.is_positive ? "+" : "-";
        Print("dt: ", dt.year, "-", dt.mon, "-", dt.day, " ", dt.hour, ":", dt.min, ":", dt.sec, ".", dt.ms,
              " TZ ", sign, tz.hour, ":", tz.min);
    }

    int mon = time_shield::get_month_number("March");
    Print("Month number for March: ", mon);

    datetime direct = (datetime)time_shield::ts("2024-01-01T00:00:00Z");
    Print("ts() shortcut: ", direct);
}
