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
#include "parts/time_parser.hpp"        ///< Functions for parsing time in various standard formats.

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

/*!
\mainpage Time Shield Library

\section intro_sec Introduction

The Time Shield Library is a comprehensive C++ library designed for time manipulation, formatting, and conversion. This library aims to provide a robust and flexible set of tools for handling various time-related tasks with ease.

\section features_sec Features

- Validation of dates and times
- Time and date formatting
- Time zone calculations
- Conversion between different time representations
- Utilities for time manipulation

\section usage_sec Usage

\code{.cpp}
#include <iostream>
#include "time_shield.hpp"

int main() {
    // Get the current timestamp
    tsh::ts_t current_ts = tsh::timestamp(); // or ts();

    // Calculate the timestamp for the end of the current year
    tsh::ts_t future_ts = tsh::end_of_year(current_ts);

    // Calculate the number of days between current timestamp and the future timestamp
    int days = tsh::get_days_difference(current_ts, future_ts); // or days(current_ts, future_ts);
    std::cout << "Days until end of year: " << days << std::endl;

    // Convert the future timestamp to a formatted string in ISO8601 format
    std::string formatted_time = tsh::to_iso8601_str(future_ts);

    tsh::ts_ms_t ts_ms = 0;
    if (!tsh::str_to_ts_ms(formatted_time, ts_ms)) {
        std::cout << "Error occurred while parsing ISO8601 string" << std::endl;
        return -1;
    }

    // Convert the milliseconds timestamp to a formatted string in ISO8601 UTC format
    formatted_time = tsh::to_iso8601_utc_ms(ts_ms);

    // Output the formatted time
    std::cout << "Future time: " << formatted_time << std::endl;

    return 0;
}
\endcode

\section install_sec Installation

To use the Time Shield Library in your project, include the header file `time_shield.hpp` in your source code. Ensure that your project is set up to compile with standards from C++11 to C++17.

\section repo_sec Repository

[Time Shield Library GitHub repository](https://github.com/NewYaroslav/time-shield-cpp).

\section license_sec License

This library is licensed under the MIT License. See the LICENSE file for more details.
*/

#endif // _TIME_SHIELD_HPP_INCLUDED
