// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_PARSER_HPP_INCLUDED
#define _TIME_SHIELD_TIME_PARSER_HPP_INCLUDED

/// \file time_parser.hpp
/// \brief Header file with functions for parsing dates and times in ISO8601 format and converting them to various timestamp formats.
///
/// This file contains functions for parsing ISO8601 date and time strings, extracting month numbers from month names,
/// and converting parsed date and time information to different timestamp formats.

#include "enums.hpp"
#include "constants.hpp"
#include "date_time_struct.hpp"
#include "time_zone_struct.hpp"
#include "validation.hpp"
#include "time_conversions.hpp"
#include <regex>
#include <algorithm>
#include <locale>
#include <array>
#include <stdexcept>
#include <sstream>
#include <cctype>

namespace time_shield {

/// \defgroup time_parsing Time Parsing
/// \brief A comprehensive set of functions for parsing and converting date and time strings.
///
/// This module provides utilities for parsing date and time strings in ISO8601 format,
/// extracting date components, and converting them into various timestamp formats.
///
/// ### Key Features:
/// - Parse ISO8601 date and time strings.
/// - Extract month numbers from month names.
/// - Convert parsed date and time to timestamp formats (seconds, milliseconds, floating-point).
///
/// ### Usage Examples:
/// - Parse an ISO8601 string and get a timestamp:
///   \code{.cpp}
///   ts_t timestamp;
///   bool success = time_shield::str_to_ts("2024-11-25T14:30:00Z", timestamp);
///   \endcode
///
/// - Extract a month number from a string:
///   \code{.cpp}
///   time_shield::Month month = time_shield::get_month_number<Month>("March");
///   \endcode
///
/// - Parse an ISO8601 string into a DateTimeStruct:
///   \code{.cpp}
///   time_shield::DateTimeStruct dt;
///   time_shield::TimeZoneStruct tz;
///   bool valid = time_shield::parse_iso8601("2024-11-25T14:30:00+01:00", dt, tz);
///   \endcode
///
/// \{

    /// \brief Get the month number by name.
    /// \tparam T The return type, default is Month enum.
    /// \param month The name of the month as a string.
    /// \return The month number corresponding to the given name.
    /// \throw std::invalid_argument if the month name is invalid.
    template<class T = Month>
    T get_month_number(const std::string& month) {
        if (month.empty()) throw std::invalid_argument("Invalid month name");

        std::string month_copy = month;
        std::transform(month_copy.begin(), month_copy.end(), month_copy.begin(), [](char &ch) {
            return std::use_facet<std::ctype<char>>(std::locale()).tolower(ch);
        });
        month_copy[0] = static_cast<char>(std::toupper(month_copy[0]));

        static const std::array<std::string, 12> short_names = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        static const std::array<std::string, 12> full_names = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        for(int i = 0; i < MONTHS_PER_YEAR; ++i) {
            if (month == short_names[i] || month == full_names[i]) {
                return static_cast<T>(i + 1);
            }
        }
        throw std::invalid_argument("Invalid month name");
    }

    /// \brief Alias for get_month_number function.
    /// \copydoc get_month_number
    template<class T = Month>
    T month_of_year(const std::string& month) {
        return get_month_number(month);
    }

//------------------------------------------------------------------------------

    /// \brief Get the month number by name, with output parameter.
    /// \tparam T The type for the month number, default is Month enum.
    /// \param month The name of the month as a string.
    /// \param value The reference to store the month number if found.
    /// \return True if the month name is valid, false otherwise.
    template<class T = Month>
    bool try_get_month_number(const std::string& month, T& value) {
        if (month.empty()) return false;

        std::string month_copy = month;
        std::transform(month_copy.begin(), month_copy.end(), month_copy.begin(), [](char &ch) {
            return std::use_facet<std::ctype<char>>(std::locale()).tolower(ch);
        });
        month_copy[0] = static_cast<char>(std::toupper(month_copy[0]));

        static const std::array<std::string, MONTHS_PER_YEAR> short_names = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        static const std::array<std::string, MONTHS_PER_YEAR> full_names = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        for (int i = 0; i < MONTHS_PER_YEAR; ++i) {
            if (month_copy == short_names[i] || month_copy == full_names[i]) {
                value = static_cast<T>(i + 1);
                return true;
            }
        }
        return false;
    }

    /// \brief Alias for try_get_month_number function.
    /// \copydoc try_get_month_number
    template<class T = Month>
    bool get_month_number(const std::string& month, T& value) {
        return try_get_month_number(month, value);
    }

    /// \brief Alias for try_get_month_number function.
    /// \copydoc try_get_month_number
    template<class T = Month>
    bool month_of_year(const std::string& month, T& value) {
        return try_get_month_number(month, value);
    }

//------------------------------------------------------------------------------

