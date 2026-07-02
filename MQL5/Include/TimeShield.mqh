//+------------------------------------------------------------------+
//|                                                   TimeShield.mqh |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef TIME_SHIELD_MQL5_MQH_INCLUDED
#define TIME_SHIELD_MQL5_MQH_INCLUDED

/// \file TimeShield.mqh
/// \ingroup mql5
/// \brief Main MQL5 header file for the Time Shield library.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

/// \defgroup mql5 MQL5 TimeShield API
/// \brief Components of the Time Shield library adapted for MetaTrader 5.
///
/// This group includes all functions, structures, and utilities that are part
/// of the MQL5-compatible version of the Time Shield library.
///
/// Example usage:
/// \code
/// #include <TimeShield.mqh>
/// time_shield::DateStruct d = time_shield::create_date_struct(2025, 6, 21);
/// \endcode

// Constants used in time calculations
#include <time_shield/constants.mqh>

// Enumerations used in time representations
#include <time_shield/enums.mqh>

// Structures representing time components
#include <time_shield/time_struct.mqh>

// Structures representing date components
#include <time_shield/date_struct.mqh>

// Structure representing a time zone
#include <time_shield/time_zone_struct.mqh>

// Structure representing date and time combinations
#include <time_shield/date_time_struct.mqh>

// Value-type wrapper for date-time with offset
#include <time_shield/DateTime.mqh>

// Functions for validation of time-related values
#include <time_shield/validation.mqh>

// Utility functions for time manipulation
#include <time_shield/time_utils.mqh>

// Functions for converting between different time representations
#include <time_shield/time_conversions.mqh>

// Functions for converting between time zones
#include <time_shield/time_zone_conversions.mqh>

// Functions for formatting time in various standard formats
#include <time_shield/time_formatting.mqh>

// Functions for parsing time in various standard formats
#include <time_shield/time_parser.mqh>

// Functions for initializing the library
#include <time_shield/initialization.mqh>

#endif // TIME_SHIELD_MQL5_MQH_INCLUDED
