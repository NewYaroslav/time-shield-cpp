// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_WORKDAY_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_WORKDAY_CONVERSIONS_HPP_INCLUDED

/// \file workday_conversions.hpp
/// \brief Helpers for computing workday-related timestamps.

#include "config.hpp"
#include "date_conversions.hpp"
#include "date_time_conversions.hpp"
#include "time_unit_conversions.hpp"
#include "validation.hpp"

namespace time_shield {

/// \ingroup time_conversions
/// \{

    /// \brief Finds the first workday number within a month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline int first_workday_day(year_t year, int month) noexcept {
        const int days = num_days_in_month(year, month);
        if (days <= 0) {
            return 0;
        }
        for (int day = 1; day <= days; ++day) {
            if (is_workday(year, month, day)) {
                return day;
            }
        }
        return 0;
    }

    /// \brief Finds the last workday number within a month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline int last_workday_day(year_t year, int month) noexcept {
        const int days = num_days_in_month(year, month);
        if (days <= 0) {
            return 0;
        }
        for (int day = days; day >= 1; --day) {
            if (is_workday(year, month, day)) {
                return day;
            }
        }
        return 0;
    }

    /// \brief Counts workdays within a month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline int count_workdays_in_month(year_t year, int month) noexcept {
        const int days = num_days_in_month(year, month);
        if (days <= 0) {
            return 0;
        }
        int total = 0;
        for (int day = 1; day <= days; ++day) {
            if (is_workday(year, month, day)) {
                ++total;
            }
        }
        return total;
    }

    /// \brief Returns workday position in month starting from 1.
    /// \param year Target year.
    /// \param month Target month (1-12).
    /// \param day Day of month (1-based).
    TIME_SHIELD_CONSTEXPR inline int workday_index_in_month(year_t year, int month, int day) noexcept {
        if (!is_workday(year, month, day)) {
            return 0;
        }
        const int days = num_days_in_month(year, month);
        if (days <= 0) {
            return 0;
        }
        int index = 0;
        for (int current = 1; current <= days; ++current) {
            if (is_workday(year, month, current)) {
                ++index;
                if (current == day) {
                    return index;
                }
            }
        }
        return 0;
    }

    /// \brief Checks whether date is the first workday of the month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    /// \param day Day of month (1-based).
    TIME_SHIELD_CONSTEXPR inline bool is_first_workday_of_month(year_t year, int month, int day) noexcept {
        return is_workday(year, month, day) && first_workday_day(year, month) == day;
    }

    /// \brief Checks if date falls within the first N workdays of the month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    /// \param day Day of month (1-based).
    /// \param count Number of leading workdays to include.
    TIME_SHIELD_CONSTEXPR inline bool is_within_first_workdays_of_month(year_t year, int month, int day, int count) noexcept {
        if (count <= 0) {
            return false;
        }
        const int total = count_workdays_in_month(year, month);
        if (count > total) {
            return false;
        }
        const int index = workday_index_in_month(year, month, day);
        return index > 0 && index <= count;
    }

    /// \brief Checks whether date is the last workday of the month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    /// \param day Day of month (1-based).
    TIME_SHIELD_CONSTEXPR inline bool is_last_workday_of_month(year_t year, int month, int day) noexcept {
        return is_workday(year, month, day) && last_workday_day(year, month) == day;
    }

    /// \brief Checks if date falls within the last N workdays of the month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    /// \param day Day of month (1-based).
    /// \param count Number of trailing workdays to include.
    TIME_SHIELD_CONSTEXPR inline bool is_within_last_workdays_of_month(year_t year, int month, int day, int count) noexcept {
        if (count <= 0) {
            return false;
        }
        const int total = count_workdays_in_month(year, month);
        if (count > total) {
            return false;
        }
        const int index = workday_index_in_month(year, month, day);
        return index > 0 && index >= (total - count + 1);
    }

    /// \brief Checks whether timestamp is the first workday of the month.
    /// \param ts Timestamp in seconds.
    TIME_SHIELD_CONSTEXPR inline bool is_first_workday_of_month(ts_t ts) noexcept {
        return is_first_workday_of_month(year_of<year_t>(ts), month_of_year<int>(ts), day_of_month<int>(ts));
    }

    /// \brief Checks whether millisecond timestamp is the first workday of the month.
    /// \param ts_ms Timestamp in milliseconds.
    TIME_SHIELD_CONSTEXPR inline bool is_first_workday_of_month_ms(ts_ms_t ts_ms) noexcept {
        return is_workday_ms(ts_ms) && is_first_workday_of_month(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)), day_of_month<int>(ms_to_sec<ts_t>(ts_ms)));
    }

    /// \brief Checks if timestamp falls within the first N workdays of the month.
    /// \param ts Timestamp in seconds.
    /// \param count Number of leading workdays to include.
    TIME_SHIELD_CONSTEXPR inline bool is_within_first_workdays_of_month(ts_t ts, int count) noexcept {
        return is_within_first_workdays_of_month(year_of<year_t>(ts), month_of_year<int>(ts), day_of_month<int>(ts), count);
    }

    /// \brief Checks if millisecond timestamp falls within the first N workdays of the month.
    /// \param ts_ms Timestamp in milliseconds.
    /// \param count Number of leading workdays to include.
    TIME_SHIELD_CONSTEXPR inline bool is_within_first_workdays_of_month_ms(ts_ms_t ts_ms, int count) noexcept {
        return is_workday_ms(ts_ms) && is_within_first_workdays_of_month(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)), day_of_month<int>(ms_to_sec<ts_t>(ts_ms)), count);
    }

    /// \brief Checks whether timestamp is the last workday of the month.
    /// \param ts Timestamp in seconds.
    TIME_SHIELD_CONSTEXPR inline bool is_last_workday_of_month(ts_t ts) noexcept {
        return is_last_workday_of_month(year_of<year_t>(ts), month_of_year<int>(ts), day_of_month<int>(ts));
    }

    /// \brief Checks whether millisecond timestamp is the last workday of the month.
    /// \param ts_ms Timestamp in milliseconds.
    TIME_SHIELD_CONSTEXPR inline bool is_last_workday_of_month_ms(ts_ms_t ts_ms) noexcept {
        return is_workday_ms(ts_ms) && is_last_workday_of_month(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)), day_of_month<int>(ms_to_sec<ts_t>(ts_ms)));
    }

    /// \brief Checks if timestamp falls within the last N workdays of the month.
    /// \param ts Timestamp in seconds.
    /// \param count Number of trailing workdays to include.
    TIME_SHIELD_CONSTEXPR inline bool is_within_last_workdays_of_month(ts_t ts, int count) noexcept {
        return is_within_last_workdays_of_month(year_of<year_t>(ts), month_of_year<int>(ts), day_of_month<int>(ts), count);
    }

    /// \brief Checks if millisecond timestamp falls within the last N workdays of the month.
    /// \param ts_ms Timestamp in milliseconds.
    /// \param count Number of trailing workdays to include.
    TIME_SHIELD_CONSTEXPR inline bool is_within_last_workdays_of_month_ms(ts_ms_t ts_ms, int count) noexcept {
        return is_workday_ms(ts_ms) && is_within_last_workdays_of_month(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)), day_of_month<int>(ms_to_sec<ts_t>(ts_ms)), count);
    }

    /// \brief Returns start-of-day timestamp for the first workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_first_workday_month(year_t year, int month) noexcept {
        const int day = first_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        return to_timestamp(year, month, day);
    }

    /// \brief Returns start-of-day millisecond timestamp for the first workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_ms_t start_of_first_workday_month_ms(year_t year, int month) noexcept {
        const int day = first_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        const ts_t day_start = to_timestamp(year, month, day);
        return sec_to_ms(day_start);
    }

    /// \brief Returns start-of-day timestamp for the first workday of month derived from timestamp.
    /// \param ts Timestamp in seconds.
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_first_workday_month(ts_t ts = time_shield::ts()) noexcept {
        return start_of_first_workday_month(year_of<year_t>(ts), month_of_year<int>(ts));
    }

    /// \brief Returns start-of-day millisecond timestamp for the first workday of month derived from millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t start_of_first_workday_month_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_first_workday_month_ms(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)));
    }

    /// \brief Returns end-of-day timestamp for the first workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_t end_of_first_workday_month(year_t year, int month) noexcept {
        const int day = first_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        const ts_t day_start = to_timestamp(year, month, day);
        return end_of_day(day_start);
    }

    /// \brief Returns end-of-day millisecond timestamp for the first workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_ms_t end_of_first_workday_month_ms(year_t year, int month) noexcept {
        const int day = first_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        const ts_t day_start = to_timestamp(year, month, day);
        const ts_ms_t day_start_ms = sec_to_ms(day_start);
        return end_of_day_ms(day_start_ms);
    }

    /// \brief Returns end-of-day timestamp for the first workday of month derived from timestamp.
    /// \param ts Timestamp in seconds.
    TIME_SHIELD_CONSTEXPR inline ts_t end_of_first_workday_month(ts_t ts = time_shield::ts()) noexcept {
        return end_of_first_workday_month(year_of<year_t>(ts), month_of_year<int>(ts));
    }

    /// \brief Returns end-of-day millisecond timestamp for the first workday of month derived from millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t end_of_first_workday_month_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_first_workday_month_ms(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)));
    }

    /// \brief Returns start-of-day timestamp for the last workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_last_workday_month(year_t year, int month) noexcept {
        const int day = last_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        return to_timestamp(year, month, day);
    }

    /// \brief Returns start-of-day millisecond timestamp for the last workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_ms_t start_of_last_workday_month_ms(year_t year, int month) noexcept {
        const int day = last_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        const ts_t day_start = to_timestamp(year, month, day);
        return sec_to_ms(day_start);
    }

    /// \brief Returns start-of-day timestamp for the last workday of month derived from timestamp.
    /// \param ts Timestamp in seconds.
    TIME_SHIELD_CONSTEXPR inline ts_t start_of_last_workday_month(ts_t ts = time_shield::ts()) noexcept {
        return start_of_last_workday_month(year_of<year_t>(ts), month_of_year<int>(ts));
    }

    /// \brief Returns start-of-day millisecond timestamp for the last workday of month derived from millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t start_of_last_workday_month_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_last_workday_month_ms(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)));
    }

    /// \brief Returns end-of-day timestamp for the last workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_t end_of_last_workday_month(year_t year, int month) noexcept {
        const int day = last_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        const ts_t day_start = to_timestamp(year, month, day);
        return end_of_day(day_start);
    }

    /// \brief Returns end-of-day millisecond timestamp for the last workday of month.
    /// \param year Target year.
    /// \param month Target month (1-12).
    TIME_SHIELD_CONSTEXPR inline ts_ms_t end_of_last_workday_month_ms(year_t year, int month) noexcept {
        const int day = last_workday_day(year, month);
        if (day <= 0) {
            return ERROR_TIMESTAMP;
        }
        const ts_t day_start = to_timestamp(year, month, day);
        const ts_ms_t day_start_ms = sec_to_ms(day_start);
        return end_of_day_ms(day_start_ms);
    }

    /// \brief Returns end-of-day timestamp for the last workday of month derived from timestamp.
    /// \param ts Timestamp in seconds.
    TIME_SHIELD_CONSTEXPR inline ts_t end_of_last_workday_month(ts_t ts = time_shield::ts()) noexcept {
        return end_of_last_workday_month(year_of<year_t>(ts), month_of_year<int>(ts));
    }

    /// \brief Returns end-of-day millisecond timestamp for the last workday of month derived from millisecond timestamp.
    /// \param ts_ms Timestamp in milliseconds.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t end_of_last_workday_month_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_last_workday_month_ms(year_of_ms(ts_ms), month_of_year<int>(ms_to_sec<ts_t>(ts_ms)));
    }

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_WORKDAY_CONVERSIONS_HPP_INCLUDED
