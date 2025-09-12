// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_INITIALIZATION_HPP_INCLUDED
#define _TIME_SHIELD_INITIALIZATION_HPP_INCLUDED

/// \file initialization.hpp
/// \ingroup lib_initialization
/// \brief Initialization helpers for the Time Shield library.
///
/// This file defines the ::time_shield::init() function, which should be called once
/// before using any other Time Shield features that rely on internal time resolution.

#include "time_utils.hpp"

namespace time_shield {

    /// \ingroup lib_initialization
    /// \brief Initializes the Time Shield library.
    ///
    /// This function performs required setup for internal components,
    /// such as triggering lazy initialization used by ::time_shield::now_realtime_us().
    /// Call it once at the beginning of your program before using other parts of the library.
    inline void init() {
        now_realtime_us();
    }

}; // namespace time_shield

#endif // _TIME_SHIELD_INITIALIZATION_HPP_INCLUDED
