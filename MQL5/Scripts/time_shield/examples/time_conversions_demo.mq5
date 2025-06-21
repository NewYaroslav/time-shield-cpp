//+------------------------------------------------------------------+
//|                                    time_conversions_demo.mq5 |
//|                     Time Shield - Conversions Examples       |
//|                      Copyright 2025, NewYaroslav              |
//|              https://github.com/NewYaroslav/time-shield-cpp   |
//+------------------------------------------------------------------+
#property script_show_inputs
#property strict

#include <TimeShield.mqh>

using namespace time_shield;

void OnStart()
{
    double sec = 123.456;
    Print("ns_of_sec: ", ns_of_sec(sec));
    Print("us_of_sec: ", us_of_sec(sec));
    Print("ms_of_sec: ", ms_of_sec(sec));

    long ms = sec_to_ms(sec);
    Print("sec_to_ms: ", ms);
    Print("ms_to_sec: ", ms_to_sec(ms));
    Print("ms_to_fsec: ", DoubleToString(ms_to_fsec(ms), 3));

    Print("min_to_sec(2.5): ", min_to_sec(2.5));
    Print("sec_to_min(150): ", sec_to_min(150));

    Print("hour_to_ms(1.5): ", hour_to_ms(1.5));
    Print("hour24_to_12(15): ", hour24_to_12(15));

    datetime ts = to_ts(2024, JUN, 21, 14, 30, 0);
    long ts_ms = to_ts_ms(2024, JUN, 21, 14, 30, 0, 250);

    DateTimeStruct dt = to_dt_ms(ts_ms);
    Print("to_dt_ms: ", dt.year, "-", dt.mon, "-", dt.day, " ", dt.hour, ":", dt.min, ":", dt.sec, ".", dt.ms);

    Print("start_of_day: ", start_of_day(ts));
    Print("end_of_day_ms: ", end_of_day_ms(ts_ms));
    Print("next_day_ms: ", next_day_ms(ts_ms));

    Print("day_of_year: ", day_of_year(ts));
    Print("month_of_year: ", month_of_year(ts));
    Print("day_of_month: ", day_of_month(ts));
    Print("start_of_year_date(2024): ", start_of_year_date(2024));
    Print("day_of_week_date(2024, JUN, 21): ", day_of_week_date(2024, JUN, 21));
}
