// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_PARSER_HPP_INCLUDED
#define _TIME_SHIELD_TIME_PARSER_HPP_INCLUDED

/// \file time_parser.hpp
/// \brief Header file with functions for parsing dates and times in ISO8601 format and converting them to various timestamp formats.
///
/// This file contains functions for parsing ISO8601 date and time strings, extracting month numbers from month names,
/// and converting parsed date and time information to different timestamp formats.
///
/// Provides:
/// - Month name parsing (e.g. "Jan", "January") to month index (1..12).
/// - ISO8601 date/time parsing into DateTimeStruct + TimeZoneStruct.
/// - Convenience functions to convert ISO8601 strings to timestamps (sec/ms/float).
///
/// \note If you need strict error handling, prefer the `str_to_*` functions that return bool.

#include "enums.hpp"
#include "constants.hpp"
#include "date_time_struct.hpp"
#include "time_zone_struct.hpp"
#include "validation.hpp"
#include "time_conversions.hpp"
#include "iso_week_conversions.hpp"

#include <algorithm>
#include <locale>
#include <array>
#include <stdexcept>
#include <cctype>
#include <string>

#if __cplusplus >= 201703L
#   include <string_view>
#endif

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
    
    namespace detail {

        /// \brief Trim ASCII whitespace from both ends.
        inline std::string trim_copy_ascii(const std::string& s) {
            size_t b = 0;
            size_t e = s.size();
            while (b < e && std::isspace(static_cast<unsigned char>(s[b])) != 0) ++b;
            while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])) != 0) --e;
            return s.substr(b, e - b);
        }

#   if __cplusplus >= 201703L
        /// \brief Trim ASCII whitespace from both ends (string_view).
        inline std::string_view trim_view_ascii(std::string_view v) {
            size_t b = 0;
            size_t e = v.size();
            while (b < e && std::isspace(static_cast<unsigned char>(v[b])) != 0) ++b;
            while (e > b && std::isspace(static_cast<unsigned char>(v[e - 1])) != 0) --e;
            return v.substr(b, e - b);
        }
#   endif

        /// \brief Normalize month token to lower-case ASCII using current locale facet.
        /// \param month Input token.
        /// \param output Output lower-case token (overwritten).
        inline void normalise_month_token_lower(const std::string& month, std::string& output) {
            output = trim_copy_ascii(month);
            if (output.empty()) return;

            const auto& facet = std::use_facet<std::ctype<char>>(std::locale());
            std::transform(output.begin(), output.end(), output.begin(),
                           [&facet](char ch) { return facet.tolower(ch); });
        }

#   if __cplusplus >= 201703L
        /// \brief Normalize month token to lower-case ASCII using current locale facet (string_view).
        /// \param month Input token view.
        /// \param output Output lower-case token (overwritten).
        inline void normalise_month_token_lower(std::string_view month, std::string& output) {
            month = trim_view_ascii(month);
            output.assign(month.begin(), month.end());
            if (output.empty()) return;

            const auto& facet = std::use_facet<std::ctype<char>>(std::locale());
            std::transform(output.begin(), output.end(), output.begin(),
                           [&facet](char ch) { return facet.tolower(ch); });
        }
#   endif

        /// \brief Try parse month name token into month index (1..12).
        /// \param month Month token (e.g. "Jan", "January", case-insensitive).
        /// \param value Output month index in range [1..12].
        /// \return True if token matches a supported month name, false otherwise.
        inline bool try_parse_month_index(const std::string& month, int& value) {
            if (month.empty()) return false;

            std::string month_copy;
            normalise_month_token_lower(month, month_copy);
            if (month_copy.empty()) return false;

            static const std::array<const char*, MONTHS_PER_YEAR> short_names = {
                "jan", "feb", "mar", "apr", "may", "jun",
                "jul", "aug", "sep", "oct", "nov", "dec"
            };
            static const std::array<const char*, MONTHS_PER_YEAR> full_names = {
                "january", "february", "march", "april", "may", "june",
                "july", "august", "september", "october", "november", "december"
            };

            for (std::size_t i = 0; i < short_names.size(); ++i) {
                if (month_copy == short_names[i] || month_copy == full_names[i]) {
                    value = static_cast<int>(i) + 1;
                    return true;
                }
            }

            return false;
        }

