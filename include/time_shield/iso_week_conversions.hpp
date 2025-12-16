// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_ISO_WEEK_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_ISO_WEEK_CONVERSIONS_HPP_INCLUDED

/// \file iso_week_conversions.hpp
/// \brief Conversions and utilities for ISO week dates (ISO 8601).
///
/// This file provides helpers to convert between calendar dates, timestamps, and ISO week dates,
/// as well as formatting and parsing helpers for ISO week-date strings.

#include "config.hpp"
#include "constants.hpp"
#include "date_struct.hpp"
#include "date_time_struct.hpp"
#include "iso_week_struct.hpp"
#include "time_conversions.hpp"
#include "unix_time_conversions.hpp"

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <inttypes.h>
#include <stdexcept>
#include <string>

namespace time_shield {

/// \ingroup time_conversions
/// \{

    /// \brief Convert Weekday enum to ISO weekday (Mon=1 .. Sun=7).
    /// \param weekday Weekday enum value.
    /// \return ISO weekday number.
    TIME_SHIELD_CONSTEXPR inline int iso_weekday_from_weekday(Weekday weekday) noexcept {
        return static_cast<int>((static_cast<int>(weekday) + DAYS_PER_WEEK - 1) % DAYS_PER_WEEK) + 1;
    }

    /// \brief Get ISO weekday for a calendar date.
    /// \param year Year component.
    /// \param month Month component.
    /// \param day Day component.
    /// \return ISO weekday number (1=Monday .. 7=Sunday).
    template<class Y, class M, class D>
    TIME_SHIELD_CONSTEXPR inline int iso_weekday_of_date(Y year, M month, D day) {
        return iso_weekday_from_weekday(day_of_week_date<Weekday>(year, month, day));
    }

    /// \brief Convert calendar date to ISO week date.
    /// \param year Year component.
    /// \param month Month component.
    /// \param day Day component.
    /// \return ISO week date representation.
    template<class Y, class M, class D>
    inline IsoWeekDateStruct to_iso_week_date(Y year, M month, D day) {
        const int iso_weekday = iso_weekday_of_date(year, month, day);
        const uday_t unix_day = date_to_unix_day(year, month, day);
        const uday_t thursday_day = unix_day + static_cast<uday_t>(4 - iso_weekday);

        const DateTimeStruct thursday_date = to_date_time<DateTimeStruct>(unix_day_to_ts(thursday_day));
        const year_t iso_year = thursday_date.year;

        const uday_t jan4_day = date_to_unix_day(iso_year, 1, 4);
        const int jan4_iso_weekday = iso_weekday_of_date(iso_year, 1, 4);
        const uday_t first_thursday = jan4_day + static_cast<uday_t>(4 - jan4_iso_weekday);

        const int32_t week = static_cast<int32_t>((thursday_day - first_thursday) / DAYS_PER_WEEK + 1);
        return create_iso_week_date_struct(iso_year, week, static_cast<int32_t>(iso_weekday));
    }

    /// \brief Convert DateStruct to ISO week date.
    /// \param date DateStruct instance.
    /// \return ISO week date representation.
    inline IsoWeekDateStruct to_iso_week_date(const DateStruct& date) {
        return to_iso_week_date(date.year, date.mon, date.day);
    }

    /// \brief Convert timestamp to ISO week date.
    /// \tparam T Timestamp type.
    /// \param ts Timestamp in seconds.
    /// \return ISO week date representation.
    template<class T = ts_t>
    inline IsoWeekDateStruct to_iso_week_date(T ts) {
        const DateTimeStruct date_time = to_date_time<DateTimeStruct, T>(ts);
        return to_iso_week_date(date_time.year, date_time.mon, date_time.day);
    }

    /// \brief Calculate number of ISO weeks in a year.
    /// \param iso_year ISO week-numbering year.
    /// \return 52 or 53 depending on the ISO year length.
    inline int iso_weeks_in_year(year_t iso_year) {
        const IsoWeekDateStruct info = to_iso_week_date(iso_year, 12, 28);
        return static_cast<int>(info.week);
    }

    /// \brief Validate ISO week date components.
    /// \param iso_year ISO week-numbering year.
    /// \param week ISO week number.
    /// \param weekday ISO weekday (1-7).
    /// \return True if components form a valid ISO week date.
    inline bool is_valid_iso_week_date(year_t iso_year, int week, int weekday) {
        if (weekday < 1 || weekday > 7) return false;
        if (week < 1) return false;
        const int max_week = iso_weeks_in_year(iso_year);
        return week <= max_week;
    }

