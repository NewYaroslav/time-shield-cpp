// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_HEADER_TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED
#define TIME_SHIELD_HEADER_TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED

/// \file time_zone_conversions.hpp
/// \brief Helpers for converting supported regional time zones and UTC.
/// \ingroup time_zone_conversions

#include "date_time_struct.hpp"
#include "time_conversions.hpp"
#include "time_zone_offset.hpp"
#include "time_unit_conversions.hpp"

namespace time_shield {

    /// \ingroup time_conversions_time_zone_conversions
    /// \{

    ts_t zone_to_gmt(ts_t local, TimeZone zone);
    ts_t gmt_to_zone(ts_t gmt, TimeZone zone);
    ts_ms_t zone_to_gmt_ms(ts_ms_t local_ms, TimeZone zone);
    ts_ms_t gmt_to_zone_ms(ts_ms_t gmt_ms, TimeZone zone);

    /// \brief Classification of a local civil timestamp in a time zone.
    enum class LocalTimeStatus {
        valid,       ///< Local time maps to exactly one UTC timestamp.
        nonexistent, ///< Local time falls into a DST forward gap.
        ambiguous,   ///< Local time maps to more than one UTC timestamp.
        unsupported  ///< Zone or timestamp cannot be resolved.
    };

    /// \brief Policy for ambiguous local civil timestamps.
    enum class AmbiguousTimePolicy {
        first_occurrence,  ///< Use the earliest UTC occurrence.
        second_occurrence, ///< Use the latest UTC occurrence.
        error              ///< Return ERROR_TIMESTAMP.
    };

    /// \brief Policy for nonexistent local civil timestamps.
    enum class NonexistentTimePolicy {
        error,          ///< Return ERROR_TIMESTAMP.
        shift_forward,  ///< Use the earliest valid local instant after the gap.
        shift_backward  ///< Use the latest valid local instant before the gap.
    };

    /// \brief Result of explicit local-time resolution.
    struct LocalTimeResolution {
        LocalTimeStatus status;
        ts_ms_t first_utc_ms;
        ts_ms_t second_utc_ms;
    };

    namespace detail {

        inline ts_t cet_to_gmt_impl(ts_t cet) {
            DateTimeStruct dt = to_date_time(cet);
            int max_days = num_days_in_month(dt.year, dt.mon);
            const int OLD_START_SUMMER_HOUR = 2;
            const int OLD_STOP_SUMMER_HOUR  = 3;
            const int NEW_SUMMER_HOUR       = 1;

            if(dt.year < 2002) {
                if(dt.mon > MAR && dt.mon < OCT) {
                    return cet - SEC_PER_HOUR * 2;
                } else if(dt.mon == MAR) {
                    for(int d = max_days; d >= dt.day; --d) {
                        if(day_of_week_date(dt.year, MAR, d) == SUN) {
                            if(d == dt.day) {
                                if(dt.hour >= OLD_START_SUMMER_HOUR) {
                                    return cet - SEC_PER_HOUR * 2;
                                }
                                return cet - SEC_PER_HOUR;
                            }
                            return cet - SEC_PER_HOUR;
                        }
                    }
                    return cet - SEC_PER_HOUR * 2;
                } else if(dt.mon == OCT) {
                    for(int d = max_days; d >= dt.day; --d) {
                        if(day_of_week_date(dt.year, OCT, d) == SUN) {
                            if(d == dt.day) {
                                if(dt.hour >= OLD_STOP_SUMMER_HOUR) {
                                    return cet - SEC_PER_HOUR;
                                }
                                return cet - SEC_PER_HOUR;
                            }
                            return cet - SEC_PER_HOUR * 2;
                        }
                    }
                    return cet - SEC_PER_HOUR;
                }
                return cet - SEC_PER_HOUR;
            }

            if(dt.mon > MAR && dt.mon < OCT) {
                return cet - SEC_PER_HOUR * 2;
            }
            if(dt.mon == MAR) {
                for(int d = max_days; d >= dt.day; --d) {
                    if(day_of_week_date(dt.year, MAR, d) == SUN) {
                        if(d == dt.day) {
                            if(dt.hour >= (NEW_SUMMER_HOUR + 2)) {
                                return cet - SEC_PER_HOUR * 2;
                            }
                            return cet - SEC_PER_HOUR;
                        }
                        return cet - SEC_PER_HOUR;
                    }
                }
                return cet - SEC_PER_HOUR * 2;
            }
            if(dt.mon == OCT) {
                for(int d = max_days; d >= dt.day; --d) {
                    if(day_of_week_date(dt.year, OCT, d) == SUN) {
                        if(d == dt.day) {
                            if(dt.hour >= (NEW_SUMMER_HOUR + 1)) {
                                return cet - SEC_PER_HOUR;
                            }
                            return cet - SEC_PER_HOUR * 2;
                        }
                        return cet - SEC_PER_HOUR * 2;
                    }
                }
            }
            return cet - SEC_PER_HOUR;
        }