#   if __cplusplus >= 201703L
        /// \brief Try parse month name token into month index (1..12), string_view overload.
        /// \param month Month token view (e.g. "Jan", "January", case-insensitive).
        /// \param value Output month index in range [1..12].
        /// \return True if token matches a supported month name, false otherwise.
        inline bool try_parse_month_index(std::string_view month, int& value) {
            if (month.empty()) return false;

            std::string month_copy;
            normalise_month_token_lower(month, month_copy);
            if (month_copy.empty()) return false;

            static const std::array<const char*, MONTHS_PER_YEAR> short_names = {
                "jan", "feb", "mar", "apr", "may", "jun",
                "jul", "aug", "sep", "oct", "nov", "dec"
            };
            static const std::array<const char*, MONTHS_PER_YEAR> full_names = {
                "january", "february", "march", "april", "may", "june",
                "july", "august", "september", "october", "november", "december"
            };

            for (std::size_t i = 0; i < short_names.size(); ++i) {
                if (month_copy == short_names[i] || month_copy == full_names[i]) {
                    value = static_cast<int>(i) + 1;
                    return true;
                }
            }

            return false;
        }
#   endif

        /// \brief Parse month name token into month index (1..12).
        /// \param month Month token.
        /// \return Month index [1..12].
        /// \throw std::invalid_argument if token is invalid.
        inline int parse_month_index(const std::string& month) {
            int value = 0;
            if (!try_parse_month_index(month, value)) {
                throw std::invalid_argument("Invalid month name");
            }
            return value;
        }

#   if __cplusplus >= 201703L
        /// \brief Parse month name token into month index (1..12), string_view overload.
        /// \param month Month token view.
        /// \return Month index [1..12].
        /// \throw std::invalid_argument if token is invalid.
        inline int parse_month_index(std::string_view month) {
            int value = 0;
            if (!try_parse_month_index(month, value)) {
                throw std::invalid_argument("Invalid month name");
            }
            return value;
        }
#   endif

//------------------------------------------------------------------------------
// Small C-style helpers (no lambdas, no detail namespace)
//------------------------------------------------------------------------------

        /// \brief Check whether character is ASCII whitespace.
        TIME_SHIELD_CONSTEXPR inline bool is_ascii_space(char c) noexcept {
            return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
        }

        /// \brief Check whether character is ASCII digit.
        TIME_SHIELD_CONSTEXPR inline bool is_ascii_digit(char c) noexcept {
            return c >= '0' && c <= '9';
        }

        /// \brief Skip ASCII whitespace.
        TIME_SHIELD_CONSTEXPR inline void skip_spaces(const char*& p, const char* end) noexcept {
            while (p < end && is_ascii_space(*p)) {
                ++p;
            }
        }

        /// \brief Parse exactly 2 digits into int.
        /// \return true on success.
        TIME_SHIELD_CONSTEXPR inline bool parse_2digits(const char*& p, const char* end, int& out) noexcept {
            if (end - p < 2) {
                return false;
            }
            const char a = p[0];
            const char b = p[1];
            if (!is_ascii_digit(a) || !is_ascii_digit(b)) {
                return false;
            }
            out = (a - '0') * 10 + (b - '0');
            p += 2;
            return true;
        }

        /// \brief Parse exactly 4 digits into year_t (via int).
        /// \return true on success.
        TIME_SHIELD_CONSTEXPR inline bool parse_4digits_year(const char*& p, const char* end, year_t& out) noexcept {
            if (end - p < 4) {
                return false;
            }
            const char a = p[0], b = p[1], c = p[2], d = p[3];
            if (!is_ascii_digit(a) || !is_ascii_digit(b) || !is_ascii_digit(c) || !is_ascii_digit(d)) {
                return false;
            }
            const int v = (a - '0') * 1000 + (b - '0') * 100 + (c - '0') * 10 + (d - '0');
            out = static_cast<year_t>(v);
            p += 4;
            return true;
        }

        /// \brief Parse fractional seconds (1..9 digits) and convert to milliseconds.
        /// \details Uses first 3 digits, scales if fewer.
        /// \return true on success.
        TIME_SHIELD_CONSTEXPR inline bool parse_fraction_to_ms(const char*& p, const char* end, int& ms_out) noexcept {
            if (p >= end || !is_ascii_digit(*p)) {
                return false;
            }

            int ms = 0;
            int digits = 0;

            while (p < end && is_ascii_digit(*p)) {
                if (digits >= 3) {
                    return false;
                }
                ms = ms * 10 + (*p - '0');
                ++digits;
                ++p;
            }

            if (digits == 1) {
                ms *= 100;
            } else if (digits == 2) {
                ms *= 10;
            }

            ms_out = ms;
            return true;
        }

    } // namespace detail

//------------------------------------------------------------------------------
// Month helpers (public)
//------------------------------------------------------------------------------

