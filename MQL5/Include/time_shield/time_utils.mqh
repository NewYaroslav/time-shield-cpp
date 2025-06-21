//+------------------------------------------------------------------+
//|                                                   time_utils.mqh |
//|                                Time Shield - MQL5 Time Utilities |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_UTILS_MQH__
#define __TIME_SHIELD_TIME_UTILS_MQH__

/// \file time_utils.mqh
/// \ingroup mql5
/// \brief Header with time-related utility functions.
///
/// This file contains various helper functions used for obtaining the current
/// timestamp in different formats and extracting sub-second components in MQL5.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

namespace time_shield {

    /// \defgroup time_utils Time Utilities
    /// \brief Utility functions for working with timestamps and time components.
    /// \{

    /// \brief Get the number of microseconds since the UNIX epoch.
    /// \return Microseconds since 1 January 1970.
    long microseconds() {
       static bool  initialized = false;
       static long  offset      = 0;
       if(!initialized) {
          long  start_ts = TimeGMT();
          ulong start_mc = GetMicrosecondCount();
          long  next_ts  = start_ts;
          while((next_ts = TimeGMT()) == start_ts) {
             // wait until the second changes
          }
          ulong next_mc = GetMicrosecondCount();
          offset        = next_ts * US_PER_SEC - (long)next_mc;
          initialized   = true;
       }
       return (long)GetMicrosecondCount() + offset;
    }

    /// \brief Get the nanosecond part of the current second.
    /// \return Nanosecond part of the current second.
    long ns_of_sec() {
       return (long)((microseconds() % US_PER_SEC) * NS_PER_US);
    }

    /// \brief Get the microsecond part of the current second.
    /// \return Microsecond part of the current second.
    int us_of_sec() {
       return (int)(microseconds() % US_PER_SEC);
    }

    /// \brief Get the millisecond part of the current second.
    /// \return Millisecond part of the current second.
    int ms_of_sec() {
       return (int)((microseconds() / 1000) % MS_PER_SEC);
    }

    /// \brief Get the current UTC timestamp in seconds.
    /// \return Current UTC timestamp in seconds.
    long ts() {
       return microseconds() / US_PER_SEC;
    }

    /// \brief Alias for ts().
    /// \copydoc ts
    long timestamp() { return ts(); }

    /// \brief Get the current UTC timestamp in floating-point seconds.
    /// \return Current UTC timestamp in floating-point seconds.
    double fts() {
       return (double)microseconds() / (double)US_PER_SEC;
    }

    /// \brief Alias for fts().
    /// \copydoc fts
    double ftimestamp() { return fts(); }

    /// \brief Get the current UTC timestamp in milliseconds.
    /// \return Current UTC timestamp in milliseconds.
    long ts_ms() {
       return microseconds() / 1000;
    }

    /// \brief Alias for ts_ms().
    /// \copydoc ts_ms
    long timestamp_ms() { return ts_ms(); }

    /// \brief Alias for ts_ms().
    /// \copydoc ts_ms
    long now() { return ts_ms(); }

    /// \brief Get the current UTC timestamp in microseconds.
    /// \return Current UTC timestamp in microseconds.
    long ts_us() {
       return microseconds();
    }

    /// \brief Alias for ts_us().
    /// \copydoc ts_us
    long timestamp_us() { return ts_us(); }

    /// \}

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_UTILS_MQH__
