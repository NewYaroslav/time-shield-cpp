//+------------------------------------------------------------------+
//|                                     time_parser_demo.mq5 |
//|                   Time Shield - Parsing Examples         |
//|                      Copyright 2025, NewYaroslav          |
//|              https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#property script_show_inputs
#property strict

#include <TimeShield.mqh>

using namespace time_shield;

void OnStart()
{
    initialize_library();

    string iso = "2024-11-25T14:30:00+01:00";

    datetime ts;
    if(str_to_ts(iso, ts))
        Print("ts: ", ts, " -> ", to_iso8601(ts));
    else
        Print("Failed to parse: ", iso);

    long ts_ms;
    if(str_to_ts_ms(iso, ts_ms))
        Print("ts_ms: ", ts_ms);

    double fts_val;
    if(str_to_fts(iso, fts_val))
        Print("fts: ", DoubleToString(fts_val, 2));

    DateTimeStruct dt;
    TimeZoneStruct tz;
    if(parse_iso8601(iso, dt, tz))
    {
        string sign = tz.is_positive ? "+" : "-";
        Print("dt: ", dt.year, "-", dt.mon, "-", dt.day, " ", dt.hour, ":", dt.min, ":", dt.sec, ".", dt.ms,
              " TZ ", sign, tz.hour, ":", tz.min);
    }

    int mon = get_month_number("March");
    Print("Month number for March: ", mon);

    datetime direct = ts("2024-01-01T00:00:00Z");
    Print("ts() shortcut: ", direct);
}
