//+------------------------------------------------------------------+
//|                                       test_time_zone_matrix.mq5  |
//|                      Time Shield - Function Test                 |
//|                      Copyright 2025, NewYaroslav                |
//|              https://github.com/NewYaroslav/time-shield-cpp     |
//+------------------------------------------------------------------+
#property script_show_inputs
#property strict

#include <TimeShield.mqh>

void OnStart() {
    time_shield::init();

    datetime ist_local = (datetime)time_shield::to_ts(2024, time_shield::JUL, 15, 12, 0, 0);
    datetime expected_gmt = (datetime)time_shield::to_ts(2024, time_shield::JUL, 15, 6, 30, 0);
    datetime gmt_from_ist = time_shield::ist_to_gmt(ist_local);
    if(gmt_from_ist == expected_gmt)
        Print("IST -> GMT passed");
    else
        Print("IST -> GMT failed: ", gmt_from_ist, " != ", expected_gmt);

    datetime myt_from_ist = time_shield::convert_time_zone(ist_local, time_shield::IST, time_shield::MYT);
    datetime expected_myt = (datetime)time_shield::to_ts(2024, time_shield::JUL, 15, 14, 30, 0);
    if(myt_from_ist == expected_myt)
        Print("IST -> MYT passed");
    else
        Print("IST -> MYT failed: ", myt_from_ist, " != ", expected_myt);

    datetime kyiv_local = (datetime)time_shield::to_ts(2024, time_shield::JUL, 15, 12, 0, 0);
    if(time_shield::kyiv_to_gmt(kyiv_local) == time_shield::eet_to_gmt(kyiv_local))
        Print("Kyiv alias passed");
    else
        Print("Kyiv alias failed");
}
