#pragma once
#ifndef _TIME_SHIELD_HPP_INCLUDED
#define _TIME_SHIELD_HPP_INCLUDED

/// \file time_shield.hpp
/// \brief Main header file for the Time Shield library.
///
/// This header file includes all the components of the Time Shield library,
/// making it easy to include the entire library in your projects with a single
/// include directive.

#include "time_shield/config.hpp"                  ///< Configuration settings for the Time Shield library.
#include "time_shield/types.hpp"                   ///< Type definitions used throughout the library.
#include "time_shield/constants.hpp"               ///< Constants used in time calculations.
#include "time_shield/enums.hpp"                   ///< Enumerations used in time representations.
#include "time_shield/time_struct.hpp"             ///< Structures representing time components.
#include "time_shield/date_struct.hpp"             ///< Structures representing date components.
#include "time_shield/time_zone_struct.hpp"        ///< Structure representing a time zone.
#include "time_shield/date_time_struct.hpp"        ///< Structure representing date and time components.
#include "time_shield/validation.hpp"              ///< Functions for validation of time-related values.
#include "time_shield/time_utils.hpp"              ///< Utility functions for time manipulation.
#include "time_shield/time_conversions.hpp"        ///< Functions for converting between different time representations.
#include "time_shield/time_conversion_aliases.hpp" ///< Convenient conversion aliases.
#include "time_shield/time_zone_conversions.hpp"   ///< Functions for converting between time zones.
#include "time_shield/time_formatting.hpp"         ///< Functions for formatting time in various standard formats.
#include "time_shield/time_parser.hpp"             ///< Functions for parsing time in various standard formats.
#if TIME_SHIELD_ENABLE_NTP_CLIENT
#   include "time_shield/ntp_client.hpp"              ///< NTP client for time offset queries.
#endif
#include "time_shield/initialization.hpp"          ///< Library initialization helpers.

/// \namespace tsh
/// \brief Alias for the namespace time_shield.
namespace tsh = time_shield;

/// \namespace tshield
/// \brief Alias for the namespace time_shield.
namespace tshield = time_shield;

/// \namespace time_shield
/// \brief Main namespace for the Time Shield library.
///
/// The time_shield namespace contains all the core components and functions of the Time Shield library.
/// It includes various utilities for working with time and dates, their formatting, conversion, and validation.
namespace time_shield {};

#endif // _TIME_SHIELD_HPP_INCLUDED