// Canonical API (recommended):
//  - parse_month(...) / try_parse_month(...): return month index as int [1..12]
//  - parse_month_enum(...) / try_parse_month_enum(...): return month as enum Month (or any integral/enum T)

    /// \brief Try parse month name token into month index [1..12].
    /// \param month Month token (e.g. "Jan", "January"), case-insensitive.
    /// \param value Output month index [1..12].
    /// \return True on success, false otherwise.
    inline bool try_parse_month(const std::string& month, int& value) {
        return detail::try_parse_month_index(month, value);
    }

    /// \brief Parse month name token into month index [1..12].
    /// \param month Month token.
    /// \return Month index [1..12].
    /// \throw std::invalid_argument if token is invalid.
    inline int parse_month(const std::string& month) {
        return detail::parse_month_index(month);
    }

#if __cplusplus >= 201703L
    /// \brief Try parse month name token into month index [1..12], string_view overload.
    /// \param month Month token view (e.g. "Jan", "January"), case-insensitive.
    /// \param value Output month index [1..12].
    /// \return True on success, false otherwise.
    inline bool try_parse_month(std::string_view month, int& value) {
        return detail::try_parse_month_index(month, value);
    }

    /// \brief Parse month name token into month index [1..12], string_view overload.
    /// \param month Month token view.
    /// \return Month index [1..12].
    /// \throw std::invalid_argument if token is invalid.
    inline int parse_month(std::string_view month) {
        return detail::parse_month_index(month);
    }
#endif

// Canonical: parse month -> enum Month (or any T)

    /// \brief Parse month name token into Month enum (throwing).
    /// \tparam T Return type, default is Month enum.
    /// \param month Month token.
    /// \return Month number (1..12) converted to T.
    /// \throw std::invalid_argument if token is invalid.
    template<class T = Month>
    inline T parse_month_enum(const std::string& month) {
        return static_cast<T>(detail::parse_month_index(month));
    }

    /// \brief Try parse month name token into Month enum (or any T).
    /// \tparam T Output type, default is Month enum.
    /// \param month Month token.
    /// \param value Output month number (1..12) converted to T.
    /// \return True if month token is valid, false otherwise.
    template<class T = Month>
    inline bool try_parse_month_enum(const std::string& month, T& value) {
        int idx = 0;
        if (!detail::try_parse_month_index(month, idx)) return false;
        value = static_cast<T>(idx);
        return true;
    }

#if __cplusplus >= 201703L
    /// \brief Parse month name token into Month enum (throwing), string_view overload.
    /// \tparam T Return type, default is Month enum.
    /// \param month Month token view.
    /// \return Month number (1..12) converted to T.
    /// \throw std::invalid_argument if token is invalid.
    template<class T = Month>
    inline T parse_month_enum(std::string_view month) {
        return static_cast<T>(detail::parse_month_index(month));
    }

    /// \brief Try parse month name token into Month enum (or any T), string_view overload.
    /// \tparam T Output type, default is Month enum.
    /// \param month Month token view.
    /// \param value Output month number (1..12) converted to T.
    /// \return True if month token is valid, false otherwise.
    template<class T = Month>
    inline bool try_parse_month_enum(std::string_view month, T& value) {
        int idx = 0;
        if (!detail::try_parse_month_index(month, idx)) return false;
        value = static_cast<T>(idx);
        return true;
    }
#endif

// Index aliases (int)

    /// \brief Try parse month name token into month index [1..12].
    /// \param month Month token (e.g. "Jan", "January"), case-insensitive.
    /// \param value Output month index [1..12].
    /// \return True on success, false otherwise.
    inline bool try_get_month_index(const std::string& month, int& value) {
        return try_parse_month(month, value);
    }

    /// \brief Parse month name token into month index [1..12].
    /// \param month Month token.
    /// \return Month index [1..12].
    /// \throw std::invalid_argument if token is invalid.
    inline int get_month_index(const std::string& month) {
        return parse_month(month);
    }

    /// \brief Parse month name token into Month enum.
    /// \param month Month token.
    /// \return Month enum value (1..12).
    /// \throw std::invalid_argument if token is invalid.
    inline Month get_month_index_enum(const std::string& month) {
        return static_cast<Month>(detail::parse_month_index(month));
    }

#if __cplusplus >= 201703L
    /// \brief Try parse month name token into month index [1..12], string_view overload.
    inline bool try_get_month_index(std::string_view month, int& value) {
        return try_parse_month(month, value);
    }

    /// \brief Parse month name token into month index [1..12], string_view overload.
    inline int get_month_index(std::string_view month) {
        return parse_month(month);
    }

    /// \brief Parse month name token into Month enum, string_view overload.
    inline Month get_month_index_enum(std::string_view month) {
        return static_cast<Month>(detail::parse_month_index(month));
    }
#endif

