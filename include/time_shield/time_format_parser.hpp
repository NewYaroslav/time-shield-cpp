// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_FORMAT_PARSER_HPP_INCLUDED
#define _TIME_SHIELD_TIME_FORMAT_PARSER_HPP_INCLUDED

/// \file time_format_parser.hpp
/// \brief Header file for fast parsing with formatter-compatible custom patterns.

#include "config.hpp"
#include "constants.hpp"
#include "date_time_struct.hpp"
#include "date_time_conversions.hpp"
#include "enums.hpp"
#include "iso_week_conversions.hpp"
#include "time_zone_struct.hpp"
#include "validation.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

#if __cplusplus >= 201703L
#   include <string_view>
#endif

namespace time_shield {

    namespace detail {
    namespace format_parse {

        struct FormatParseState {
            bool has_tz;
            bool has_year;
            bool has_century;
            bool has_two_digit_year;
            bool has_month;
            bool has_day;
            bool has_day_of_year;
            bool has_hour24;
            bool has_hour12;
            bool has_minute;
            bool has_second;
            bool has_millisecond;
            bool has_meridiem;
            bool is_pm;
            bool has_weekday;
            bool has_iso_weekday;
            bool has_unix_seconds;
            year_t year;
            int century;
            int two_digit_year;
            int month;
            int day;
            int day_of_year;
            int hour24;
            int hour12;
            int minute;
            int second;
            int millisecond;
            int weekday;
            int iso_weekday;
            ts_t unix_seconds;
            TimeZoneStruct tz;
        };

        inline FormatParseState create_format_parse_state() noexcept {
            FormatParseState state;
            state.has_tz = false;
            state.has_year = false;
            state.has_century = false;
            state.has_two_digit_year = false;
            state.has_month = false;
            state.has_day = false;
            state.has_day_of_year = false;
            state.has_hour24 = false;
            state.has_hour12 = false;
            state.has_minute = false;
            state.has_second = false;
            state.has_millisecond = false;
            state.has_meridiem = false;
            state.is_pm = false;
            state.has_weekday = false;
            state.has_iso_weekday = false;
            state.has_unix_seconds = false;
            state.year = 0;
            state.century = 0;
            state.two_digit_year = 0;
            state.month = 0;
            state.day = 0;
            state.day_of_year = 0;
            state.hour24 = 0;
            state.hour12 = 0;
            state.minute = 0;
            state.second = 0;
            state.millisecond = 0;
            state.weekday = 0;
            state.iso_weekday = 0;
            state.unix_seconds = 0;
            state.tz = create_time_zone_struct(0, 0, true);
            return state;
        }

        TIME_SHIELD_CONSTEXPR inline bool is_ascii_digit(char c) noexcept {
            return c >= '0' && c <= '9';
        }

        inline bool match_literal(const char*& p, const char* end, char expected) noexcept {
            if (p >= end || *p != expected) {
                return false;
            }
            ++p;
            return true;
        }

        inline bool match_literal(const char*& p, const char* end, const char* literal) noexcept {
            if (!literal) {
                return false;
            }
            while (*literal) {
                if (p >= end || *p != *literal) {
                    return false;
                }
                ++p;
                ++literal;
            }
            return true;
        }

        inline bool parse_exact_2digits(const char*& p, const char* end, int& out) noexcept {
            if (end - p < 2 || !is_ascii_digit(p[0]) || !is_ascii_digit(p[1])) {
                return false;
            }
            out = (p[0] - '0') * 10 + (p[1] - '0');
            p += 2;
            return true;
        }

        inline bool parse_unsigned_digits(
                const char*& p,
                const char* end,
                int min_digits,
                int max_digits,
                int64_t& out) noexcept {
            const char* start = p;
            int digits = 0;
            int64_t value = 0;
            while (p < end && digits < max_digits && is_ascii_digit(*p)) {
                value = value * 10 + static_cast<int64_t>(*p - '0');
                ++p;
                ++digits;
            }
            if (digits < min_digits) {
                p = start;
                return false;
            }
            out = value;
            return true;
        }

