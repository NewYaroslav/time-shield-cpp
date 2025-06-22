#pragma once
#ifndef _TIME_SHIELD_INITIALIZATION_HPP_INCLUDED
#define _TIME_SHIELD_INITIALIZATION_HPP_INCLUDED

/// \file initialization.hpp
/// \brief Library initialization helpers.
///
/// Call ::initialize_library() once before using other functions.

#include "time_utils.hpp"

namespace time_shield {

/// \defgroup time_initialization Library Initialization
/// \brief Functions used to initialize the Time Shield library.
/// \{

/// \brief Initialize the Time Shield library.
///
/// This function performs any required setup steps.
inline void initialize_library() {
    microseconds();
}

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_INITIALIZATION_HPP_INCLUDED