// Month number aliases (T)

    /// \brief Get the month number by name (throwing).
    /// \tparam T Return type, default is Month enum.
    /// \param month Month token.
    /// \return Month number (1..12) converted to T.
    /// \throw std::invalid_argument if token is invalid.
    template<class T = Month>
    inline T get_month_number(const std::string& month) {
        return parse_month_enum<T>(month);
    }

    /// \brief Alias for get_month_number (throwing).
    template<class T = Month>
    inline T month_of_year(const std::string& month) {
        return get_month_number<T>(month);
    }

    /// \brief Try get the month number by name, with output parameter.
    /// \tparam T Output type, default is Month enum.
    /// \param month Month token.
    /// \param value Output month number (1..12) converted to T.
    /// \return True if month token is valid, false otherwise.
    template<class T = Month>
    inline bool try_get_month_number(const std::string& month, T& value) {
        return try_parse_month_enum<T>(month, value);
    }

    /// \brief Alias for try_get_month_number (output parameter).
    template<class T = Month>
    inline bool get_month_number(const std::string& month, T& value) {
        return try_get_month_number<T>(month, value);
    }

    /// \brief Alias for try_get_month_number (output parameter).
    template<class T = Month>
    inline bool month_of_year(const std::string& month, T& value) {
        return try_get_month_number<T>(month, value);
    }

#if __cplusplus >= 201703L
    /// \brief Get the month number by name (throwing), string_view overload.
    template<class T = Month>
    inline T get_month_number(std::string_view month) {
        return parse_month_enum<T>(month);
    }

    /// \brief Alias for get_month_number (throwing), string_view overload.
    template<class T = Month>
    inline T month_of_year(std::string_view month) {
        return get_month_number<T>(month);
    }

    /// \brief Try get the month number by name, string_view overload.
    template<class T = Month>
    inline bool try_get_month_number(std::string_view month, T& value) {
        return try_parse_month_enum<T>(month, value);
    }

    /// \brief Alias for try_get_month_number, string_view overload.
    template<class T = Month>
    inline bool get_month_number(std::string_view month, T& value) {
        return try_get_month_number<T>(month, value);
    }

    /// \brief Alias for try_get_month_number, string_view overload.
    template<class T = Month>
    inline bool month_of_year(std::string_view month, T& value) {
        return try_get_month_number<T>(month, value);
    }
#endif

// const char* overloads to avoid ambiguity with string vs string_view for literals

    /// \brief Get the month number by name (throwing), const char* overload.
    /// \tparam T Return type, default is Month enum.
    /// \param month Month token C-string.
    /// \return Month number (1..12) converted to T.
    /// \throw std::invalid_argument if token is invalid.
    template<class T = Month>
    inline T get_month_number(const char* month) {
#if __cplusplus >= 201703L
        return get_month_number<T>(std::string_view(month));
#else
        return get_month_number<T>(std::string(month));
#endif
    }

    /// \brief Try get the month number by name, const char* overload.
    /// \tparam T Output type, default is Month enum.
    /// \param month Month token C-string.
    /// \param value Output month number (1..12) converted to T.
    /// \return True if month token is valid, false otherwise.
    template<class T = Month>
    inline bool try_get_month_number(const char* month, T& value) {
#if __cplusplus >= 201703L
        return try_get_month_number<T>(std::string_view(month), value);
#else
        return try_get_month_number<T>(std::string(month), value);
#endif
    }

    /// \brief Alias for get_month_number (throwing), const char* overload.
    template<class T = Month>
    inline T month_of_year(const char* month) {
        return get_month_number<T>(month);
    }

    /// \brief Alias for try_get_month_number (output parameter), const char* overload.
    template<class T = Month>
    inline bool get_month_number(const char* month, T& value) {
        return try_get_month_number<T>(month, value);
    }

    /// \brief Alias for try_get_month_number (output parameter), const char* overload.
    template<class T = Month>
    inline bool month_of_year(const char* month, T& value) {
        return try_get_month_number<T>(month, value);
    }