        inline bool parse_signed_digits(
                const char*& p,
                const char* end,
                int min_digits,
                int max_digits,
                int64_t& out) noexcept {
            const char* start = p;
            bool negative = false;
            if (p < end && (*p == '+' || *p == '-')) {
                negative = (*p == '-');
                ++p;
            }
            int64_t value = 0;
            if (!parse_unsigned_digits(p, end, min_digits, max_digits, value)) {
                p = start;
                return false;
            }
            out = negative ? -value : value;
            return true;
        }

        inline bool parse_space_padded_2digits(const char*& p, const char* end, int& out) noexcept {
            if (end - p < 2) {
                return false;
            }
            if (p[0] == ' ' && is_ascii_digit(p[1])) {
                out = p[1] - '0';
                p += 2;
                return true;
            }
            return parse_exact_2digits(p, end, out);
        }

        inline bool parse_meridiem(const char*& p, const char* end, bool uppercase, bool& is_pm) noexcept {
            if (end - p < 2) {
                return false;
            }
            if (uppercase) {
                if (p[0] == 'A' && p[1] == 'M') {
                    is_pm = false;
                } else if (p[0] == 'P' && p[1] == 'M') {
                    is_pm = true;
                } else {
                    return false;
                }
            } else {
                if (p[0] == 'a' && p[1] == 'm') {
                    is_pm = false;
                } else if (p[0] == 'p' && p[1] == 'm') {
                    is_pm = true;
                } else {
                    return false;
                }
            }
            p += 2;
            return true;
        }

        inline bool match_name_token(
                const char*& p,
                const char* end,
                const char* const* names,
                std::size_t count,
                int index_base,
                int& out) noexcept {
            for (std::size_t i = 0; i < count; ++i) {
                const char* name = names[i];
                const std::size_t len = std::strlen(name);
                if (static_cast<std::size_t>(end - p) < len) {
                    continue;
                }
                bool matched = true;
                for (std::size_t k = 0; k < len; ++k) {
                    if (p[k] != name[k]) {
                        matched = false;
                        break;
                    }
                }
                if (matched) {
                    p += static_cast<std::ptrdiff_t>(len);
                    out = static_cast<int>(i) + index_base;
                    return true;
                }
            }
            return false;
        }

        inline bool parse_month_token(const char*& p, const char* end, FormatType format, int& month) noexcept {
            static const char* const uppercase_names[] = {
                "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
            };
            static const char* const short_names[] = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
            };
            static const char* const full_names[] = {
                "January", "February", "March", "April", "May", "June",
                "July", "August", "September", "October", "November", "December"
            };
            switch (format) {
                case UPPERCASE_NAME:
                    return match_name_token(p, end, uppercase_names, 12, 1, month);
                case SHORT_NAME:
                    return match_name_token(p, end, short_names, 12, 1, month);
                case FULL_NAME:
                    return match_name_token(p, end, full_names, 12, 1, month);
                default:
                    return false;
            }
        }

        inline bool parse_weekday_token(const char*& p, const char* end, FormatType format, int& weekday) noexcept {
            static const char* const uppercase_names[] = {
                "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
            };
            static const char* const short_names[] = {
                "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
            };
            static const char* const full_names[] = {
                "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
            };
            switch (format) {
                case UPPERCASE_NAME:
                    return match_name_token(p, end, uppercase_names, 7, 0, weekday);
                case SHORT_NAME:
                    return match_name_token(p, end, short_names, 7, 0, weekday);
                case FULL_NAME:
                    return match_name_token(p, end, full_names, 7, 0, weekday);
                default:
                    return false;
            }
        }

        inline bool parse_tz_compact(const char*& p, const char* end, TimeZoneStruct& tz) noexcept {
            if (end - p < 5 || (*p != '+' && *p != '-')) {
                return false;
            }
            tz.is_positive = (*p == '+');
            ++p;
            if (!parse_exact_2digits(p, end, tz.hour)) {
                return false;
            }
            if (!parse_exact_2digits(p, end, tz.min)) {
                return false;
            }
            return is_valid_time_zone_offset(tz);
        }

