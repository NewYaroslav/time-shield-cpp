//+------------------------------------------------------------------+
//|                                                   TimeShield.mqh |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef TIME_SHIELD_MQL4_MQH_INCLUDED
#define TIME_SHIELD_MQL4_MQH_INCLUDED

/// \file TimeShield.mqh
/// \brief Main MQL4 header file for the Time Shield library.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

/// \class TimeShield
/// \brief Provides static time helpers for MQL4.
class TimeShield {
public:
    enum Constants {
        MS_PER_SEC = 1000,       ///< Milliseconds per second.
        US_PER_SEC = 1000000,    ///< Microseconds per second.
        SEC_PER_MIN = 60,        ///< Seconds per minute.
        SEC_PER_HOUR = 3600,     ///< Seconds per hour.
        SEC_PER_DAY = 86400      ///< Seconds per day.
    };

    /// \brief Get milliseconds per second.
    /// \return Milliseconds per second.
    static int ms_per_sec() {
        return MS_PER_SEC;
    }

    /// \brief Get microseconds per second.
    /// \return Microseconds per second.
    static int us_per_sec() {
        return US_PER_SEC;
    }

    /// \brief Get seconds per minute.
    /// \return Seconds per minute.
    static int sec_per_min() {
        return SEC_PER_MIN;
    }

    /// \brief Get seconds per hour.
    /// \return Seconds per hour.
    static int sec_per_hour() {
        return SEC_PER_HOUR;
    }

    /// \brief Get seconds per day.
    /// \return Seconds per day.
    static int sec_per_day() {
        return SEC_PER_DAY;
    }

    /// \brief Get raw terminal tick counter in milliseconds.
    /// \return Wrapped 32-bit terminal tick counter.
    static uint tick_count_ms() {
        return GetTickCount();
    }

    /// \brief Get monotonic milliseconds since terminal start with wrap handling.
    /// \return Monotonic millisecond counter.
    static ulong monotonic_ms() {
        static bool initialized = false;
        static uint last_raw = 0;
        static ulong high = 0;

        const uint raw = GetTickCount();
        if(!initialized) {
            initialized = true;
            last_raw = raw;
            return (ulong)raw;
        }

        if(raw < last_raw) {
            high += 4294967296;
        }
        last_raw = raw;

        return high + raw;
    }

    /// \brief Get current UTC timestamp in seconds.
    /// \return Current UTC timestamp in seconds.
    static long ts() {
        return (long)TimeGMT();
    }

    /// \brief Alias for ts().
    /// \return Current UTC timestamp in seconds.
    static long timestamp() {
        return ts();
    }

    /// \brief Get current UTC timestamp in milliseconds.
    /// \return Current UTC timestamp in milliseconds.
    static long ts_ms() {
        static bool initialized = false;
        static long offset = 0;

        if(!initialized) {
            const long start_ts = ts();
            long next_ts = start_ts;
            while((next_ts = ts()) == start_ts) {
            }

            offset = next_ts * MS_PER_SEC - (long)monotonic_ms();
            initialized = true;
        }

        return (long)monotonic_ms() + offset;
    }

    /// \brief Alias for ts_ms().
    /// \return Current UTC timestamp in milliseconds.
    static long timestamp_ms() {
        return ts_ms();
    }

    /// \brief Alias for ts_ms().
    /// \return Current UTC timestamp in milliseconds.
    static long now() {
        return ts_ms();
    }

    /// \brief Get second of day from timestamp.
    /// \param timestamp Timestamp in seconds.
    /// \return Second of day.
    static int sec_of_day(const long timestamp) {
        return (int)(timestamp % SEC_PER_DAY);
    }

    /// \brief Get second of day from milliseconds timestamp.
    /// \param timestamp_ms Timestamp in milliseconds.
    /// \return Second of day.
    static int sec_of_day_ms(const long timestamp_ms) {
        return sec_of_day(timestamp_ms / MS_PER_SEC);
    }

    /// \brief Get second of day from hours, minutes and seconds.
    /// \param hour Hour value.
    /// \param minute Minute value.
    /// \param second Second value.
    /// \return Second of day.
    static int sec_of_day(const int hour, const int minute, const int second) {
        return hour * SEC_PER_HOUR + minute * SEC_PER_MIN + second;
    }

    /// \brief Check time-of-day components.
    /// \param hour Hour value.
    /// \param minute Minute value.
    /// \param second Second value.
    /// \return True when the time is valid.
    static bool is_valid_time(const int hour, const int minute, const int second) {
        return hour >= 0 && hour < 24
            && minute >= 0 && minute < 60
            && second >= 0 && second < 60;
    }

    /// \brief Convert string with time of day to second of day.
    /// \param value Time string in HH, HH:MM, or HH:MM:SS format.
    /// \return Second of day or SEC_PER_DAY on parse failure.
    static int sec_of_day(const string value) {
        string parts[];
        const ushort separator = StringGetCharacter(":", 0);
        const int count = StringSplit(value, separator, parts);

        if(count < 1 || count > 3) {
            ArrayFree(parts);
            return SEC_PER_DAY;
        }

        int hour = 0;
        int minute = 0;
        int second = 0;

        hour = (int)StringToInteger(parts[0]);
        if(count > 1) {
            minute = (int)StringToInteger(parts[1]);
        }
        if(count > 2) {
            second = (int)StringToInteger(parts[2]);
        }

        ArrayFree(parts);

        if(!is_valid_time(hour, minute, second)) {
            return SEC_PER_DAY;
        }

        return sec_of_day(hour, minute, second);
    }
};

#endif // TIME_SHIELD_MQL4_MQH_INCLUDED
