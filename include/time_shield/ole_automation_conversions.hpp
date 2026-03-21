// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_OLE_AUTOMATION_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_OLE_AUTOMATION_CONVERSIONS_HPP_INCLUDED

/// \file ole_automation_conversions.hpp
/// \brief OLE Automation Date (OA date) conversions.
/// \ingroup time_conversions
///
/// OA date is a floating-point day count where:
/// - 0.0 is 1899-12-30 00:00:00
/// - the integer part is days offset from that date
/// - the fractional part is time-of-day / 24
/// - negative fractional values follow Excel/COM serial semantics
///
/// This header provides conversions between OA date and:
/// - Unix timestamps in seconds (ts_t)
/// - Unix timestamps in milliseconds (ts_ms_t)
/// - floating seconds (fts_t)

#include "config.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "date_time_conversions.hpp"

#include <cstdint>
#include <limits>

namespace time_shield {

    namespace detail {

        TIME_SHIELD_CONSTEXPR inline bool oadate_can_cast_to_i64(oadate_t value) noexcept {
            return value >= static_cast<oadate_t>((std::numeric_limits<int64_t>::min)())
                && value <= static_cast<oadate_t>((std::numeric_limits<int64_t>::max)());
        }

        TIME_SHIELD_CONSTEXPR inline oadate_t oadate_abs(oadate_t value) noexcept {
            return value < 0 ? -value : value;
        }

        TIME_SHIELD_CONSTEXPR inline oadate_t oadate_trunc_toward_zero(oadate_t value) noexcept {
            if (!oadate_can_cast_to_i64(value)) {
                return value;
            }
            return static_cast<oadate_t>(static_cast<int64_t>(value));
        }

        TIME_SHIELD_CONSTEXPR inline oadate_t oadate_floor_value(oadate_t value) noexcept {
            const oadate_t truncated = oadate_trunc_toward_zero(value);
            if (truncated == value) {
                return truncated;
            }
            if (value < 0) {
                return truncated - static_cast<oadate_t>(1.0);
            }
            return truncated;
        }

        TIME_SHIELD_CONSTEXPR inline bool oadate_has_fraction(oadate_t value) noexcept {
            if (!oadate_can_cast_to_i64(value)) {
                return false;
            }
            return oadate_trunc_toward_zero(value) != value;
        }

        TIME_SHIELD_CONSTEXPR inline oadate_t linear_days_to_oadate(oadate_t linear_days) noexcept {
            if (linear_days < 0 && oadate_has_fraction(linear_days)) {
                const oadate_t whole_days = oadate_floor_value(linear_days);
                const oadate_t fraction = linear_days - whole_days;
                return whole_days - fraction;
            }
            return linear_days;
        }

        TIME_SHIELD_CONSTEXPR inline oadate_t oadate_to_linear_days(oadate_t oa) noexcept {
            if (oa < 0 && oadate_has_fraction(oa)) {
                const oadate_t whole_days = oadate_trunc_toward_zero(oa);
                const oadate_t fraction = oadate_abs(oa - whole_days);
                return whole_days + fraction;
            }
            return oa;
        }

    } // namespace detail

    /// \brief Convert Unix timestamp (seconds) to OA date.
    /// \param ts Unix timestamp in seconds (may be negative).
    /// \return OA date value.
    TIME_SHIELD_CONSTEXPR inline oadate_t ts_to_oadate(ts_t ts) noexcept {
        const oadate_t linear_days = static_cast<oadate_t>(OLE_EPOCH)
            + static_cast<oadate_t>(ts) / static_cast<oadate_t>(SEC_PER_DAY);
        return detail::linear_days_to_oadate(linear_days);
    }
    
    /// \brief Convert Unix timestamp (floating seconds) to OA date.
    /// \param ts Unix timestamp in seconds as floating point (may be negative).
    /// \return OA date value.
    TIME_SHIELD_CONSTEXPR inline oadate_t fts_to_oadate(fts_t ts) noexcept {
        const oadate_t linear_days = static_cast<oadate_t>(OLE_EPOCH)
            + static_cast<oadate_t>(ts) / static_cast<oadate_t>(SEC_PER_DAY);
        return detail::linear_days_to_oadate(linear_days);
    }
    
    /// \brief Convert Unix timestamp (milliseconds) to OA date.
    /// \param ts_ms Unix timestamp in milliseconds (may be negative).
    /// \return OA date value.
    TIME_SHIELD_CONSTEXPR inline oadate_t ts_ms_to_oadate(ts_ms_t ts_ms) noexcept {
        const oadate_t linear_days = static_cast<oadate_t>(OLE_EPOCH)
            + static_cast<oadate_t>(ts_ms) / static_cast<oadate_t>(MS_PER_DAY);
        return detail::linear_days_to_oadate(linear_days);
    }
    
    /// \brief Convert OA date to Unix timestamp (seconds).
    /// \param oa OA date value.
    /// \return Unix timestamp in seconds (truncated toward zero).
    TIME_SHIELD_CONSTEXPR inline ts_t oadate_to_ts(oadate_t oa) noexcept {
        const oadate_t linear_days = detail::oadate_to_linear_days(oa);
        const oadate_t seconds = (linear_days - static_cast<oadate_t>(OLE_EPOCH))
                               * static_cast<oadate_t>(SEC_PER_DAY);
        return static_cast<ts_t>(seconds);
    }
    
    /// \brief Convert OA date to Unix timestamp (floating seconds).
    /// \param oa OA date value.
    /// \return Unix timestamp in seconds as floating point.
    TIME_SHIELD_CONSTEXPR inline fts_t oadate_to_fts(oadate_t oa) noexcept {
        const oadate_t linear_days = detail::oadate_to_linear_days(oa);
        return static_cast<fts_t>((linear_days - static_cast<oadate_t>(OLE_EPOCH))
             * static_cast<oadate_t>(SEC_PER_DAY));
    }
    
    /// \brief Convert OA date to Unix timestamp (milliseconds).
    /// \param oa OA date value.
    /// \return Unix timestamp in milliseconds (truncated toward zero).
    TIME_SHIELD_CONSTEXPR inline ts_ms_t oadate_to_ts_ms(oadate_t oa) noexcept {
        const oadate_t linear_days = detail::oadate_to_linear_days(oa);
        const oadate_t ms = (linear_days - static_cast<oadate_t>(OLE_EPOCH))
                          * static_cast<oadate_t>(MS_PER_DAY);
        return static_cast<ts_ms_t>(ms);
    }
    
    /// \brief Build OA date from calendar components (Gregorian).
    /// \tparam T1 Year type.
    /// \tparam T2 Month/day/time components type.
    /// \tparam T3 Milliseconds type.
    /// \return OA date value.
    template<class T1, class T2, class T3>
    TIME_SHIELD_CONSTEXPR inline oadate_t to_oadate(
            T1 year, T2 month, T2 day,
            T2 hour = 0, T2 min = 0, T2 sec = 0, T3 ms = 0) noexcept {
        // Use existing conversion to floating timestamp (seconds).
        const fts_t fts = to_ftimestamp(year, month, day, hour, min, sec, ms);
        return fts_to_oadate(fts);
    }

} // namespace time_shield

#endif // _TIME_SHIELD_OLE_AUTOMATION_CONVERSIONS_HPP_INCLUDED