        inline bool assign_int_field(bool& has_field, int& field, int value) noexcept {
            if (has_field && field != value) {
                return false;
            }
            has_field = true;
            field = value;
            return true;
        }

        inline bool assign_year_field(bool& has_field, year_t& field, year_t value) noexcept {
            if (has_field && field != value) {
                return false;
            }
            has_field = true;
            field = value;
            return true;
        }

        inline bool resolve_day_of_year(year_t year, int day_of_year, int& month, int& day) noexcept {
            if (day_of_year < 1) {
                return false;
            }
            const bool is_leap = is_leap_year_date(year);
            const int max_day = is_leap ? 366 : 365;
            if (day_of_year > max_day) {
                return false;
            }

            static const int days_per_month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
            int remaining = day_of_year;
            month = 1;
            while (month <= 12) {
                int days = days_per_month[month - 1];
                if (month == 2 && is_leap) {
                    ++days;
                }
                if (remaining <= days) {
                    day = remaining;
                    return true;
                }
                remaining -= days;
                ++month;
            }
            return false;
        }

        inline int compute_day_of_year(year_t year, int month, int day) noexcept {
            static const int day_offsets[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
            int result = day_offsets[month - 1] + day;
            if (month > 2 && is_leap_year_date(year)) {
                ++result;
            }
            return result;
        }

        inline bool parse_compact_extended_year(const char*& p, const char* end, year_t& out) noexcept {
            const char* start = p;
            bool negative = false;
            if (p < end && (*p == '+' || *p == '-')) {
                negative = (*p == '-');
                ++p;
            }

            int64_t head = 0;
            if (!parse_unsigned_digits(p, end, 1, 18, head)) {
                p = start;
                return false;
            }

            int64_t year_value = 0;
            if (p < end && *p == 'M') {
                ++p;
                int64_t millennia = 0;
                if (!parse_unsigned_digits(p, end, 1, 18, millennia)) {
                    p = start;
                    return false;
                }
                int64_t tail = 0;
                if (p < end && *p == 'K') {
                    ++p;
                    if (!parse_unsigned_digits(p, end, 3, 3, tail)) {
                        p = start;
                        return false;
                    }
                    year_value = head * 1000000LL + millennia * 1000LL + tail;
                } else {
                    year_value = head * 1000000LL + millennia;
                }
            } else if (p < end && *p == 'K') {
                ++p;
                int64_t tail = 0;
                if (!parse_unsigned_digits(p, end, 3, 3, tail)) {
                    p = start;
                    return false;
                }
                year_value = head * 1000LL + tail;
            } else {
                year_value = head;
            }

            out = static_cast<year_t>(negative ? -year_value : year_value);
            return true;
        }

        inline bool parse_format_sequence(
                const char*& p,
                const char* end,
                const char* format_data,
                std::size_t format_size,
                FormatParseState& state) noexcept;

        inline bool parse_format_token(
                const char*& p,
                const char* end,
                char token,
                std::size_t repeat_count,
                FormatParseState& state) noexcept {
            int value = 0;
            int64_t wide_value = 0;
            switch (token) {
                case 'a':
                    return repeat_count == 1
                        && parse_weekday_token(p, end, SHORT_NAME, value)
                        && assign_int_field(state.has_weekday, state.weekday, value);
                case 'A':
                    return repeat_count == 1
                        && parse_weekday_token(p, end, FULL_NAME, value)
                        && assign_int_field(state.has_weekday, state.weekday, value);
                case 'b':
                    return repeat_count == 1
                        && parse_month_token(p, end, SHORT_NAME, value)
                        && assign_int_field(state.has_month, state.month, value);
                case 'B':
                    return repeat_count == 1
                        && parse_month_token(p, end, FULL_NAME, value)
                        && assign_int_field(state.has_month, state.month, value);
                case 'c':
                    return repeat_count == 1
                        && parse_format_sequence(p, end, "%a %b %e %H:%M:%S %Y", 20, state);
                case 'C':
                    if (repeat_count != 1 || !parse_unsigned_digits(p, end, 1, 6, wide_value)) {
                        return false;
                    }
                    return assign_int_field(state.has_century, state.century, static_cast<int>(wide_value));
                case 'd':
                    if (repeat_count >= 2 || !parse_exact_2digits(p, end, value)) {
                        return false;
                    }
                    return assign_int_field(state.has_day, state.day, value);
                case 'D':
                    if (repeat_count == 1) {
                        return parse_format_sequence(p, end, "%m/%d/%y", 8, state);
                    }
                    if (repeat_count == 2) {
                        if (!parse_exact_2digits(p, end, value)) {
                            return false;
                        }
                        return assign_int_field(state.has_day, state.day, value);
                    }
                    return false;
                case 'e':
                    if (repeat_count != 1 || !parse_space_padded_2digits(p, end, value)) {
                        return false;
                    }
                    return assign_int_field(state.has_day, state.day, value);
                case 'F':
                    return repeat_count == 1
                        && parse_format_sequence(p, end, "%Y-%m-%d", 8, state);
                case 'H':
                    if (repeat_count > 2 || !parse_exact_2digits(p, end, value)) {
                        return false;
                    }
                    return assign_int_field(state.has_hour24, state.hour24, value);
                case 'h':
                    if (repeat_count == 2) {
                        if (!parse_exact_2digits(p, end, value)) {
                            return false;
                        }
                        return assign_int_field(state.has_hour24, state.hour24, value);
                    }
                    return repeat_count == 1
                        && parse_month_token(p, end, SHORT_NAME, value)
                        && assign_int_field(state.has_month, state.month, value);
                case 'I':
                    if (repeat_count != 1 || !parse_exact_2digits(p, end, value)) {
                        return false;
                    }
                    return assign_int_field(state.has_hour12, state.hour12, value);
                case 'j':
                    if (repeat_count != 1 || !parse_unsigned_digits(p, end, 3, 3, wide_value)) {
                        return false;
                    }
                    return assign_int_field(state.has_day_of_year, state.day_of_year, static_cast<int>(wide_value));
                case 'k':
                    if (repeat_count != 1 || !parse_space_padded_2digits(p, end, value)) {
                        return false;
                    }
                    return assign_int_field(state.has_hour24, state.hour24, value);
                case 'l':
                    if (repeat_count != 1 || !parse_space_padded_2digits(p, end, value)) {
                        return false;
                    }
                    return assign_int_field(state.has_hour12, state.hour12, value);
                case 'm':
                    if (repeat_count == 1) {
                        if (!parse_exact_2digits(p, end, value)) {
                            return false;
                        }
                        return assign_int_field(state.has_month, state.month, value);
                    }
                    if (repeat_count == 2) {
                        if (!parse_exact_2digits(p, end, value)) {
                            return false;
                        }
                        return assign_int_field(state.has_minute, state.minute, value);
                    }
                    return false;
                case 'M':
                    if (repeat_count == 1) {
                        if (!parse_exact_2digits(p, end, value)) {
                            return false;
                        }
                        return assign_int_field(state.has_minute, state.minute, value);
                    }
                    if (repeat_count == 2) {
                        if (!parse_exact_2digits(p, end, value)) {
                            return false;
                        }
                        return assign_int_field(state.has_month, state.month, value);
                    }
                    if (repeat_count == 3) {
                        return parse_month_token(p, end, UPPERCASE_NAME, value)
                            && assign_int_field(state.has_month, state.month, value);
                    }
                    return false;
                case 'n':
                    return repeat_count == 1 && match_literal(p, end, '\n');
                case 'p':
                    if (repeat_count != 1 || !parse_meridiem(p, end, true, state.is_pm)) {
                        return false;
                    }
                    state.has_meridiem = true;
                    return true;
                case 'P':
                    if (repeat_count != 1 || !parse_meridiem(p, end, false, state.is_pm)) {
                        return false;
                    }
                    state.has_meridiem = true;
                    return true;
                case 'r':
                    return repeat_count == 1
                        && parse_format_sequence(p, end, "%I:%M:%S %p", 11, state);
                case 'R':
                    return repeat_count == 1
                        && parse_format_sequence(p, end, "%H:%M", 5, state);
                case 's':
                    if (repeat_count == 1) {
                        if (!parse_signed_digits(p, end, 1, 18, wide_value)) {
                            return false;
                        }
                        state.has_unix_seconds = true;
                        state.unix_seconds = static_cast<ts_t>(wide_value);
                        return true;
                    }
                    if (repeat_count == 3) {
                        if (!parse_unsigned_digits(p, end, 1, 3, wide_value)) {
                            return false;
                        }
                        return assign_int_field(state.has_millisecond, state.millisecond, static_cast<int>(wide_value));
                    }
                    return false;
                case 'S':
                    if (repeat_count <= 2) {
                        if (!parse_exact_2digits(p, end, value)) {
                            return false;
                        }
                        return assign_int_field(state.has_second, state.second, value);
                    }
                    if (repeat_count == 3) {
                        if (!parse_unsigned_digits(p, end, 1, 3, wide_value)) {
                            return false;
                        }
                        return assign_int_field(state.has_millisecond, state.millisecond, static_cast<int>(wide_value));
                    }
                    return false;
                case 't':
                    return repeat_count == 1 && match_literal(p, end, '\t');
                case 'T':
                    return repeat_count == 1
                        && parse_format_sequence(p, end, "%H:%M:%S", 8, state);
                case 'u':
                    if (repeat_count != 1 || !parse_unsigned_digits(p, end, 1, 1, wide_value)) {
                        return false;
                    }
                    return assign_int_field(state.has_iso_weekday, state.iso_weekday, static_cast<int>(wide_value));
                case 'w':
                    if (repeat_count == 1) {
                        if (!parse_unsigned_digits(p, end, 1, 1, wide_value)) {
                            return false;
                        }
                        return assign_int_field(state.has_weekday, state.weekday, static_cast<int>(wide_value));
                    }
                    if (repeat_count == 3) {
                        return parse_weekday_token(p, end, SHORT_NAME, value)
                            && assign_int_field(state.has_weekday, state.weekday, value);
                    }
                    return false;
                case 'W':
                    if (repeat_count == 3) {
                        return parse_weekday_token(p, end, UPPERCASE_NAME, value)
                            && assign_int_field(state.has_weekday, state.weekday, value);
                    }
                    return false;
                case 'y':
                    if (repeat_count != 1 || !parse_unsigned_digits(p, end, 1, 2, wide_value)) {
                        return false;
                    }
                    return assign_int_field(state.has_two_digit_year, state.two_digit_year, static_cast<int>(wide_value));
                case 'Y':
                    if (repeat_count == 1) {
                        if (!parse_signed_digits(p, end, 1, 18, wide_value)) {
                            return false;
                        }
                        return assign_year_field(state.has_year, state.year, static_cast<year_t>(wide_value));
                    }
                    if (repeat_count == 2) {
                        if (!parse_unsigned_digits(p, end, 2, 2, wide_value)) {
                            return false;
                        }
                        return assign_int_field(state.has_two_digit_year, state.two_digit_year, static_cast<int>(wide_value));
                    }
                    if (repeat_count == 4) {
                        if (!parse_signed_digits(p, end, 4, 4, wide_value)) {
                            return false;
                        }
                        return assign_year_field(state.has_year, state.year, static_cast<year_t>(wide_value));
                    }
                    if (repeat_count == 6) {
                        year_t parsed_year = 0;
                        if (!parse_compact_extended_year(p, end, parsed_year)) {
                            return false;
                        }
                        return assign_year_field(state.has_year, state.year, parsed_year);
                    }
                    return false;
                case 'z':
                    if (repeat_count != 1 || !parse_tz_compact(p, end, state.tz)) {
                        return false;
                    }
                    state.has_tz = true;
                    return true;
                case 'Z':
                    if (repeat_count != 1 || !match_literal(p, end, "UTC")) {
                        return false;
                    }
                    state.tz = create_time_zone_struct(0, 0, true);
                    state.has_tz = true;
                    return true;
                default:
                    return false;
            }
        }

        inline bool parse_format_sequence(
                const char*& p,
                const char* end,
                const char* format_data,
                std::size_t format_size,
                FormatParseState& state) noexcept {
            bool is_command = false;
            std::size_t repeat_count = 0;
            char last_char = 0;

            for (std::size_t i = 0; i < format_size; ++i) {
                const char current_char = format_data[i];
                if (!is_command) {
                    if (current_char == '%') {
                        ++repeat_count;
                        if (repeat_count == 2) {
                            if (!match_literal(p, end, '%')) {
                                return false;
                            }
                            repeat_count = 0;
                        }
                        continue;
                    }
                    if (!repeat_count) {
                        if (!match_literal(p, end, current_char)) {
                            return false;
                        }
                        continue;
                    }
                    last_char = current_char;
                    is_command = true;
                    continue;
                }
                if (last_char == current_char) {
                    ++repeat_count;
                    continue;
                }
                if (!parse_format_token(p, end, last_char, repeat_count, state)) {
                    return false;
                }
                repeat_count = 0;
                is_command = false;
                --i;
            }

            if (is_command) {
                if (!parse_format_token(p, end, last_char, repeat_count, state)) {
                    return false;
                }
            }
            return true;
        }

        inline bool validate_weekday_constraints(const FormatParseState& state, const DateTimeStruct& dt) noexcept {
            if (state.has_weekday && day_of_week_date<Weekday>(dt.year, dt.mon, dt.day) != state.weekday) {
                return false;
            }
            if (state.has_iso_weekday && iso_weekday_of_date(dt.year, dt.mon, dt.day) != state.iso_weekday) {
                return false;
            }
            return true;
        }

        inline bool finalize_calendar_state(FormatParseState& state, DateTimeStruct& out_dt, TimeZoneStruct& out_tz) noexcept {
            out_tz = state.has_tz ? state.tz : create_time_zone_struct(0, 0, true);

            year_t year = 0;
            if (state.has_year) {
                year = state.year;
                if (state.has_century && year / 100 != state.century) {
                    return false;
                }
                const int yy = static_cast<int>(year >= 0 ? (year % 100) : -(year % 100));
                if (state.has_two_digit_year && yy != state.two_digit_year) {
                    return false;
                }
            } else if (state.has_century || state.has_two_digit_year) {
                year = static_cast<year_t>((state.has_century ? state.century : 0) * 100
                    + (state.has_two_digit_year ? state.two_digit_year : 0));
            } else {
                return false;
            }

            int month = state.has_month ? state.month : 0;
            int day = state.has_day ? state.day : 0;
            if (state.has_day_of_year) {
                int resolved_month = 0;
                int resolved_day = 0;
                if (!resolve_day_of_year(year, state.day_of_year, resolved_month, resolved_day)) {
                    return false;
                }
                if (state.has_month && state.month != resolved_month) {
                    return false;
                }
                if (state.has_day && state.day != resolved_day) {
                    return false;
                }
                month = resolved_month;
                day = resolved_day;
            }
            if (month == 0 || day == 0) {
                return false;
            }

            int hour = 0;
            if (state.has_hour24) {
                hour = state.hour24;
                if (state.has_meridiem && state.has_hour12) {
                    int expected = state.hour12 % 12;
                    if (state.is_pm) {
                        expected += 12;
                    }
                    if (expected != hour) {
                        return false;
                    }
                }
            } else if (state.has_hour12) {
                if (!state.has_meridiem || state.hour12 < 1 || state.hour12 > 12) {
                    return false;
                }
                hour = state.hour12 % 12;
                if (state.is_pm) {
                    hour += 12;
                }
            }

            out_dt = create_date_time_struct(
                year,
                month,
                day,
                hour,
                state.has_minute ? state.minute : 0,
                state.has_second ? state.second : 0,
                state.has_millisecond ? state.millisecond : 0);
            if (!is_valid_date_time(out_dt)) {
                return false;
            }
            return validate_weekday_constraints(state, out_dt);
        }

        inline bool finalize_format_parse_state(
                FormatParseState& state,
                DateTimeStruct& out_dt,
                TimeZoneStruct& out_tz) noexcept {
            if (state.has_unix_seconds) {
                out_tz = state.has_tz ? state.tz : create_time_zone_struct(0, 0, true);
                const tz_t offset = time_zone_struct_to_offset(out_tz);
                const ts_t local_ts = state.has_tz ? static_cast<ts_t>(state.unix_seconds + offset)
                                                   : state.unix_seconds;
                out_dt = to_date_time<DateTimeStruct>(local_ts);
                if (state.has_millisecond) {
                    out_dt.ms = state.millisecond;
                }

                if (state.has_year && out_dt.year != state.year) return false;
                if (state.has_month && out_dt.mon != state.month) return false;
                if (state.has_day && out_dt.day != state.day) return false;
                if (state.has_hour24 && out_dt.hour != state.hour24) return false;
                if (state.has_minute && out_dt.min != state.minute) return false;
                if (state.has_second && out_dt.sec != state.second) return false;
                if (state.has_millisecond && out_dt.ms != state.millisecond) return false;
                if (state.has_day_of_year && compute_day_of_year(out_dt.year, out_dt.mon, out_dt.day) != state.day_of_year) return false;
                return validate_weekday_constraints(state, out_dt);
            }

            return finalize_calendar_state(state, out_dt, out_tz);
        }

        inline bool try_parse_format_core(
                const char* data,
                std::size_t length,
                const char* format,
                std::size_t format_length,
                DateTimeStruct& out_dt,
                TimeZoneStruct& out_tz) noexcept {
            if (!data || !format) {
                return false;
            }
            FormatParseState state = create_format_parse_state();
            const char* p = data;
            const char* end = data + length;
            if (!parse_format_sequence(p, end, format, format_length, state)) {
                return false;
            }
            if (p != end) {
                return false;
            }
            return finalize_format_parse_state(state, out_dt, out_tz);
        }

    } // namespace format_parse
    } // namespace detail

