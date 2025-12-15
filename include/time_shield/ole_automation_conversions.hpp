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
///
/// This header provides conversions between OA date and:
/// - Unix timestamps in seconds (ts_t)
/// - Unix timestamps in milliseconds (ts_ms_t)
/// - floating seconds (fts_t)

#include "config.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "time_conversions.hpp"

namespace time_shield {

    /// \brief Convert Unix timestamp (seconds) to OA date.
    /// \param ts Unix timestamp in seconds (may be negative).
    /// \return OA date value.
    TIME_SHIELD_CONSTEXPR inline oadate_t ts_to_oadate(ts_t ts) noexcept {
        return static_cast<oadate_t>(OLE_EPOCH)
             + static_cast<oadate_t>(ts) / static_cast<oadate_t>(SEC_PER_DAY);
    }
    
    /// \brief Convert Unix timestamp (floating seconds) to OA date.
    /// \param ts Unix timestamp in seconds as floating point (may be negative).
    /// \return OA date value.
    TIME_SHIELD_CONSTEXPR inline oadate_t fts_to_oadate(fts_t ts) noexcept {
        return static_cast<oadate_t>(OLE_EPOCH)
             + static_cast<oadate_t>(ts) / static_cast<oadate_t>(SEC_PER_DAY);
    }
    
    /// \brief Convert Unix timestamp (milliseconds) to OA date.
    /// \param ts_ms Unix timestamp in milliseconds (may be negative).
    /// \return OA date value.
    TIME_SHIELD_CONSTEXPR inline oadate_t ts_ms_to_oadate(ts_ms_t ts_ms) noexcept {
        return static_cast<oadate_t>(OLE_EPOCH)
             + static_cast<oadate_t>(ts_ms) / static_cast<oadate_t>(MS_PER_DAY);
    }
    
    /// \brief Convert OA date to Unix timestamp (seconds).
    /// \param oa OA date value.
    /// \return Unix timestamp in seconds (truncated toward zero).
    TIME_SHIELD_CONSTEXPR inline ts_t oadate_to_ts(oadate_t oa) noexcept {
        const oadate_t seconds = (oa - static_cast<oadate_t>(OLE_EPOCH))
                               * static_cast<oadate_t>(SEC_PER_DAY);
        return static_cast<ts_t>(seconds);
    }
    
    /// \brief Convert OA date to Unix timestamp (floating seconds).
    /// \param oa OA date value.
    /// \return Unix timestamp in seconds as floating point.
    TIME_SHIELD_CONSTEXPR inline fts_t oadate_to_fts(oadate_t oa) noexcept {
        return static_cast<fts_t>((oa - static_cast<oadate_t>(OLE_EPOCH))
             * static_cast<oadate_t>(SEC_PER_DAY));
    }
    
    /// \brief Convert OA date to Unix timestamp (milliseconds).
    /// \param oa OA date value.
    /// \return Unix timestamp in milliseconds (truncated toward zero).
    TIME_SHIELD_CONSTEXPR inline ts_ms_t oadate_to_ts_ms(oadate_t oa) noexcept {
        const oadate_t ms = (oa - static_cast<oadate_t>(OLE_EPOCH))
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
