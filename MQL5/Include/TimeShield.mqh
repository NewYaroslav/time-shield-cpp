//+------------------------------------------------------------------+
//|                                                   TimeShield.mqh |
//|                        MQL5 Facade for TimeShield C++ Library    |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_FACADE_MQH__
#define __TIME_SHIELD_FACADE_MQH__

/// \file TimeShield.mqh
/// \brief MQL5-style facade header for the Time Shield library.
/// 
/// This is a thin wrapper over the internal `time_shield.mqh` header, designed
/// to provide a user-friendly PascalCase interface for MQL5 users.
/// Prefer `#include <TimeShield.mqh>` when using the library in MQL5.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

/// \defgroup mql5 MQL5 TimeShield API
/// \brief Components of the Time Shield library adapted for MetaTrader 5.
/// @{
///
/// This group includes all functions, structures, and utilities that are part of the MQL5-compatible version
/// of the Time Shield library.
///
/// Example usage:
/// \code
/// #include <TimeShield.mqh>
/// time_shield::DateStruct d = time_shield::create_date_struct(2025, 6, 21);
/// \endcode
///
/// @}

#include <time_shield.mqh>

#endif // __TIME_SHIELD_FACADE_MQH__