    /// \brief Parse a time zone string into a TimeZoneStruct.
    /// \details This function parses a time zone string in the format "+hh:mm" or "Z" into a TimeZoneStruct.
    /// If the string is empty or "Z", it sets the time zone to UTC.
    /// \param tz_str The time zone string.
    /// \param tz The TimeZoneStruct to be filled.
    /// \return True if the parsing is successful and the time zone is valid, false otherwise.
    bool parse_time_zone(const std::string& tz_str, TimeZoneStruct &tz) {
        if (tz_str.empty()) {
            tz.hour = 0;
            tz.min = 0;
            tz.is_positive = true;
            return true;
        }
        if (tz_str == "Z") {
            tz.hour = 0;
            tz.min = 0;
            tz.is_positive = true;
            return true;
        }
        tz.is_positive = (tz_str[0] == '+');
        tz.hour = std::stoi(tz_str.substr(1, 2));
        tz.min = std::stoi(tz_str.substr(4, 2));
        return is_valid_time_zone(tz);
    }

    /// \brief Alias for parse_time_zone function.
    /// \copydoc parse_time_zone
    inline bool parse_tz(const std::string& tz_str, TimeZoneStruct &tz) {
        return parse_time_zone(tz_str, tz);
    }

//------------------------------------------------------------------------------

    /// \brief Parse a date and time string in ISO8601 format.
    /// \details This function parses a date and time string in the ISO8601 format and fills the provided
    /// DateTimeStruct and TimeZoneStruct with the parsed values.
    /// \param input The input string in ISO8601 format.
    /// \param dt The DateTimeStruct to be filled with the parsed date and time values.
    /// \param tz The TimeZoneStruct to be filled with the parsed time zone values.
    /// \return True if the parsing is successful and the date and time values are valid, false otherwise.
    bool parse_iso8601(
            const std::string& input,
            DateTimeStruct &dt,
            TimeZoneStruct &tz) {
        // Регулярное выражение для даты в формате ISO8601
        static const std::regex date_regex(R"((\d{4})[-\/\.](\d{2})[-\/\.](\d{2}))");
        std::smatch date_match;

        // Регулярное выражение для времени в формате ISO8601 с часовым поясом и без
        static const std::regex time_regex(R"((\d{2}):(\d{2}):(\d{2})(?:\.(\d+))?(Z|[+-]\d{2}:\d{2})?)");
        std::smatch time_match;

        dt = create_date_time_struct(0);
        tz = create_time_zone_struct(0, 0);

        // Парсинг даты
        if (std::regex_search(input, date_match, date_regex)) {
            dt.year = std::stoll(date_match[1].str());
            dt.mon = std::stoi(date_match[2].str());
            dt.day = std::stoi(date_match[3].str());
        } else {
            return false;
        }

        // Парсинг времени и часового пояса
        if (std::regex_search(input, time_match, time_regex)) {
            dt.hour = std::stoi(time_match[1].str());
            dt.min = std::stoi(time_match[2].str());
            dt.sec = std::stoi(time_match[3].str());
            if (time_match[4].matched) {
                dt.ms = std::stoi(time_match[4].str());
            }
            if (time_match[5].matched) {
                if (!parse_time_zone(time_match[5].str(), tz)) return false;
            }
            return is_valid_date_time(dt);
        }
        return true;
    }