//------------------------------------------------------------------------------
// Time zone parsing (C-style, high performance)
//------------------------------------------------------------------------------

    /// \brief Parse timezone character buffer into TimeZoneStruct.
    /// \details Supported formats:
    /// - ""    -> UTC (+00:00)
    /// - "Z"   -> UTC (+00:00)
    /// - "+HH:MM" or "-HH:MM"
    /// \param data Pointer to timezone buffer (may be not null-terminated).
    /// \param length Number of characters in buffer.
    /// \param tz Output time zone struct.
    /// \return True if parsing succeeds and tz is valid, false otherwise.
    inline bool parse_time_zone(const char* data, std::size_t length, TimeZoneStruct& tz) noexcept {
        if (!data) {
            return false;
        }

        if (length == 0) {
            tz.hour = 0;
            tz.min = 0;
            tz.is_positive = true;
            return true;
        }

        if (length == 1 && (data[0] == 'Z' || data[0] == 'z')) {
            tz.hour = 0;
            tz.min = 0;
            tz.is_positive = true;
            return true;
        }

        if (length != 6) {
            return false;
        }

        const char sign = data[0];
        if (sign != '+' && sign != '-') {
            return false;
        }
        if (data[3] != ':') {
            return false;
        }
        if (!detail::is_ascii_digit(data[1]) || !detail::is_ascii_digit(data[2]) ||
            !detail::is_ascii_digit(data[4]) || !detail::is_ascii_digit(data[5])) {
            return false;
        }

        tz.is_positive = (sign == '+');
        tz.hour = (data[1] - '0') * 10 + (data[2] - '0');
        tz.min  = (data[4] - '0') * 10 + (data[5] - '0');

        return is_valid_time_zone(tz);
    }

    /// \brief Parse timezone string into TimeZoneStruct.
    /// \details Wrapper over parse_time_zone(const char*, std::size_t, TimeZoneStruct&).
    inline bool parse_time_zone(const std::string& tz_str, TimeZoneStruct& tz) noexcept {
        return parse_time_zone(tz_str.c_str(), tz_str.size(), tz);
    }

    /// \brief Alias for parse_time_zone.
    inline bool parse_tz(const std::string& tz_str, TimeZoneStruct& tz) noexcept {
        return parse_time_zone(tz_str, tz);
    }

    /// \brief Alias for parse_time_zone (buffer overload).
    inline bool parse_tz(const char* data, std::size_t length, TimeZoneStruct& tz) noexcept {
        return parse_time_zone(data, length, tz);
    }

//------------------------------------------------------------------------------
// ISO8601 parsing (C-style, no regex, no allocations)
//------------------------------------------------------------------------------

    /// \brief Parse ISO8601 character buffer into DateTimeStruct and TimeZoneStruct.
    /// \details Supported inputs:
    /// - "YYYY-MM-DD"
    /// - "YYYY-MM-DDThh:mm"
    /// - "YYYY-MM-DDThh:mm:ss"
    /// - "YYYY-MM-DDThh:mm:ss.fff" (1..9 digits fraction; milliseconds from first 3 digits, scaled if fewer)
    /// - Any of the above time forms with "Z" or "+HH:MM"/"-HH:MM"
    /// - Separator between date and time: 'T' or ASCII whitespace.
    ///
    /// Date separators supported: '-', '/', '.' (as in original regex).
    ///
    /// \param input Pointer to buffer (may be not null-terminated).
    /// \param length Buffer length.
    /// \param dt Output DateTimeStruct (filled). On success, dt is always initialized.
    /// \param tz Output TimeZoneStruct (filled). If timezone is not present, UTC is used.
    /// \return True if parsing succeeds and dt is valid, false otherwise.
    inline bool parse_iso8601(const char* input, std::size_t length,
                              DateTimeStruct& dt, TimeZoneStruct& tz) noexcept {
        if (!input) {
            return false;
        }

        const char* p = input;
        const char* end = input + length;

        detail::skip_spaces(p, end);

        dt = create_date_time_struct(0);
        tz = create_time_zone_struct(0, 0);
        tz.is_positive = true;

        const char* const date_start = p;
        const char* date_end = p;
        while (date_end < end && *date_end != 'T' && *date_end != 't' && !detail::is_ascii_space(*date_end)) {
            ++date_end;
        }

        bool parsed_iso_week_date = false;
        if (date_end > date_start) {
            IsoWeekDateStruct iso_date{};
            if (parse_iso_week_date(std::string(date_start, static_cast<std::size_t>(date_end - date_start)), iso_date)) {
                const DateStruct calendar_date = iso_week_date_to_date(iso_date);
                dt.year = calendar_date.year;
                dt.mon = calendar_date.mon;
                dt.day = calendar_date.day;
                p = date_end;
                parsed_iso_week_date = true;
            }
        }

        if (!parsed_iso_week_date) {
            // ---- Date: YYYY<sep>MM<sep>DD
            if (!detail::parse_4digits_year(p, end, dt.year)) {
                return false;
            }
            if (p >= end) {
                return false;
            }
            const char sep1 = *p;
            if (sep1 != '-' && sep1 != '/' && sep1 != '.') {
                return false;
            }
            ++p;

            if (!detail::parse_2digits(p, end, dt.mon)) {
                return false;
            }
            if (p >= end) {
                return false;
            }
            const char sep2 = *p;
            if (sep2 != '-' && sep2 != '/' && sep2 != '.') {
                return false;
            }
            ++p;

            if (!detail::parse_2digits(p, end, dt.day)) {
                return false;
            }
        }

        // Date-only?
        {
            const char* q = p;
            detail::skip_spaces(q, end);
            if (q == end) {
                // dt already has time=0 ms=0
                return is_valid_date_time(dt);
            }
        }

        // ---- Date/time separator: 'T' or whitespace
        if (p >= end) {
            return false;
        }

        if (*p == 'T' || *p == 't') {
            ++p;
        } else 
        if (detail::is_ascii_space(*p)) {
            // allow one or more spaces
            detail::skip_spaces(p, end);
        } else {
            return false;
        }

        // ---- Time: hh:mm[:ss][.frac]
        if (!detail::parse_2digits(p, end, dt.hour)) {
            return false;
        }
        if (p >= end || *p != ':') {
            return false;
        }
        ++p;

        if (!detail::parse_2digits(p, end, dt.min)) {
            return false;
        }

        dt.sec = 0;
        dt.ms = 0;
        bool has_seconds = false;

        // Optional :ss
        if (p < end && *p == ':') {
            ++p;
            if (!detail::parse_2digits(p, end, dt.sec)) {
                return false;
            }
            has_seconds = true;
        }

        // Optional .fraction (allowed only if we had seconds in original regex,
        // but we accept it when seconds are present; for hh:mm (no seconds) we keep it strict).
        if (p < end && *p == '.') {
            // require seconds field to exist (avoid accepting YYYY-MM-DDThh:mm.xxx)
            if (!has_seconds) {
                // Ambiguous: could be "hh:mm.fff" which is not in your original formats.
                // Keep strict to preserve behavior.
                return false;
            }

            ++p;
            int ms = 0;
            if (!detail::parse_fraction_to_ms(p, end, ms)) {
                return false;
            }
            dt.ms = ms;
        }

        // ---- Optional timezone: [spaces] (Z | ±HH:MM)
        detail::skip_spaces(p, end);

        if (p < end) {
            if (*p == 'Z' || *p == 'z') {
                tz.hour = 0;
                tz.min = 0;
                tz.is_positive = true;
                ++p;
            } else if (*p == '+' || *p == '-') {
                // need 6 chars
                if (static_cast<std::size_t>(end - p) < 6) {
                    return false;
                }
                if (!parse_time_zone(p, 6, tz)) {
                    return false;
                }
                p += 6;
            }
        }

        detail::skip_spaces(p, end);
        if (p != end) {
            return false;
        }

        return is_valid_date_time(dt);
    }

    /// \brief Parse ISO8601 string into DateTimeStruct and TimeZoneStruct.
    /// \details Wrapper over parse_iso8601(const char*, std::size_t, DateTimeStruct&, TimeZoneStruct&).
    inline bool parse_iso8601(const std::string& input, DateTimeStruct& dt, TimeZoneStruct& tz) noexcept {
        return parse_iso8601(input.c_str(), input.size(), dt, tz);
    }
    
