// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_DETAIL_FAST_DATE_HPP_INCLUDED
#define _TIME_SHIELD_DETAIL_FAST_DATE_HPP_INCLUDED

/// \file fast_date.hpp
/// \brief Fast date conversion helpers.

#include "mul_hi.hpp"

#include <cstdint>

namespace time_shield {
namespace detail {

    namespace {
        constexpr int16_t k_doy_from_march[12] = {
            0,   // Mar
            31,  // Apr
            61,  // May
            92,  // Jun
            122, // Jul
            153, // Aug
            184, // Sep
            214, // Oct
            245, // Nov
            275, // Dec
            306, // Jan
            337  // Feb
        };
    } // namespace

    struct DaySplit {
        int64_t days;
        int64_t sec_of_day;
    };

    /// \brief Split UNIX seconds into whole days and seconds-of-day.
    TIME_SHIELD_CONSTEXPR inline DaySplit split_unix_day(ts_t p_ts) noexcept {
        int64_t days = p_ts / SEC_PER_DAY;
        int64_t sec_of_day = p_ts % SEC_PER_DAY;
        if (sec_of_day < 0) {
            sec_of_day += SEC_PER_DAY;
            days -= 1;
        }
        return {days, sec_of_day};
    }

    struct FastDate {
        int64_t year;
        int month;
        int day;
    };

    /// \brief Convert date to days since Unix epoch using a fast constexpr algorithm.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    TIME_SHIELD_CONSTEXPR int64_t fast_days_from_date_constexpr(
            int64_t p_year,
            int p_month,
            int p_day) noexcept {
        const int month_adjust = (p_month <= 2 ? 1 : 0);
        const int64_t y = p_year - month_adjust;
        int m = p_month - 3;
        if (m < 0) {
            m += 12;
        }

        if (y >= 0) {
            const uint64_t y_u = static_cast<uint64_t>(y);
            const uint64_t era = y_u / 400U;
            const uint64_t yoe = y_u - era * 400U;
            const uint64_t doy = static_cast<uint64_t>(k_doy_from_march[m]) + static_cast<uint64_t>(p_day - 1);
            const uint64_t doe = yoe * 365U + yoe / 4U - yoe / 100U + doy;
            return static_cast<int64_t>(era * 146097U + doe) - 719468;
        }

        const int64_t era = (y - 399) / 400;
        const int64_t yoe = y - era * 400;
        const int64_t doy = static_cast<int64_t>(k_doy_from_march[m]) + static_cast<int64_t>(p_day - 1);
        const int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
        return era * 146097 + doe - 719468;
    }

    /// \brief Convert date to days since Unix epoch using a fast algorithm.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    inline int64_t fast_days_from_date(int64_t p_year, int p_month, int p_day) noexcept {
        const int month_adjust = (p_month <= 2 ? 1 : 0);
        const int64_t y = p_year - month_adjust;
        int m = p_month - 3;
        if (m < 0) {
            m += 12;
        }

        if (y >= 0) {
            const uint64_t y_u = static_cast<uint64_t>(y);
            const uint64_t era = y_u / 400U;
            const uint64_t yoe = y_u - era * 400U;
            const uint64_t doy = static_cast<uint64_t>(k_doy_from_march[m]) + static_cast<uint64_t>(p_day - 1);
            const uint64_t doe = yoe * 365U + yoe / 4U - yoe / 100U + doy;
            return static_cast<int64_t>(era * 146097U + doe) - 719468;
        }

