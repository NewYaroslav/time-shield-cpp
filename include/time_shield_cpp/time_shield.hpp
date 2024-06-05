#pragma once
/// \file time_shield.hpp
/// \brief Main header file for the Time Shield library.
///
/// This header file includes all the components of the Time Shield library,
/// making it easy to include the entire library in your projects with a single
/// include directive.
///
#ifndef _TIME_SHIELD_HPP_INCLUDED
#define _TIME_SHIELD_HPP_INCLUDED

#include "parts/config.hpp"             ///< Configuration settings for the Time Shield library.
#include "parts/types.hpp"              ///< Type definitions used throughout the library.
#include "parts/constants.hpp"          ///< Constants used in time calculations.
#include "parts/enums.hpp"              ///< Enumerations used in time representations.
#include "parts/time_struct.hpp"        ///< Structures representing time components.
#include "parts/date_struct.hpp"        ///< Structures representing date components.
#include "parts/time_zone_struct.hpp"   ///< Structure representing a time zone.
#include "parts/date_time_struct.hpp"   ///< Structure representing date and time components.
#include "parts/validation.hpp"         ///< Functions for validation of time-related values.
#include "parts/time_utils.hpp"         ///< Utility functions for time manipulation.
#include "parts/time_conversions.hpp"   ///< Functions for converting between different time representations.
#include "parts/time_formatting.hpp"    ///< Functions for formatting time in various standard formats.

/// \namespace tsh
/// \brief Alias for the namespace time_shield.
namespace tsh = time_shield;

#endif // _TIME_SHIELD_HPP_INCLUDED