    /// \ingroup time_parsing
    /// \brief Parse input using formatter-compatible custom pattern.
    inline bool try_parse_format(
            const char* data,
            std::size_t length,
            const char* format,
            std::size_t format_length,
            DateTimeStruct& out_dt,
            TimeZoneStruct& out_tz) noexcept {
        return detail::format_parse::try_parse_format_core(data, length, format, format_length, out_dt, out_tz);
    }

    /// \ingroup time_parsing
    /// \brief Parse input using formatter-compatible custom pattern and convert to UTC seconds.
    inline bool try_parse_format_ts(
            const char* data,
            std::size_t length,
            const char* format,
            std::size_t format_length,
            ts_t& out_ts) noexcept {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!try_parse_format(data, length, format, format_length, dt, tz)) {
            out_ts = 0;
            return false;
        }
        try {
            out_ts = dt_to_timestamp(dt) - time_zone_struct_to_offset(tz);
            return true;
        } catch (...) {
            out_ts = 0;
            return false;
        }
    }

    /// \ingroup time_parsing
    /// \brief Parse input using formatter-compatible custom pattern and convert to UTC milliseconds.
    inline bool try_parse_format_ts_ms(
            const char* data,
            std::size_t length,
            const char* format,
            std::size_t format_length,
            ts_ms_t& out_ts) noexcept {
        DateTimeStruct dt;
        TimeZoneStruct tz;
        if (!try_parse_format(data, length, format, format_length, dt, tz)) {
            out_ts = 0;
            return false;
        }
        try {
            out_ts = dt_to_timestamp_ms(dt) - sec_to_ms<ts_ms_t, tz_t>(time_zone_struct_to_offset(tz));
            return true;
        } catch (...) {
            out_ts = 0;
            return false;
        }
    }

    /// \ingroup time_parsing
    /// \brief Parse std::string using formatter-compatible custom pattern.
    inline bool try_parse_format(
            const std::string& data,
            const std::string& format,
            DateTimeStruct& out_dt,
            TimeZoneStruct& out_tz) noexcept {
        return try_parse_format(data.data(), data.size(), format.data(), format.size(), out_dt, out_tz);
    }

    /// \ingroup time_parsing
    /// \brief Parse null-terminated strings using formatter-compatible custom pattern.
    inline bool try_parse_format(
            const char* data,
            const char* format,
            DateTimeStruct& out_dt,
            TimeZoneStruct& out_tz) noexcept {
        if (!data || !format) {
            return false;
        }
        return try_parse_format(data, std::strlen(data), format, std::strlen(format), out_dt, out_tz);
    }

    /// \ingroup time_parsing
    /// \brief Parse std::string using formatter-compatible custom pattern and convert to UTC seconds.
    inline bool try_parse_format_ts(
            const std::string& data,
            const std::string& format,
            ts_t& out_ts) noexcept {
        return try_parse_format_ts(data.data(), data.size(), format.data(), format.size(), out_ts);
    }

    /// \ingroup time_parsing
    /// \brief Parse null-terminated strings using custom format and convert to UTC seconds.
    inline bool try_parse_format_ts(
            const char* data,
            const char* format,
            ts_t& out_ts) noexcept {
        if (!data || !format) {
            out_ts = 0;
            return false;
        }
        return try_parse_format_ts(data, std::strlen(data), format, std::strlen(format), out_ts);
    }

    /// \ingroup time_parsing
    /// \brief Parse std::string using formatter-compatible custom pattern and convert to UTC milliseconds.
    inline bool try_parse_format_ts_ms(
            const std::string& data,
            const std::string& format,
            ts_ms_t& out_ts) noexcept {
        return try_parse_format_ts_ms(data.data(), data.size(), format.data(), format.size(), out_ts);
    }

    /// \ingroup time_parsing
    /// \brief Parse null-terminated strings using custom format and convert to UTC milliseconds.
    inline bool try_parse_format_ts_ms(
            const char* data,
            const char* format,
            ts_ms_t& out_ts) noexcept {
        if (!data || !format) {
            out_ts = 0;
            return false;
        }
        return try_parse_format_ts_ms(data, std::strlen(data), format, std::strlen(format), out_ts);
    }

