//+------------------------------------------------------------------+
//|                                               initialization.mqh |
//|                   Time Shield - Library Initialization Functions |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_INITIALIZATION_MQH__
#define __TIME_SHIELD_INITIALIZATION_MQH__

/// \file initialization.mqh
/// \ingroup mql5
/// \brief Library initialization helpers.
///
/// Call ::init() once at the beginning of OnStart before
/// using other functions from the library. It currently triggers the
/// lazy initialization used by microseconds().

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include "time_utils.mqh"

namespace time_shield {

    /// \fn void init()
    /// \ingroup mql5_initialization
    /// \brief Initialize the Time Shield library.
    ///
    /// This function performs all necessary setup steps. Call it in
    /// `OnStart` before invoking any other Time Shield functions.
    void init() {
        microseconds();
    }

}; // namespace time_shield

#endif // __TIME_SHIELD_INITIALIZATION_MQH__
