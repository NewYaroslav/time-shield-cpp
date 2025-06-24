//+------------------------------------------------------------------+
//|                                                        enums.mqh |
//|                                  Time Shield - MQL5 Enumerations |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_ENUMS_MQH__
#define __TIME_SHIELD_ENUMS_MQH__

/// \file enums.mqh
/// \ingroup mql5
/// \brief Header file with enumerations for weekdays, months, and other time-related categories.
///
/// This file contains enum definitions for representing various time-related concepts.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

namespace time_shield {

    /// \ingroup mql5_time_enums
    /// \{

    /// Enumeration of the format options for representing a weekday or month.
    enum FormatType {
        UPPERCASE_NAME = 0, ///< Uppercase short name
        SHORT_NAME,         ///< Short name
        FULL_NAME,          ///< Full name
    };

    /// Enumeration of the days of the week.
    enum Weekday {
        SUN = 0,    ///< Sunday
        MON,        ///< Monday
        TUE,        ///< Tuesday
        WED,        ///< Wednesday
        THU,        ///< Thursday
        FRI,        ///< Friday
        SAT         ///< Saturday
    };
    
    /// \fn string to_weekday_str(Weekday value, FormatType format = UPPERCASE_NAME)
    /// \brief Converts a Weekday enum value to a string.
    /// \param value The Weekday enum value to convert.
    /// \param format The format to use for the string representation (default is UPPERCASE_NAME).
    /// \return A string with the representation of the day.
    string to_weekday_str(Weekday value, FormatType format = UPPERCASE_NAME) {
        static const string uppercase_names[] = {
            "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
        };
        static const string short_names[] = {
            "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
        };
        static const string full_names[] = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };
        switch(format) {
        default:
        case UPPERCASE_NAME:
            return uppercase_names[(int)value];
        case SHORT_NAME:
            return short_names[(int)value];
        case FULL_NAME:
            return full_names[(int)value];
        }
        return "";
    }

    /// Enumeration of the months of the year.
    enum Month {
        JAN = 1,    ///< January
        FEB,        ///< February
        MAR,        ///< March
        APR,        ///< April
        MAY,        ///< May
        JUN,        ///< June
        JUL,        ///< July
        AUG,        ///< August
        SEP,        ///< September
        OCT,        ///< October
        NOV,        ///< November
        DEC         ///< December
    };

    /// \fn string to_month_str(Month value, FormatType format = UPPERCASE_NAME)
    /// \brief Converts a Month enum value to a string.
    /// \param value The Month enum value to convert.
    /// \param format The format to use for the string representation (default is UPPERCASE_NAME).
    /// \return A string with the representation of the month.
    string to_month_str(Month value, FormatType format = UPPERCASE_NAME) {
        static const string uppercase_names[] = {
            "",
            "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
            "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
        };
        static const string short_names[] = {
            "",
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        static const string full_names[] = {
            "",
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };
        switch(format) {
        default:
        case UPPERCASE_NAME:
            return uppercase_names[(int)value];
        case SHORT_NAME:
            return short_names[(int)value];
        case FULL_NAME:
            return full_names[(int)value];
        }
        return "";
    }

    /// Enumeration of the time zones.
    enum TimeZone {
        GMT,    ///< Greenwich Mean Time
        UTC,    ///< Coordinated Universal Time
        EET,    ///< Eastern European Time
        CET,    ///< Central European Time
        WET,    ///< Western European Time
        EEST,   ///< Eastern European Summer Time
        CEST,   ///< Central European Summer Time
        WEST,   ///< Western European Summer Time
        UNKNOWN ///< Unknown Time Zone
    };

    /// \fn string to_timezone_str(TimeZone value, FormatType format = UPPERCASE_NAME)
    /// \brief Converts a TimeZone enum value to a string.
    /// \param value The TimeZone enum value to convert.
    /// \param format The format to use for the string representation (default is UPPERCASE_NAME).
    /// \return A string with the representation of the time zone.
    string to_timezone_str(TimeZone value, FormatType format = UPPERCASE_NAME) {
        static const string uppercase_names[] = {
            "GMT", "UTC", "EET", "CET", "WET", "EEST", "CEST", "WEST", "UNKNOWN"
        };
        static const string short_names[] = {
            "GMT", "UTC", "EET", "CET", "WET", "EEST", "CEST", "WEST", "Unknown"
        };
        static const string full_names[] = {
            "Greenwich Mean Time", "Coordinated Universal Time", "Eastern European Time",
            "Central European Time", "Western European Time", "Eastern European Summer Time",
            "Central European Summer Time", "Western European Summer Time", "Unknown Time Zone"
        };
        switch(format) {
        default:
        case UPPERCASE_NAME:
            return uppercase_names[(int)value];
        case SHORT_NAME:
            return short_names[(int)value];
        case FULL_NAME:
            return full_names[(int)value];
        }
        return "";
    }

    /// Enumeration of the moon phases.
    enum MoonPhase {
        WAXING_CRESCENT, ///< Waxing Crescent Moon
        FIRST_QUARTER,   ///< First Quarter Moon
        WAXING_GIBBOUS,  ///< Waxing Gibbous Moon
        FULL_MOON,       ///< Full Moon
        WANING_GIBBOUS,  ///< Waning Gibbous Moon
        LAST_QUARTER,    ///< Last Quarter Moon
        WANING_CRESCENT, ///< Waning Crescent Moon
        NEW_MOON         ///< New Moon
    };

    /// Enumeration of time format types.
    enum TimeFormatType {
        ISO8601_WITH_TZ,      ///< ISO8601 format with time zone (e.g., "2024-06-06T12:30:45+03:00")
        ISO8601_NO_TZ,        ///< ISO8601 format without time zone (e.g., "2024-06-06T12:30:45")
        MQL5_FULL,            ///< MQL5 time format (e.g., "2024.06.06 12:30:45")
        MQL5_DATE_ONLY,       ///< MQL5 date format (e.g., "2024.06.06")
        MQL5_TIME_ONLY,       ///< MQL5 time format (e.g., "12:30:45")
        AMERICAN_MONTH_DAY,   ///< American date format (e.g., "06/06/2024")
        EUROPEAN_MONTH_DAY,   ///< European date format (e.g., "06.06.2024")
        AMERICAN_TIME,        ///< American time format (e.g., "12:30 PM")
        EUROPEAN_TIME         ///< European time format (e.g., "12:30")
    };
    
    /// \}
    
    string to_str(Weekday value, FormatType format = UPPERCASE_NAME) {
        return to_weekday_str(value, format);
    }
    
    string to_str(Month value, FormatType format = UPPERCASE_NAME) {
        return to_month_str(value, format);
    }
    
    string to_str(TimeZone value, FormatType format = UPPERCASE_NAME) {
        return to_timezone_str(value, format);
    }

}; // namespace time_shield

#endif // __TIME_SHIELD_ENUMS_MQH__