//------------------------------------------------------------------------------
// ISO8601 -> timestamps
//------------------------------------------------------------------------------

    /// \brief Convert an ISO8601 string to a timestamp (ts_t).
    /// \param str ISO8601 string.
    /// \param ts Output timestamp (seconds).
    /// \return True if parsing and conversion succeed, false otherwise.
    inline bool str_to_ts(const std::string& str, ts_t& ts) {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!parse_iso8601(str, dt, tz)) return false;
        try {
            ts = dt_to_timestamp(dt) + to_offset(tz);
            return true;
        } catch (...) {}
        return false;
    }
    
    /// \brief Parse ISO8601 character buffer and convert to timestamp (seconds).
    /// \param data Pointer to character buffer.
    /// \param length Buffer length in bytes.
    /// \param ts Output timestamp in seconds.
    /// \return true if parsing succeeds, false otherwise.
    inline bool str_to_ts(const char* data, std::size_t length, ts_t& ts) {
        if (!data || length == 0) {
            ts = 0;
            return false;
        }
        // Без string_view (C++11) просто собираем std::string по длине.
        return str_to_ts(std::string(data, length), ts);
    }

    /// \brief Convert an ISO8601 string to a millisecond timestamp (ts_ms_t).
    /// \param str ISO8601 string.
    /// \param ts Output timestamp (milliseconds).
    /// \return True if parsing and conversion succeed, false otherwise.
    inline bool str_to_ts_ms(const std::string& str, ts_ms_t& ts) {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!parse_iso8601(str, dt, tz)) return false;
        try {
            ts = static_cast<ts_ms_t>(dt_to_timestamp_ms(dt)) + sec_to_ms(to_offset(tz));
            return true;
        } catch (...) {}
        return false;
    }
    
    /// \brief Convert ISO8601 character buffer to millisecond timestamp (ts_ms_t).
    /// \param data Pointer to character buffer.
    /// \param length Number of characters in buffer.
    /// \param ts Output timestamp in milliseconds.
    /// \return True if parsing and conversion succeed, false otherwise.
    inline bool str_to_ts_ms(const char* data, std::size_t length, ts_ms_t& ts) {
        if (!data || length == 0) {
            ts = 0;
            return false;
        }
        return str_to_ts_ms(std::string(data, length), ts);
    }

    /// \brief Convert an ISO8601 string to a floating-point timestamp (fts_t).
    /// \param str ISO8601 string.
    /// \param ts Output timestamp (floating-point seconds).
    /// \return True if parsing and conversion succeed, false otherwise.
    inline bool str_to_fts(const std::string& str, fts_t& ts) {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!parse_iso8601(str, dt, tz)) return false;
        try {
            ts = dt_to_ftimestamp(dt) + static_cast<fts_t>(to_offset(tz));
            return true;
        } catch (...) {}
        return false;
    }
    
    /// \brief Convert ISO8601 character buffer to floating-point timestamp (fts_t).
    /// \param data Pointer to character buffer.
    /// \param length Number of characters in buffer.
    /// \param ts Output timestamp in floating-point seconds.
    /// \return True if parsing and conversion succeed, false otherwise.
    inline bool str_to_fts(const char* data, std::size_t length, fts_t& ts) {
        if (!data || length == 0) {
            ts = 0;
            return false;
        }
        return str_to_fts(std::string(data, length), ts);
    }