        const int64_t era = (y - 399) / 400;
        const int64_t yoe = y - era * 400;
        const int64_t doy = static_cast<int64_t>(k_doy_from_march[m]) + static_cast<int64_t>(p_day - 1);
        const int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
        return era * 146097 + doe - 719468;
    }

    /// \brief Convert days since Unix epoch to date using a fast constexpr algorithm.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    TIME_SHIELD_CONSTEXPR FastDate fast_date_from_days_constexpr(int64_t p_days) noexcept {
        constexpr uint64_t ERAS = 4726498270ULL;
        constexpr int64_t D_SHIFT = static_cast<int64_t>(146097ULL * ERAS - 719469ULL);
        constexpr int64_t Y_SHIFT = static_cast<int64_t>(400ULL * ERAS - 1ULL);
        constexpr uint64_t C1 = 505054698555331ULL;
        constexpr uint64_t C2 = 50504432782230121ULL;
        constexpr uint64_t C3 = 8619973866219416ULL;
        constexpr uint64_t YPT_SCALE = 782432ULL;
        constexpr uint64_t YPT_BUMP_THRESHOLD = 126464ULL;
        constexpr uint64_t SHIFT_JAN_FEB = 191360ULL;
        constexpr uint64_t SHIFT_OTHER = 977792ULL;

        const uint64_t rev = static_cast<uint64_t>(D_SHIFT - p_days);
        const uint64_t cen = mul_shift_u64_constexpr(rev, C1);
        const uint64_t jul = rev + cen - (cen / 4U);

        const uint64_t num_hi = mul_shift_u64_constexpr(jul, C2);
        const uint64_t num_low = jul * C2;
        const uint64_t yrs = static_cast<uint64_t>(Y_SHIFT) - num_hi;

        const uint64_t ypt = mul_shift_u64_constexpr(YPT_SCALE, num_low);
        const bool bump = ypt < YPT_BUMP_THRESHOLD;
        const uint64_t shift = bump ? SHIFT_JAN_FEB : SHIFT_OTHER;

        const uint64_t N = (yrs & 3ULL) * 512ULL + shift - ypt;
        const uint64_t d = mul_shift_u64_constexpr((N & 0xFFFFULL), C3);

        return FastDate{
            static_cast<int64_t>(yrs + (bump ? 1U : 0U)),
            static_cast<int>(N >> 16),
            static_cast<int>(d + 1U)
        };
    }

    /// \brief Convert days since Unix epoch to date using a fast algorithm.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    inline FastDate fast_date_from_days(int64_t p_days) noexcept {
        constexpr uint64_t ERAS = 4726498270ULL;
        constexpr int64_t D_SHIFT = static_cast<int64_t>(146097ULL * ERAS - 719469ULL);
        constexpr int64_t Y_SHIFT = static_cast<int64_t>(400ULL * ERAS - 1ULL);
        constexpr uint64_t C1 = 505054698555331ULL;
        constexpr uint64_t C2 = 50504432782230121ULL;
        constexpr uint64_t C3 = 8619973866219416ULL;
        constexpr uint64_t YPT_SCALE = 782432ULL;
        constexpr uint64_t YPT_BUMP_THRESHOLD = 126464ULL;
        constexpr uint64_t SHIFT_JAN_FEB = 191360ULL;
        constexpr uint64_t SHIFT_OTHER = 977792ULL;

        const uint64_t rev = static_cast<uint64_t>(D_SHIFT - p_days);
        const uint64_t cen = mul_shift_u64(rev, C1);
        const uint64_t jul = rev + cen - (cen / 4U);

        const uint64_t num_hi = mul_shift_u64(jul, C2);
        const uint64_t num_low = jul * C2;
        const uint64_t yrs = static_cast<uint64_t>(Y_SHIFT) - num_hi;

        const uint64_t ypt = mul_shift_u64(YPT_SCALE, num_low);
        const bool bump = ypt < YPT_BUMP_THRESHOLD;
        const uint64_t shift = bump ? SHIFT_JAN_FEB : SHIFT_OTHER;

        const uint64_t N = (yrs & 3ULL) * 512ULL + shift - ypt;
        const uint64_t d = mul_shift_u64((N & 0xFFFFULL), C3);

        FastDate result{};
        result.day = static_cast<int>(d + 1U);
        result.month = static_cast<int>(N >> 16);
        result.year = static_cast<int64_t>(yrs + (bump ? 1U : 0U));
        return result;
    }

    /// \brief Convert days since Unix epoch to year using a fast constexpr algorithm.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    TIME_SHIELD_CONSTEXPR int64_t fast_year_from_days_constexpr(int64_t p_days) noexcept {
        constexpr uint64_t ERAS = 4726498270ULL;
        constexpr int64_t D_SHIFT = static_cast<int64_t>(146097ULL * ERAS - 719469ULL);
        constexpr int64_t Y_SHIFT = static_cast<int64_t>(400ULL * ERAS - 1ULL);
        constexpr uint64_t C1 = 505054698555331ULL;
        constexpr uint64_t C2 = 50504432782230121ULL;
        constexpr uint64_t YPT_SCALE = 782432ULL;
        constexpr uint64_t YPT_BUMP_THRESHOLD = 126464ULL;

        const uint64_t rev = static_cast<uint64_t>(D_SHIFT - p_days);
        const uint64_t cen = mul_shift_u64_constexpr(rev, C1);
        const uint64_t jul = rev + cen - (cen / 4U);

        const uint64_t num_hi = mul_shift_u64_constexpr(jul, C2);
        const uint64_t num_low = jul * C2;
        const uint64_t yrs = static_cast<uint64_t>(Y_SHIFT) - num_hi;

        const uint64_t ypt = mul_shift_u64_constexpr(YPT_SCALE, num_low);
        const bool bump = ypt < YPT_BUMP_THRESHOLD;
        return static_cast<int64_t>(yrs + (bump ? 1U : 0U));
    }

    /// \brief Convert days since Unix epoch to year using a fast algorithm.
    /// \note Inspired by the algorithm described in:
    ///       https://www.benjoffe.com/fast-date-64
    ///       This implementation is written from scratch (no code copied).
    inline int64_t fast_year_from_days(int64_t p_days) noexcept {
        constexpr uint64_t ERAS = 4726498270ULL;
        constexpr int64_t D_SHIFT = static_cast<int64_t>(146097ULL * ERAS - 719469ULL);
        constexpr int64_t Y_SHIFT = static_cast<int64_t>(400ULL * ERAS - 1ULL);
        constexpr uint64_t C1 = 505054698555331ULL;
        constexpr uint64_t C2 = 50504432782230121ULL;
        constexpr uint64_t YPT_SCALE = 782432ULL;
        constexpr uint64_t YPT_BUMP_THRESHOLD = 126464ULL;

        const uint64_t rev = static_cast<uint64_t>(D_SHIFT - p_days);
        const uint64_t cen = mul_shift_u64(rev, C1);
        const uint64_t jul = rev + cen - (cen / 4U);

        const uint64_t num_hi = mul_shift_u64(jul, C2);
        const uint64_t num_low = jul * C2;
        const uint64_t yrs = static_cast<uint64_t>(Y_SHIFT) - num_hi;

        const uint64_t ypt = mul_shift_u64(YPT_SCALE, num_low);
        const bool bump = ypt < YPT_BUMP_THRESHOLD;
        return static_cast<int64_t>(yrs + (bump ? 1U : 0U));
    }

} // namespace detail
} // namespace time_shield

#endif // _TIME_SHIELD_DETAIL_FAST_DATE_HPP_INCLUDED