    /// \brief Convert an ISO8601 string to a timestamp (ts_t).
    /// \details This function parses a string in ISO8601 format and converts it to a timestamp.
    /// \param str The ISO8601 string.
    /// \param ts The timestamp to be filled.
    /// \return True if the parsing and conversion are successful, false otherwise.
    inline bool str_to_ts(const std::string &str, ts_t& ts) {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!parse_iso8601(str, dt, tz)) return false;
        try {
            ts = to_timestamp(dt) + to_offset(tz);
            return true;
        } catch(...) {}
        return false;
    }

    /// \brief Convert an ISO8601 string to a millisecond timestamp (ts_ms_t).
    /// \details This function parses a string in ISO8601 format and converts it to a millisecond timestamp.
    /// \param str The ISO8601 string.
    /// \param ts The millisecond timestamp to be filled.
    /// \return True if the parsing and conversion are successful, false otherwise.
    inline bool str_to_ts_ms(const std::string &str, ts_ms_t& ts) {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!parse_iso8601(str, dt, tz)) return false;
        try {
            ts = to_timestamp_ms(dt) + sec_to_ms(to_offset(tz));
            return true;
        } catch(...) {}
        return false;
    }

    /// \brief Convert an ISO8601 string to a floating-point timestamp (fts_t).
    /// \details This function parses a string in ISO8601 format and converts it to a floating-point timestamp.
    /// \param str The ISO8601 string.
    /// \param ts The floating-point timestamp to be filled.
    /// \return True if the parsing and conversion are successful, false otherwise.
    inline bool str_to_fts(const std::string &str, fts_t& ts) {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!parse_iso8601(str, dt, tz)) return false;
        try {
            ts = to_ftimestamp(dt) + static_cast<fts_t>(to_offset(tz));
            return true;
        } catch(...) {}
        return false;
    }

    /// \brief Convert an ISO8601 string to a timestamp (ts_t).
    /// \details This function parses a string in ISO8601 format and converts it to a timestamp.
    /// If parsing fails, it returns 0.
    /// \param str The ISO8601 string.
    /// \return The timestamp value. Returns 0 if parsing fails.
    inline ts_t ts(const std::string& str) {
        ts_t ts = 0;
        str_to_ts(str, ts);
        return ts;
    }

    /// \brief Convert an ISO8601 string to a millisecond timestamp (ts_ms_t).
    /// \details This function parses a string in ISO8601 format and converts it to a millisecond timestamp.
    /// If parsing fails, it returns 0.
    /// \param str The ISO8601 string.
    /// \return The parsed millisecond timestamp, or 0 if parsing fails.
    inline ts_ms_t ts_ms(const std::string& str) {
        ts_ms_t ts = 0;
        str_to_ts_ms(str, ts);
        return ts;
    }

    /// \brief Convert an ISO8601 string to a floating-point timestamp (fts_t).
    /// \details This function parses a string in ISO8601 format and converts it to a floating-point timestamp.
    /// If the parsing fails, it returns 0.
    /// \param str The ISO8601 string.
    /// \return The floating-point timestamp if successful, 0 otherwise.
    inline fts_t fts(const std::string& str) {
        fts_t ts = 0;
        str_to_fts(str, ts);
        return ts;
    }

    //--------------------------------------------------------------------------

    /// \brief Parse time of day string to seconds of day.
    ///
    /// Supported formats:
    /// - HH:MM:SS
    /// - HH:MM
    /// - HH
    ///
    /// \tparam T Return type (default int).
    /// \param str Time of day as string.
    /// \param sec Parsed seconds of day on success.
    /// \return True on successful parsing.
    template<class T = int>
    inline bool sec_of_day(const std::string& str, T& sec) {
        if (str.empty()) return false;

        const char* p = str.c_str();
        int parts[3] = {0, 0, 0}; // hour, minute, second
        int idx = 0;

        while (*p && idx < 3) {
            // Parse integer
            int value = 0;
            bool has_digit = false;

            while (*p >= '0' && *p <= '9') {
                has_digit = true;
                value = value * 10 + (*p - '0');
                ++p;
            }

            if (!has_digit) return false;
            parts[idx++] = value;

            // Expect colon or end
            if (*p == ':') {
                ++p;
            } else if (*p == '\0') {
                break;
            } else {
                return false; // unexpected character
            }
        }

        if (idx == 0) return false;
        if (!is_valid_time(parts[0], parts[1], parts[2])) return false;

        sec = static_cast<T>(sec_of_day(parts[0], parts[1], parts[2]));
        return true;
    }

    /// \brief Convert time of day string to seconds of day.
    ///
    /// Supported formats:
    /// - HH:MM:SS
    /// - HH:MM
    /// - HH
    ///
    /// \tparam T Return type (default int).
    /// \param str Time of day as string.
    /// \return Parsed seconds of day or SEC_PER_DAY if parsing fails.
    template<class T = int>
    inline T sec_of_day(const std::string& str) {
        T value{};
        if (sec_of_day(str, value))
            return value;
        return static_cast<T>(SEC_PER_DAY);
    }


    /// \brief Convert an ISO8601 C-style string to a timestamp (ts_t).
    /// \details This function parses a string in ISO8601 format and converts it to a timestamp.
    /// If parsing fails, it returns 0.
    /// \param str The ISO8601 C-style string.
    /// \return The timestamp value. Returns 0 if parsing fails.
    inline ts_t ts(const char *str) {
        ts_t ts = 0;
        str_to_ts(str, ts);
        return ts;
    }

    /// \brief Convert an ISO8601 C-style string to a millisecond timestamp (ts_ms_t).
    /// \details This function parses a string in ISO8601 format and converts it to a millisecond timestamp.
    /// If parsing fails, it returns 0.
    /// \param str The ISO8601 C-style string.
    /// \return The parsed millisecond timestamp, or 0 if parsing fails.
    inline ts_ms_t ts_ms(const char *str) {
        ts_ms_t ts = 0;
        str_to_ts_ms(str, ts);
        return ts;
    }

    /// \brief Convert an ISO8601 C-style string to a floating-point timestamp (fts_t).
    /// \details This function parses a string in ISO8601 format and converts it to a floating-point timestamp.
    /// If the parsing fails, it returns 0.
    /// \param str The ISO8601 C-style string.
    /// \return The floating-point timestamp if successful, 0 otherwise.
    inline fts_t fts(const char *str) {
        fts_t ts = 0;
        str_to_fts(str, ts);
        return ts;
    }

/// \}

};

#endif // _TIME_SHIELD_TIME_PARSER_HPP_INCLUDED