        inline ts_t gmt_to_cet_impl(ts_t gmt) {
            DateTimeStruct dt = to_date_time(gmt);
            const int SWITCH_HOUR = 1;

            if(dt.mon > MAR && dt.mon < OCT) {
                return gmt + SEC_PER_HOUR * 2;
            }
            if(dt.mon == MAR) {
                int last = last_sunday_month_day(dt.year, MAR);
                if(dt.day > last) {
                    return gmt + SEC_PER_HOUR * 2;
                }
                if(dt.day < last) {
                    return gmt + SEC_PER_HOUR;
                }
                if(dt.hour >= SWITCH_HOUR) {
                    return gmt + SEC_PER_HOUR * 2;
                }
                return gmt + SEC_PER_HOUR;
            }
            if(dt.mon == OCT) {
                int last = last_sunday_month_day(dt.year, OCT);
                if(dt.day > last) {
                    return gmt + SEC_PER_HOUR;
                }
                if(dt.day < last) {
                    return gmt + SEC_PER_HOUR * 2;
                }
                if(dt.hour >= SWITCH_HOUR) {
                    return gmt + SEC_PER_HOUR;
                }
                return gmt + SEC_PER_HOUR * 2;
            }
            return gmt + SEC_PER_HOUR;
        }

        inline ts_t european_local_to_gmt(ts_t local, int standard_offset_hours) {
            return cet_to_gmt_impl(local - SEC_PER_HOUR * (standard_offset_hours - 1));
        }

        inline ts_t gmt_to_european_local(ts_t gmt, int standard_offset_hours) {
            return gmt_to_cet_impl(gmt) + SEC_PER_HOUR * (standard_offset_hours - 1);
        }