    /// \brief Convert ISO week date to calendar date.
    /// \param iso_date ISO week date structure.
    /// \return Calendar date corresponding to the ISO week date.
    /// \throws std::invalid_argument if the ISO week date is invalid.
    inline DateStruct iso_week_date_to_date(const IsoWeekDateStruct& iso_date) {
        if (!is_valid_iso_week_date(iso_date.year, iso_date.week, iso_date.weekday)) {
            throw std::invalid_argument("Invalid ISO week date");
        }

        const uday_t jan4_day = date_to_unix_day(iso_date.year, 1, 4);
        const int jan4_iso_weekday = iso_weekday_of_date(iso_date.year, 1, 4);
        const uday_t first_thursday = jan4_day + static_cast<uday_t>(4 - jan4_iso_weekday);
        const uday_t target_thursday = first_thursday + static_cast<uday_t>(iso_date.week - 1) * DAYS_PER_WEEK;
        const uday_t target_day = target_thursday + static_cast<uday_t>(iso_date.weekday - 4);

        const DateTimeStruct date_time = to_date_time<DateTimeStruct>(unix_day_to_ts(target_day));
        return create_date_struct(date_time.year, date_time.mon, date_time.day);
    }

    /// \brief Format ISO week date to string.
    /// \param iso_date ISO week date to format.
    /// \param extended When true, uses extended format with separators ("YYYY-Www-D").
    /// \param include_weekday When false, omits weekday ("YYYY-Www"). Weekday defaults to Monday when omitted.
    /// \return Formatted ISO week-date string.
    inline std::string format_iso_week_date(const IsoWeekDateStruct& iso_date, bool extended = true, bool include_weekday = true) {
        if (!is_valid_iso_week_date(iso_date.year, iso_date.week, iso_date.weekday)) {
            throw std::invalid_argument("Invalid ISO week date");
        }

        if (!include_weekday) {
            const char* fmt = extended ? "%" PRId64 "-W%.2d" : "%" PRId64 "W%.2d";
            char buffer[32] = {0};
            std::snprintf(buffer, sizeof(buffer), fmt, iso_date.year, iso_date.week);
            return std::string(buffer);
        }

        const char* fmt = extended ? "%" PRId64 "-W%.2d-%d" : "%" PRId64 "W%.2d%d";
        char buffer[32] = {0};
        std::snprintf(buffer, sizeof(buffer), fmt, iso_date.year, iso_date.week, iso_date.weekday);
        return std::string(buffer);
    }

    /// \brief Parse ISO week date string.
    /// \param input Input string containing ISO week date.
    /// \param iso_date Output ISO week date structure.
    /// \return True if parsing succeeded and produced a valid ISO week date; otherwise false.
    inline bool parse_iso_week_date(const std::string& input, IsoWeekDateStruct& iso_date) noexcept {
        const char* p = input.c_str();
        const char* const end = p + input.size();

        auto parse_signed_year = [&]() -> bool {
            bool negative = false;
            if (p < end && (*p == '+' || *p == '-')) {
                negative = (*p == '-');
                ++p;
            }
            const char* start_digits = p;
            int64_t value = 0;
            while (p < end && std::isdigit(static_cast<unsigned char>(*p)) != 0) {
                value = value * 10 + static_cast<int64_t>(*p - '0');
                ++p;
            }
            if (p == start_digits) return false;
            iso_date.year = negative ? -value : value;
            return true;
        };

        if (!parse_signed_year()) return false;
        if (p >= end) return false;

        const bool has_dash_after_year = (*p == '-');
        if (has_dash_after_year) {
            ++p;
            if (p >= end) return false;
        }

        if (*p != 'W' && *p != 'w') return false;
        ++p;

        int week = 0;
        for (int i = 0; i < 2; ++i) {
            if (p >= end || std::isdigit(static_cast<unsigned char>(*p)) == 0) return false;
            week = week * 10 + (*p - '0');
            ++p;
        }

        if (week == 0) return false;

        bool has_weekday = false;
        if (p < end) {
            if ((*p == '-' && has_dash_after_year) || (!has_dash_after_year && std::isdigit(static_cast<unsigned char>(*p)) == 0)) {
                if (*p == '-') ++p;
                if (p >= end) return false;
                if (std::isdigit(static_cast<unsigned char>(*p)) == 0) return false;
                iso_date.weekday = *p - '0';
                ++p;
                has_weekday = true;
            } else if (std::isdigit(static_cast<unsigned char>(*p)) != 0) {
                iso_date.weekday = *p - '0';
                ++p;
                has_weekday = true;
            }
        }

        if (!has_weekday) {
            iso_date.weekday = 1;
        }

        iso_date.week = week;

        if (p != end) return false;
        return is_valid_iso_week_date(iso_date.year, iso_date.week, iso_date.weekday);
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_ISO_WEEK_CONVERSIONS_HPP_INCLUDED