//------------------------------------------------------------------------------
// Convenience string -> predicates (workdays)
//------------------------------------------------------------------------------

    /// \brief Parse ISO8601 string and check if it falls on a workday (seconds precision).
    inline bool is_workday(const std::string& str) {
        ts_t ts = 0;
        if (!str_to_ts(str, ts)) return false;
        return is_workday(ts);
    }

    /// \brief Parse ISO8601 string and check if it falls on a workday (milliseconds precision).
    inline bool is_workday_ms(const std::string& str) {
        ts_ms_t ts = 0;
        if (!str_to_ts_ms(str, ts)) return false;
        return is_workday_ms(ts);
    }
    
    /// \brief Alias for is_workday(const std::string&).
    /// \copydoc is_workday(const std::string&)
    inline bool workday(const std::string& str) {
        return is_workday(str);
    }

    /// \brief Alias for is_workday_ms(const std::string&).
    /// \copydoc is_workday_ms(const std::string&)
    inline bool workday_ms(const std::string& str) {
        return is_workday_ms(str);
    }

    /// \brief Parse ISO8601 string and check if it is the first workday of its month (seconds).
    /// \param str ISO8601 formatted string.
    /// \return true if parsing succeeds and the timestamp corresponds to the first workday of the month, false otherwise.
    inline bool is_first_workday_of_month(const std::string& str) {
        ts_t ts = 0;
        if (!str_to_ts(str, ts)) return false;
        return is_first_workday_of_month(ts);
    }

    /// \brief Parse an ISO8601 string and check if it is the first workday of its month (millisecond precision).
    /// \param str ISO8601 formatted string.
    /// \return true if parsing succeeds and the timestamp corresponds to the first workday of the month, false otherwise.
    inline bool is_first_workday_of_month_ms(const std::string& str) {
        ts_ms_t ts = 0;
        if (!str_to_ts_ms(str, ts)) return false;
        return is_first_workday_of_month_ms(ts);
    }

    /// \brief Parse an ISO8601 string and check if it is the last workday of its month (seconds).
    /// \param str ISO8601 formatted string.
    /// \return true if parsing succeeds and the timestamp corresponds to the last workday of the month, false otherwise.
    inline bool is_last_workday_of_month(const std::string& str) {
        ts_t ts = 0;
        if (!str_to_ts(str, ts)) return false;
        return is_last_workday_of_month(ts);
    }

    /// \brief Parse an ISO8601 string and check if it is the last workday of its month (millisecond).
    /// \param str ISO8601 formatted string.
    /// \return true if parsing succeeds and the timestamp corresponds to the last workday of the month, false otherwise.
    inline bool is_last_workday_of_month_ms(const std::string& str) {
        ts_ms_t ts = 0;
        if (!str_to_ts_ms(str, ts)) return false;
        return is_last_workday_of_month_ms(ts);
    }

    /// \brief Parse an ISO8601 string and check if it falls within the first N workdays of its month.
    /// \param str ISO8601 formatted string.
    /// \param count Number of leading workdays to test against.
    /// \return true if parsing succeeds and the timestamp corresponds to a workday ranked within the first N positions, false otherwise.
    inline bool is_within_first_workdays_of_month(const std::string& str, int count) {
        ts_t ts = 0;
        if (!str_to_ts(str, ts)) return false;
        return is_within_first_workdays_of_month(ts, count);
    }

    /// \brief Parse an ISO8601 string and check if it falls within the first N workdays of its month (millisecond precision).
    /// \param str ISO8601 formatted string.
    /// \param count Number of leading workdays to test against.
    /// \return true if parsing succeeds and the timestamp corresponds to a workday ranked within the first N positions, false otherwise.
    inline bool is_within_first_workdays_of_month_ms(const std::string& str, int count) {
        ts_ms_t ts = 0;
        if (!str_to_ts_ms(str, ts)) return false;
        return is_within_first_workdays_of_month_ms(ts, count);
    }

    /// \brief Parse ISO8601 string and check if it is within last N workdays of its month (seconds).
    /// \param str ISO8601 formatted string.
    /// \param count Number of trailing workdays to test against.
    /// \return true if parsing succeeds and the timestamp corresponds to a workday ranked within the final N positions, false otherwise.
    inline bool is_within_last_workdays_of_month(const std::string& str, int count) {
        ts_t ts = 0;
        if (!str_to_ts(str, ts)) return false;
        return is_within_last_workdays_of_month(ts, count);
    }

    /// \brief Parse ISO8601 string and check if it is within last N workdays of its month (milliseconds).
    /// \param str ISO8601 formatted string.
    /// \param count Number of trailing workdays to test against.
    /// \return true if parsing succeeds and the timestamp corresponds to a workday ranked within the final N positions, false otherwise.
    inline bool is_within_last_workdays_of_month_ms(const std::string& str, int count) {
        ts_ms_t ts = 0;
        if (!str_to_ts_ms(str, ts)) return false;
        return is_within_last_workdays_of_month_ms(ts, count);
    }
    