        inline bool is_us_eastern_dst_local(const DateTimeStruct& dt) {
            const int SWITCH_HOUR = 2;
            int start_day = 0;
            int end_day = 0;
            int start_month = 0;
            int end_month = 0;

            if(dt.year >= 2007) {
                start_month = MAR;
                end_month = NOV;
                int first_sunday_march = static_cast<int>(
                    1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, MAR, 1)) % DAYS_PER_WEEK);
                start_day = first_sunday_march + 7;
                end_day = static_cast<int>(
                    1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, NOV, 1)) % DAYS_PER_WEEK);
            } else {
                start_month = APR;
                end_month = OCT;
                start_day = static_cast<int>(
                    1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, APR, 1)) % DAYS_PER_WEEK);
                end_day = last_sunday_month_day(dt.year, OCT);
            }

            if(dt.mon > start_month && dt.mon < end_month) {
                return true;
            }
            if(dt.mon < start_month || dt.mon > end_month) {
                return false;
            }
            if(dt.mon == start_month) {
                if(dt.day > start_day) {
                    return true;
                }
                if(dt.day < start_day) {
                    return false;
                }
                return dt.hour >= SWITCH_HOUR;
            }
            if(dt.mon == end_month) {
                if(dt.day < end_day) {
                    return true;
                }
                if(dt.day > end_day) {
                    return false;
                }
                return dt.hour < SWITCH_HOUR;
            }
            return false;
        }

        inline bool fixed_zone_offset(TimeZone zone, tz_t& utc_offset) {
            switch(zone) {
                case GMT:
                case UTC:
                case WET:
                    utc_offset = 0;
                    return true;
                case WEST:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR);
                    return true;
                case CET:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR);
                    return true;
                case CEST:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 2);
                    return true;
                case EET:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 2);
                    return true;
                case EEST:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 3);
                    return true;
                case IST:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 5 + SEC_PER_MIN * 30);
                    return true;
                case MYT:
                case WITA:
                case SGT:
                case PHT:
                case HKT:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 8);
                    return true;
                case WIB:
                case ICT:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 7);
                    return true;
                case WIT:
                case JST:
                case KST:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 9);
                    return true;
                case KZT:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 5);
                    return true;
                case TRT:
                case BYT:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 3);
                    return true;
                case GST:
                    utc_offset = static_cast<tz_t>(SEC_PER_HOUR * 4);
                    return true;
                default:
                    utc_offset = 0;
                    return false;
            }
        }

        inline ts_ms_t zone_to_gmt_ms_by_seconds(ts_ms_t local_ms, TimeZone zone) {
            if(local_ms == ERROR_TIMESTAMP) {
                return ERROR_TIMESTAMP;
            }
            const ts_t local_sec = ms_to_sec<ts_t>(local_ms);
            const ts_ms_t remainder_ms = local_ms - sec_to_ms<ts_ms_t>(local_sec);
            const ts_t gmt_sec = zone_to_gmt(local_sec, zone);
            if(gmt_sec == ERROR_TIMESTAMP) {
                return ERROR_TIMESTAMP;
            }
            return sec_to_ms<ts_ms_t>(gmt_sec) + remainder_ms;
        }

        inline ts_ms_t gmt_to_zone_ms_by_seconds(ts_ms_t gmt_ms, TimeZone zone) {
            if(gmt_ms == ERROR_TIMESTAMP) {
                return ERROR_TIMESTAMP;
            }
            const ts_t gmt_sec = ms_to_sec<ts_t>(gmt_ms);
            const ts_ms_t remainder_ms = gmt_ms - sec_to_ms<ts_ms_t>(gmt_sec);
            const ts_t local_sec = gmt_to_zone(gmt_sec, zone);
            if(local_sec == ERROR_TIMESTAMP) {
                return ERROR_TIMESTAMP;
            }
            return sec_to_ms<ts_ms_t>(local_sec) + remainder_ms;
        }

    } // namespace detail

    /// \brief Convert Central European Time to Greenwich Mean Time.
    /// \param cet Timestamp in seconds in CET/CEST.
    /// \return Timestamp in seconds in GMT.
    inline ts_t cet_to_gmt(ts_t cet) {
        return detail::cet_to_gmt_impl(cet);
    }

    /// \brief Convert Eastern European Time to Greenwich Mean Time.
    /// \param eet Timestamp in seconds in EET/EEST.
    /// \return Timestamp in seconds in GMT.
    inline ts_t eet_to_gmt(ts_t eet) {
        return detail::european_local_to_gmt(eet, 2);
    }

    /// \brief Check if local US Eastern time uses DST.
    /// \param dt Local time in ET.
    /// \return True if DST applies for the provided local timestamp.
    inline bool is_us_eastern_dst_local(const DateTimeStruct& dt) {
        return detail::is_us_eastern_dst_local(dt);
    }

    /// \brief Convert US Eastern Time (New York, EST/EDT) to GMT (UTC).
    /// \param et Timestamp in seconds in ET.
    /// \return Timestamp in seconds in GMT (UTC).
    inline ts_t et_to_gmt(ts_t et) {
        DateTimeStruct dt = to_date_time(et);
        bool is_dst = detail::is_us_eastern_dst_local(dt);
        return et + SEC_PER_HOUR * (is_dst ? 4 : 5);
    }

    /// \brief Convert GMT (UTC) to US Eastern Time (New York, EST/EDT).
    /// \param gmt Timestamp in seconds in GMT (UTC).
    /// \return Timestamp in seconds in ET.
    inline ts_t gmt_to_et(ts_t gmt) {
        ts_t et_standard = gmt - SEC_PER_HOUR * 5;
        DateTimeStruct dt_local = to_date_time(et_standard);
        bool is_dst = detail::is_us_eastern_dst_local(dt_local);
        return gmt - SEC_PER_HOUR * (is_dst ? 4 : 5);
    }

    /// \brief Convert New York Time to GMT (UTC).
    /// \param ny Timestamp in seconds in ET.
    /// \return Timestamp in seconds in GMT (UTC).
    inline ts_t ny_to_gmt(ts_t ny) {
        return et_to_gmt(ny);
    }

    /// \brief Convert GMT (UTC) to New York Time.
    /// \param gmt Timestamp in seconds in GMT (UTC).
    /// \return Timestamp in seconds in ET.
    inline ts_t gmt_to_ny(ts_t gmt) {
        return gmt_to_et(gmt);
    }

    /// \brief Convert US Central Time (America/Chicago, CST/CDT) to GMT (UTC).
    /// \param ct Timestamp in seconds in CT.
    /// \return Timestamp in seconds in GMT (UTC).
    inline ts_t ct_to_gmt(ts_t ct) {
        return et_to_gmt(ct + SEC_PER_HOUR);
    }

    /// \brief Convert GMT (UTC) to US Central Time (America/Chicago, CST/CDT).
    /// \param gmt Timestamp in seconds in GMT (UTC).
    /// \return Timestamp in seconds in CT.
    inline ts_t gmt_to_ct(ts_t gmt) {
        return gmt_to_et(gmt) - SEC_PER_HOUR;
    }

    /// \brief Convert Greenwich Mean Time to Central European Time.
    /// \param gmt Timestamp in seconds in GMT.
    /// \return Timestamp in seconds in CET/CEST.
    inline ts_t gmt_to_cet(ts_t gmt) {
        return detail::gmt_to_cet_impl(gmt);
    }

    /// \brief Convert Greenwich Mean Time to Eastern European Time.
    /// \param gmt Timestamp in seconds in GMT.
    /// \return Timestamp in seconds in EET/EEST.
    inline ts_t gmt_to_eet(ts_t gmt) {
        return detail::gmt_to_european_local(gmt, 2);
    }

    /// \brief Convert supported local civil time to GMT (UTC).
    /// \param local Timestamp in seconds in the source time zone.
    /// \param zone Source time zone.
    /// \return Timestamp in seconds in GMT, or ERROR_TIMESTAMP for unsupported zones.
    inline ts_t zone_to_gmt(ts_t local, TimeZone zone) {
        if(local == ERROR_TIMESTAMP) {
            return ERROR_TIMESTAMP;
        }

        tz_t utc_offset = 0;
        switch(zone) {
            case GMT:
            case UTC:
                return local;
            case WET:
                return detail::european_local_to_gmt(local, 0);
            case CET:
                return cet_to_gmt(local);
            case EET:
                return eet_to_gmt(local);
            case WEST:
            case CEST:
            case EEST:
                detail::fixed_zone_offset(zone, utc_offset);
                return to_utc(local, utc_offset);
            case ET:
                return et_to_gmt(local);
            case CT:
                return ct_to_gmt(local);
            case UNKNOWN:
                return ERROR_TIMESTAMP;
            default:
                if(detail::fixed_zone_offset(zone, utc_offset)) {
                    return to_utc(local, utc_offset);
                }
                return ERROR_TIMESTAMP;
        }
    }

    /// \brief Convert GMT (UTC) to a supported local civil time zone.
    /// \param gmt Timestamp in seconds in GMT (UTC).
    /// \param zone Destination time zone.
    /// \return Timestamp in seconds in the destination time zone, or ERROR_TIMESTAMP for unsupported zones.
    inline ts_t gmt_to_zone(ts_t gmt, TimeZone zone) {
        if(gmt == ERROR_TIMESTAMP) {
            return ERROR_TIMESTAMP;
        }

        tz_t utc_offset = 0;
        switch(zone) {
            case GMT:
            case UTC:
                return gmt;
            case WET:
                return detail::gmt_to_european_local(gmt, 0);
            case CET:
                return gmt_to_cet(gmt);
            case EET:
                return gmt_to_eet(gmt);
            case WEST:
            case CEST:
            case EEST:
                detail::fixed_zone_offset(zone, utc_offset);
                return to_local(gmt, utc_offset);
            case ET:
                return gmt_to_et(gmt);
            case CT:
                return gmt_to_ct(gmt);
            case UNKNOWN:
                return ERROR_TIMESTAMP;
            default:
                if(detail::fixed_zone_offset(zone, utc_offset)) {
                    return to_local(gmt, utc_offset);
                }
                return ERROR_TIMESTAMP;
        }
    }

    /// \brief Convert a timestamp between two supported local civil time zones.
    /// \param local Timestamp in seconds in the source time zone.
    /// \param from Source time zone.
    /// \param to Destination time zone.
    /// \return Timestamp in seconds in the destination time zone, or ERROR_TIMESTAMP on failure.
    inline ts_t convert_time_zone(ts_t local, TimeZone from, TimeZone to) {
        ts_t gmt = zone_to_gmt(local, from);
        return gmt == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                      : gmt_to_zone(gmt, to);
    }

    /// \brief Convert supported local civil time in milliseconds to GMT (UTC).
    /// \param local_ms Timestamp in milliseconds in the source time zone.
    /// \param zone Source time zone.
    /// \return Timestamp in milliseconds in GMT, or ERROR_TIMESTAMP for unsupported zones.
    inline ts_ms_t zone_to_gmt_ms(ts_ms_t local_ms, TimeZone zone) {
        if(local_ms == ERROR_TIMESTAMP) {
            return ERROR_TIMESTAMP;
        }

        tz_t utc_offset = 0;
        switch(zone) {
            case GMT:
            case UTC:
                return local_ms;
            case WET:
            case CET:
            case EET:
            case ET:
            case CT:
                return detail::zone_to_gmt_ms_by_seconds(local_ms, zone);
            case WEST:
            case CEST:
            case EEST:
                detail::fixed_zone_offset(zone, utc_offset);
                return to_utc_ms(local_ms, utc_offset);
            case UNKNOWN:
                return ERROR_TIMESTAMP;
            default:
                if(detail::fixed_zone_offset(zone, utc_offset)) {
                    return to_utc_ms(local_ms, utc_offset);
                }
                return ERROR_TIMESTAMP;
        }
    }

    /// \brief Convert GMT (UTC) in milliseconds to a supported local civil time zone.
    /// \param gmt_ms Timestamp in milliseconds in GMT (UTC).
    /// \param zone Destination time zone.
    /// \return Timestamp in milliseconds in the destination time zone, or ERROR_TIMESTAMP for unsupported zones.
    inline ts_ms_t gmt_to_zone_ms(ts_ms_t gmt_ms, TimeZone zone) {
        if(gmt_ms == ERROR_TIMESTAMP) {
            return ERROR_TIMESTAMP;
        }

        tz_t utc_offset = 0;
        switch(zone) {
            case GMT:
            case UTC:
                return gmt_ms;
            case WET:
            case CET:
            case EET:
            case ET:
            case CT:
                return detail::gmt_to_zone_ms_by_seconds(gmt_ms, zone);
            case WEST:
            case CEST:
            case EEST:
                detail::fixed_zone_offset(zone, utc_offset);
                return to_local_ms(gmt_ms, utc_offset);
            case UNKNOWN:
                return ERROR_TIMESTAMP;
            default:
                if(detail::fixed_zone_offset(zone, utc_offset)) {
                    return to_local_ms(gmt_ms, utc_offset);
                }
                return ERROR_TIMESTAMP;
        }
    }

    /// \brief Convert a millisecond timestamp between two supported local civil time zones.
    /// \param local_ms Timestamp in milliseconds in the source time zone.
    /// \param from Source time zone.
    /// \param to Destination time zone.
    /// \return Timestamp in milliseconds in the destination time zone, or ERROR_TIMESTAMP on failure.
    inline ts_ms_t convert_time_zone_ms(ts_ms_t local_ms, TimeZone from, TimeZone to) {
        ts_ms_t gmt_ms = zone_to_gmt_ms(local_ms, from);
        return gmt_ms == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                         : gmt_to_zone_ms(gmt_ms, to);
    }

    namespace detail {

        inline LocalTimeResolution make_local_time_resolution(
            LocalTimeStatus status,
            ts_ms_t first_utc_ms = ERROR_TIMESTAMP,
            ts_ms_t second_utc_ms = ERROR_TIMESTAMP) {
            LocalTimeResolution result = {status, first_utc_ms, second_utc_ms};
            return result;
        }

        inline bool dynamic_dst_zone_offsets(TimeZone zone,
                                             tz_t& first_offset,
                                             tz_t& second_offset) {
            switch(zone) {
                case WET:
                    first_offset = 0;
                    second_offset = static_cast<tz_t>(SEC_PER_HOUR);
                    return true;
                case CET:
                    first_offset = static_cast<tz_t>(SEC_PER_HOUR);
                    second_offset = static_cast<tz_t>(SEC_PER_HOUR * 2);
                    return true;
                case EET:
                    first_offset = static_cast<tz_t>(SEC_PER_HOUR * 2);
                    second_offset = static_cast<tz_t>(SEC_PER_HOUR * 3);
                    return true;
                case ET:
                    first_offset = static_cast<tz_t>(-SEC_PER_HOUR * 5);
                    second_offset = static_cast<tz_t>(-SEC_PER_HOUR * 4);
                    return true;
                case CT:
                    first_offset = static_cast<tz_t>(-SEC_PER_HOUR * 6);
                    second_offset = static_cast<tz_t>(-SEC_PER_HOUR * 5);
                    return true;
                default:
                    first_offset = 0;
                    second_offset = 0;
                    return false;
            }
        }

        inline tz_t dynamic_zone_standard_offset(TimeZone zone) {
            switch(zone) {
                case WET:
                    return 0;
                case CET:
                    return static_cast<tz_t>(SEC_PER_HOUR);
                case EET:
                    return static_cast<tz_t>(SEC_PER_HOUR * 2);
                case ET:
                    return static_cast<tz_t>(-SEC_PER_HOUR * 5);
                case CT:
                    return static_cast<tz_t>(-SEC_PER_HOUR * 6);
                default:
                    return 0;
            }
        }

        inline bool is_european_dynamic_zone(TimeZone zone) {
            return zone == WET || zone == CET || zone == EET;
        }

        inline bool is_us_dynamic_zone(TimeZone zone) {
            return zone == ET || zone == CT;
        }

        inline bool european_dst_at_utc_ms(ts_ms_t utc_ms) {
            const DateTimeStruct dt = to_date_time(ms_to_sec<ts_t>(utc_ms));
            const int start_day = last_sunday_month_day(dt.year, MAR);
            const int end_day = last_sunday_month_day(dt.year, OCT);
            const ts_ms_t start_ms =
                to_timestamp_ms(dt.year, int(MAR), start_day, 1, 0, 0);
            const ts_ms_t end_ms =
                to_timestamp_ms(dt.year, int(OCT), end_day, 1, 0, 0);
            return utc_ms >= start_ms && utc_ms < end_ms;
        }

        inline int first_sunday_month_day(int year, int month) {
            return static_cast<int>(
                1 + (DAYS_PER_WEEK - day_of_week_date(year, month, 1)) %
                        DAYS_PER_WEEK);
        }

        inline bool us_dst_at_utc_ms(TimeZone zone, ts_ms_t utc_ms) {
            const DateTimeStruct dt = to_date_time(ms_to_sec<ts_t>(utc_ms));
            const int year = static_cast<int>(dt.year);
            int start_month = MAR;
            int end_month = NOV;
            int start_day = first_sunday_month_day(year, MAR) + 7;
            int end_day = first_sunday_month_day(year, NOV);

            if(dt.year < 2007) {
                start_month = APR;
                end_month = OCT;
                start_day = first_sunday_month_day(year, APR);
                end_day = last_sunday_month_day(year, OCT);
            }

            const tz_t standard_offset = dynamic_zone_standard_offset(zone);
            const tz_t daylight_offset =
                static_cast<tz_t>(standard_offset + SEC_PER_HOUR);
            const ts_ms_t start_local =
                to_timestamp_ms(dt.year, start_month, start_day, 2, 0, 0);
            const ts_ms_t end_local =
                to_timestamp_ms(dt.year, end_month, end_day, 2, 0, 0);
            const ts_ms_t start_utc = to_utc_ms(start_local, standard_offset);
            const ts_ms_t end_utc = to_utc_ms(end_local, daylight_offset);
            return utc_ms >= start_utc && utc_ms < end_utc;
        }

        inline bool dynamic_offset_applies_at_utc_ms(TimeZone zone,
                                                     ts_ms_t utc_ms,
                                                     tz_t offset) {
            if(is_european_dynamic_zone(zone)) {
                const tz_t standard_offset = dynamic_zone_standard_offset(zone);
                const tz_t expected =
                    static_cast<tz_t>(standard_offset +
                                      (european_dst_at_utc_ms(utc_ms)
                                           ? SEC_PER_HOUR
                                           : 0));
                return offset == expected;
            }

            if(is_us_dynamic_zone(zone)) {
                const tz_t standard_offset = dynamic_zone_standard_offset(zone);
                const tz_t expected =
                    static_cast<tz_t>(standard_offset +
                                      (us_dst_at_utc_ms(zone, utc_ms)
                                           ? SEC_PER_HOUR
                                           : 0));
                return offset == expected;
            }

            return false;
        }

        inline void add_local_time_candidate(LocalTimeResolution& result,
                                             ts_ms_t local_ms,
                                             TimeZone zone,
                                             tz_t offset) {
            const ts_ms_t candidate = to_utc_ms(local_ms, offset);
            if(candidate == ERROR_TIMESTAMP ||
               to_local_ms(candidate, offset) != local_ms ||
               !dynamic_offset_applies_at_utc_ms(zone, candidate, offset) ||
               result.first_utc_ms == candidate ||
               result.second_utc_ms == candidate) {
                return;
            }

            if(result.first_utc_ms == ERROR_TIMESTAMP) {
                result.first_utc_ms = candidate;
                return;
            }

            if(result.second_utc_ms == ERROR_TIMESTAMP) {
                result.second_utc_ms = candidate;
            }
        }

        inline LocalTimeResolution resolve_with_dynamic_offsets(
            ts_ms_t local_ms,
            TimeZone zone,
            tz_t first_offset,
            tz_t second_offset) {
            LocalTimeResolution result =
                make_local_time_resolution(LocalTimeStatus::nonexistent);

            add_local_time_candidate(result, local_ms, zone, first_offset);
            add_local_time_candidate(result, local_ms, zone, second_offset);

            if(result.first_utc_ms == ERROR_TIMESTAMP) {
                return result;
            }

            if(result.second_utc_ms == ERROR_TIMESTAMP) {
                result.status = LocalTimeStatus::valid;
                return result;
            }

            if(result.second_utc_ms < result.first_utc_ms) {
                const ts_ms_t tmp = result.first_utc_ms;
                result.first_utc_ms = result.second_utc_ms;
                result.second_utc_ms = tmp;
            }

            result.status = LocalTimeStatus::ambiguous;
            return result;
        }

        inline bool local_time_status_has_utc(LocalTimeStatus status) {
            return status == LocalTimeStatus::valid ||
                   status == LocalTimeStatus::ambiguous;
        }

        inline ts_ms_t local_time_resolution_to_utc(
            const LocalTimeResolution& resolution,
            AmbiguousTimePolicy ambiguous_policy) {
            if(resolution.status == LocalTimeStatus::valid) {
                return resolution.first_utc_ms;
            }

            if(resolution.status == LocalTimeStatus::ambiguous) {
                switch(ambiguous_policy) {
                    case AmbiguousTimePolicy::first_occurrence:
                        return resolution.first_utc_ms;
                    case AmbiguousTimePolicy::second_occurrence:
                        return resolution.second_utc_ms;
                    case AmbiguousTimePolicy::error:
                    default:
                        return ERROR_TIMESTAMP;
                }
            }

            return ERROR_TIMESTAMP;
        }

    } // namespace detail

    /// \brief Resolve the effective UTC offset for a UTC millisecond instant.
    /// \param utc_ms UTC timestamp in milliseconds.
    /// \param zone Time zone to inspect.
    /// \param out Receives offset in seconds on success.
    /// \return True when the zone and timestamp can be resolved.
    inline bool zone_offset_at_utc_ms(ts_ms_t utc_ms,
                                      TimeZone zone,
                                      tz_t& out) noexcept {
        if(utc_ms == ERROR_TIMESTAMP || zone == UNKNOWN) {
            return false;
        }

        if(zone == GMT || zone == UTC) {
            out = 0;
            return true;
        }

        if(detail::is_european_dynamic_zone(zone)) {
            const tz_t standard_offset = detail::dynamic_zone_standard_offset(zone);
            out = static_cast<tz_t>(standard_offset +
                                    (detail::european_dst_at_utc_ms(utc_ms)
                                         ? SEC_PER_HOUR
                                         : 0));
            return true;
        }

        if(detail::is_us_dynamic_zone(zone)) {
            const tz_t standard_offset = detail::dynamic_zone_standard_offset(zone);
            out = static_cast<tz_t>(standard_offset +
                                    (detail::us_dst_at_utc_ms(zone, utc_ms)
                                         ? SEC_PER_HOUR
                                         : 0));
            return true;
        }

        tz_t utc_offset = 0;
        if(detail::fixed_zone_offset(zone, utc_offset)) {
            out = utc_offset;
            return true;
        }

        return false;
    }

    /// \brief Resolve the effective UTC offset for a UTC second instant.
    /// \param utc UTC timestamp in seconds.
    /// \param zone Time zone to inspect.
    /// \param out Receives offset in seconds on success.
    /// \return True when the zone and timestamp can be resolved.
    inline bool zone_offset_at_utc(ts_t utc,
                                   TimeZone zone,
                                   tz_t& out) noexcept {
        return utc == ERROR_TIMESTAMP
                   ? false
                   : zone_offset_at_utc_ms(sec_to_ms<ts_ms_t>(utc), zone, out);
    }

    /// \brief Resolve local civil time to zero, one, or two UTC candidates.
    /// \param local_ms Local civil timestamp in milliseconds.
    /// \param zone Source time zone.
    /// \return Resolution status plus UTC candidates in milliseconds.
    inline LocalTimeResolution resolve_local_time_ms(ts_ms_t local_ms,
                                                     TimeZone zone) {
        if(local_ms == ERROR_TIMESTAMP || zone == UNKNOWN) {
            return detail::make_local_time_resolution(
                LocalTimeStatus::unsupported);
        }

        if(zone == GMT || zone == UTC) {
            return detail::make_local_time_resolution(LocalTimeStatus::valid,
                                                      local_ms);
        }

        tz_t first_offset = 0;
        tz_t second_offset = 0;
        if(detail::dynamic_dst_zone_offsets(zone, first_offset, second_offset)) {
            return detail::resolve_with_dynamic_offsets(local_ms,
                                                        zone,
                                                        first_offset,
                                                        second_offset);
        }

        tz_t utc_offset = 0;
        if(detail::fixed_zone_offset(zone, utc_offset)) {
            return detail::make_local_time_resolution(
                LocalTimeStatus::valid,
                to_utc_ms(local_ms, utc_offset));
        }

        return detail::make_local_time_resolution(LocalTimeStatus::unsupported);
    }

    /// \brief Resolve local civil time given in seconds.
    ///
    /// UTC candidates are returned in milliseconds in LocalTimeResolution.
    inline LocalTimeResolution resolve_local_time(ts_t local, TimeZone zone) {
        return local == ERROR_TIMESTAMP
                   ? detail::make_local_time_resolution(
                         LocalTimeStatus::unsupported)
                   : resolve_local_time_ms(sec_to_ms<ts_ms_t>(local), zone);
    }

    namespace detail {

        inline ts_ms_t shift_nonexistent_local_time_ms(ts_ms_t local_ms,
                                                       TimeZone zone,
                                                       int direction) {
            const ts_ms_t window = static_cast<ts_ms_t>(MS_PER_DAY) * 2;
            const bool forward = direction >= 0;
            ts_ms_t low = forward ? local_ms : local_ms - window;
            ts_ms_t high = forward ? local_ms + window : local_ms;

            LocalTimeResolution edge =
                resolve_local_time_ms(forward ? high : low, zone);
            if(!local_time_status_has_utc(edge.status)) {
                return ERROR_TIMESTAMP;
            }

            while(high - low > 1) {
                const ts_ms_t mid = low + (high - low) / 2;
                const LocalTimeResolution resolution =
                    resolve_local_time_ms(mid, zone);
                if(local_time_status_has_utc(resolution.status)) {
                    if(forward) {
                        high = mid;
                    } else {
                        low = mid;
                    }
                } else if(forward) {
                    low = mid;
                } else {
                    high = mid;
                }
            }

            const LocalTimeResolution shifted =
                resolve_local_time_ms(forward ? high : low, zone);
            return local_time_resolution_to_utc(
                shifted,
                AmbiguousTimePolicy::first_occurrence);
        }

    } // namespace detail

    /// \brief Convert local civil time to UTC with explicit DST policies.
    inline ts_ms_t zone_to_gmt_ms(ts_ms_t local_ms,
                                  TimeZone zone,
                                  AmbiguousTimePolicy ambiguous_policy,
                                  NonexistentTimePolicy nonexistent_policy) {
        const LocalTimeResolution resolution =
            resolve_local_time_ms(local_ms, zone);

        if(resolution.status == LocalTimeStatus::nonexistent) {
            switch(nonexistent_policy) {
                case NonexistentTimePolicy::shift_forward:
                    return detail::shift_nonexistent_local_time_ms(
                        local_ms,
                        zone,
                        1);
                case NonexistentTimePolicy::shift_backward:
                    return detail::shift_nonexistent_local_time_ms(
                        local_ms,
                        zone,
                        -1);
                case NonexistentTimePolicy::error:
                default:
                    return ERROR_TIMESTAMP;
            }
        }

        return detail::local_time_resolution_to_utc(resolution,
                                                    ambiguous_policy);
    }

    /// \brief Convert local civil time in seconds to UTC with explicit DST policies.
    inline ts_t zone_to_gmt(ts_t local,
                            TimeZone zone,
                            AmbiguousTimePolicy ambiguous_policy,
                            NonexistentTimePolicy nonexistent_policy) {
        const ts_ms_t utc_ms = local == ERROR_TIMESTAMP
                                   ? ERROR_TIMESTAMP
                                   : zone_to_gmt_ms(sec_to_ms<ts_ms_t>(local),
                                                    zone,
                                                    ambiguous_policy,
                                                    nonexistent_policy);
        return utc_ms == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                         : ms_to_sec<ts_t>(utc_ms);
    }

    /// \brief Convert only unambiguous existing local time to UTC.
    inline ts_ms_t zone_to_gmt_ms_strict(ts_ms_t local_ms, TimeZone zone) {
        return zone_to_gmt_ms(local_ms,
                              zone,
                              AmbiguousTimePolicy::error,
                              NonexistentTimePolicy::error);
    }

    /// \brief Convert only unambiguous existing local time in seconds to UTC.
    inline ts_t zone_to_gmt_strict(ts_t local, TimeZone zone) {
        return zone_to_gmt(local,
                           zone,
                           AmbiguousTimePolicy::error,
                           NonexistentTimePolicy::error);
    }

    /// \brief Convert local civil time between zones with explicit DST policies.
    inline ts_ms_t convert_time_zone_ms(
        ts_ms_t local_ms,
        TimeZone from,
        TimeZone to,
        AmbiguousTimePolicy ambiguous_policy,
        NonexistentTimePolicy nonexistent_policy) {
        const ts_ms_t gmt_ms = zone_to_gmt_ms(local_ms,
                                              from,
                                              ambiguous_policy,
                                              nonexistent_policy);
        return gmt_ms == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                         : gmt_to_zone_ms(gmt_ms, to);
    }

    /// \brief Convert local civil time in seconds between zones with explicit DST policies.
    inline ts_t convert_time_zone(
        ts_t local,
        TimeZone from,
        TimeZone to,
        AmbiguousTimePolicy ambiguous_policy,
        NonexistentTimePolicy nonexistent_policy) {
        const ts_t gmt = zone_to_gmt(local,
                                     from,
                                     ambiguous_policy,
                                     nonexistent_policy);
        return gmt == ERROR_TIMESTAMP ? ERROR_TIMESTAMP
                                      : gmt_to_zone(gmt, to);
    }

    inline ts_t ist_to_gmt(ts_t ist) { return zone_to_gmt(ist, IST); }
    inline ts_t gmt_to_ist(ts_t gmt) { return gmt_to_zone(gmt, IST); }

    inline ts_t myt_to_gmt(ts_t myt) { return zone_to_gmt(myt, MYT); }
    inline ts_t gmt_to_myt(ts_t gmt) { return gmt_to_zone(gmt, MYT); }

    inline ts_t wib_to_gmt(ts_t wib) { return zone_to_gmt(wib, WIB); }
    inline ts_t gmt_to_wib(ts_t gmt) { return gmt_to_zone(gmt, WIB); }

    inline ts_t wita_to_gmt(ts_t wita) { return zone_to_gmt(wita, WITA); }
    inline ts_t gmt_to_wita(ts_t gmt) { return gmt_to_zone(gmt, WITA); }

    inline ts_t wit_to_gmt(ts_t wit) { return zone_to_gmt(wit, WIT); }
    inline ts_t gmt_to_wit(ts_t gmt) { return gmt_to_zone(gmt, WIT); }

    inline ts_t kzt_to_gmt(ts_t kzt) { return zone_to_gmt(kzt, KZT); }
    inline ts_t gmt_to_kzt(ts_t gmt) { return gmt_to_zone(gmt, KZT); }

    inline ts_t trt_to_gmt(ts_t trt) { return zone_to_gmt(trt, TRT); }
    inline ts_t gmt_to_trt(ts_t gmt) { return gmt_to_zone(gmt, TRT); }

    inline ts_t byt_to_gmt(ts_t byt) { return zone_to_gmt(byt, BYT); }
    inline ts_t gmt_to_byt(ts_t gmt) { return gmt_to_zone(gmt, BYT); }

    inline ts_t sgt_to_gmt(ts_t sgt) { return zone_to_gmt(sgt, SGT); }
    inline ts_t gmt_to_sgt(ts_t gmt) { return gmt_to_zone(gmt, SGT); }

    inline ts_t ict_to_gmt(ts_t ict) { return zone_to_gmt(ict, ICT); }
    inline ts_t gmt_to_ict(ts_t gmt) { return gmt_to_zone(gmt, ICT); }

    inline ts_t pht_to_gmt(ts_t pht) { return zone_to_gmt(pht, PHT); }
    inline ts_t gmt_to_pht(ts_t gmt) { return gmt_to_zone(gmt, PHT); }

    inline ts_t gst_to_gmt(ts_t gst) { return zone_to_gmt(gst, GST); }
    inline ts_t gmt_to_gst(ts_t gmt) { return gmt_to_zone(gmt, GST); }

    inline ts_t hkt_to_gmt(ts_t hkt) { return zone_to_gmt(hkt, HKT); }
    inline ts_t gmt_to_hkt(ts_t gmt) { return gmt_to_zone(gmt, HKT); }

    inline ts_t jst_to_gmt(ts_t jst) { return zone_to_gmt(jst, JST); }
    inline ts_t gmt_to_jst(ts_t gmt) { return gmt_to_zone(gmt, JST); }

    inline ts_t kst_to_gmt(ts_t kst) { return zone_to_gmt(kst, KST); }
    inline ts_t gmt_to_kst(ts_t gmt) { return gmt_to_zone(gmt, KST); }

    /// \brief Convert Kyiv civil time to GMT using the EET/EEST rules.
    inline ts_t kyiv_to_gmt(ts_t kyiv) { return eet_to_gmt(kyiv); }

    /// \brief Convert GMT to Kyiv civil time using the EET/EEST rules.
    inline ts_t gmt_to_kyiv(ts_t gmt) { return gmt_to_eet(gmt); }

    /// \}

} // namespace time_shield

#endif // TIME_SHIELD_HEADER_TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED
