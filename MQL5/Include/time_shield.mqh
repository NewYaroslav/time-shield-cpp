//+------------------------------------------------------------------+
//|                                                   TimeShield.mqh |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_MQH__
#define __TIME_SHIELD_MQH__

/// \file time_shield.mqh
/// \ingroup mql5
/// \brief Main header file for the Time Shield library.
///
/// This header file includes all the components of the Time Shield library,
/// making it easy to include the entire library in your projects with a single
/// include directive.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

// Configuration settings for the Time Shield library
#include <time_shield/config.mqh>

// Type definitions used throughout the library
#include <time_shield/types.mqh>

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

// Functions for validation of time-related values
#include <time_shield/validation.mqh>

// Utility functions for time manipulation
#include <time_shield/time_utils.mqh>

// Functions for converting between different time representations
#include <time_shield/time_conversions.mqh>

// Functions for formatting time in various standard formats
#include <time_shield/time_formatting.mqh>

// Functions for parsing time in various standard formats
#include <time_shield/time_parser.mqh>

#endif // __TIME_SHIELD_MQH__