#if __cplusplus >= 201703L
    /// \ingroup time_parsing
    /// \brief Parse std::string_view using formatter-compatible custom pattern.
    inline bool try_parse_format(
            std::string_view data,
            std::string_view format,
            DateTimeStruct& out_dt,
            TimeZoneStruct& out_tz) noexcept {
        return try_parse_format(data.data(), data.size(), format.data(), format.size(), out_dt, out_tz);
    }

    /// \ingroup time_parsing
    /// \brief Parse std::string_view using formatter-compatible custom pattern and convert to UTC seconds.
    inline bool try_parse_format_ts(
            std::string_view data,
            std::string_view format,
            ts_t& out_ts) noexcept {
        return try_parse_format_ts(data.data(), data.size(), format.data(), format.size(), out_ts);
    }

    /// \ingroup time_parsing
    /// \brief Parse std::string_view using formatter-compatible custom pattern and convert to UTC milliseconds.
    inline bool try_parse_format_ts_ms(
            std::string_view data,
            std::string_view format,
            ts_ms_t& out_ts) noexcept {
        return try_parse_format_ts_ms(data.data(), data.size(), format.data(), format.size(), out_ts);
    }
#endif

} // namespace time_shield

#endif // _TIME_SHIELD_TIME_FORMAT_PARSER_HPP_INCLUDED