//------------------------------------------------------------------------------
// Convenience: C-string wrappers (non-throwing, ambiguous on failure)
//------------------------------------------------------------------------------

    /// \brief Convert ISO8601 C-string to timestamp (seconds).
    /// \details Returns 0 on failure (ambiguous if epoch is a valid value for your usage).
    /// \param str C-style string with ISO8601 timestamp, may be nullptr.
    /// \return Timestamp in seconds, or 0 if parsing fails.
    inline ts_t ts(const char* str) {
        ts_t out = 0;
        str_to_ts(str ? std::string(str) : std::string(), out);
        return out;
    }

    /// \brief Convert ISO8601 character buffer to timestamp (seconds).
    /// \details Does not require null terminator. Returns 0 on failure
    ///          (ambiguous if epoch is a valid value for your usage).
    /// \param data Pointer to character buffer.
    /// \param length Number of characters in buffer.
    /// \return Timestamp in seconds, or 0 if parsing fails.
    inline ts_t ts(const char* data, std::size_t length) {
        ts_t out = 0;
        if (!str_to_ts(data, length, out)) {
            return 0;
        }
        return out;
    }

    /// \brief Convert ISO8601 C-string to timestamp (milliseconds).
    /// \details Returns 0 on failure (ambiguous if epoch is a valid value for your usage).
    /// \param str C-style string with ISO8601 timestamp, may be nullptr.
    /// \return Timestamp in milliseconds, or 0 if parsing fails.
    inline ts_ms_t ts_ms(const char* str) {
        ts_ms_t out = 0;
        str_to_ts_ms(str ? std::string(str) : std::string(), out);
        return out;
    }

    /// \brief Convert ISO8601 character buffer to timestamp (milliseconds).
    /// \details Does not require null terminator. Returns 0 on failure.
    /// \param data Pointer to character buffer.
    /// \param length Number of characters in buffer.
    /// \return Timestamp in milliseconds, or 0 if parsing fails.
    inline ts_ms_t ts_ms(const char* data, std::size_t length) {
        ts_ms_t out = 0;
        if (!str_to_ts_ms(data, length, out)) {
            return 0;
        }
        return out;
    }

    /// \brief Convert ISO8601 C-string to floating timestamp (seconds).
    /// \details Returns 0 on failure.
    /// \param str C-style string with ISO8601 timestamp, may be nullptr.
    /// \return Timestamp in seconds (floating-point), or 0 if parsing fails.
    inline fts_t fts(const char* str) {
        fts_t out = 0;
        str_to_fts(str ? std::string(str) : std::string(), out);
        return out;
    }

    /// \brief Convert ISO8601 character buffer to floating timestamp (seconds).
    /// \details Does not require null terminator. Returns 0 on failure.
    /// \param data Pointer to character buffer.
    /// \param length Number of characters in buffer.
    /// \return Timestamp in seconds (floating-point), or 0 if parsing fails.
    inline fts_t fts(const char* data, std::size_t length) {
        fts_t out = 0;
        if (!str_to_fts(data, length, out)) {
            return 0.0;
        }
        return out;
    }

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

/// \}

};

#endif // _TIME_SHIELD_TIME_PARSER_HPP_INCLUDED
