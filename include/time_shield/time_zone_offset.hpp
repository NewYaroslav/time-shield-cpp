// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_ZONE_OFFSET_HPP_INCLUDED
#define _TIME_SHIELD_TIME_ZONE_OFFSET_HPP_INCLUDED

/// \file time_zone_offset.hpp
/// \brief UTC offset arithmetic helpers (UTC <-> local) and TimeZoneStruct offset extraction.
/// \ingroup time_zone_conversions
///
/// This header provides simple, allocation-free conversions between:
/// - UTC timestamp and local timestamp using a numeric UTC offset (in seconds).
/// - UTC milliseconds and local milliseconds using the same offset.
///
/// \note The offset is interpreted as an UTC offset in seconds, i.e.:
///       local = utc + utc_offset
///       utc   = local - utc_offset
///
/// \note If the input equals ERROR_TIMESTAMP, the functions return ERROR_TIMESTAMP unchanged.

#include "config.hpp"
#include "types.hpp"
#include "time_conversions.hpp"
#include "time_zone_struct.hpp"

namespace time_shield {

    /// \ingroup time_conversions_time_zone_conversions
    /// \{

    /// \brief Convert local timestamp (seconds) to UTC using UTC offset.
    /// \param local Local timestamp in seconds.
    /// \param utc_offset UTC offset in seconds (e.g. CET=+3600, MSK=+10800, EST=-18000).
    /// \return UTC timestamp in seconds. If \p local equals ERROR_TIMESTAMP, returns ERROR_TIMESTAMP.
    TIME_SHIELD_CONSTEXPR inline ts_t to_utc(ts_t local, tz_t utc_offset) noexcept {
        return local == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                        : local - static_cast<ts_t>(utc_offset);
    }

    /// \brief Convert UTC timestamp (seconds) to local time using UTC offset.
    /// \param utc UTC timestamp in seconds.
    /// \param utc_offset UTC offset in seconds (e.g. CET=+3600, MSK=+10800, EST=-18000).
    /// \return Local timestamp in seconds. If \p utc equals ERROR_TIMESTAMP, returns ERROR_TIMESTAMP.
    TIME_SHIELD_CONSTEXPR inline ts_t to_local(ts_t utc, tz_t utc_offset) noexcept {
        return utc == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                      : utc + static_cast<ts_t>(utc_offset);
    }

    /// \brief Convert local timestamp (milliseconds) to UTC using UTC offset.
    /// \param local_ms Local timestamp in milliseconds.
    /// \param utc_offset UTC offset in seconds (will be converted to milliseconds).
    /// \return UTC timestamp in milliseconds. If \p local_ms equals ERROR_TIMESTAMP, returns ERROR_TIMESTAMP.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t to_utc_ms(ts_ms_t local_ms, tz_t utc_offset) noexcept {
        return local_ms == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                           : local_ms - sec_to_ms<ts_ms_t, tz_t>(utc_offset);
    }

    /// \brief Convert UTC timestamp (milliseconds) to local time using UTC offset.
    /// \param utc_ms UTC timestamp in milliseconds.
    /// \param utc_offset UTC offset in seconds (will be converted to milliseconds).
    /// \return Local timestamp in milliseconds. If \p utc_ms equals ERROR_TIMESTAMP, returns ERROR_TIMESTAMP.
    TIME_SHIELD_CONSTEXPR inline ts_ms_t to_local_ms(ts_ms_t utc_ms, tz_t utc_offset) noexcept {
        return utc_ms == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                         : utc_ms + sec_to_ms<ts_ms_t, tz_t>(utc_offset);
    }

    /// \brief Extract numeric UTC offset (in seconds) from TimeZoneStruct.
    /// \param tz Time zone descriptor.
    /// \return UTC offset in seconds (local = utc + offset).
    TIME_SHIELD_CONSTEXPR inline tz_t utc_offset_of(const TimeZoneStruct& tz) noexcept {
        return time_zone_struct_to_offset(tz);
    }

    /// \}

} // namespace time_shield

#endif
