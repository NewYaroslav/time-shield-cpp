// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED

/// \file time_zone_conversions.hpp
/// \brief Helpers for converting CET/EET/ET timestamps to GMT (UTC).
/// \ingroup time_zone_conversions

#include "date_time_struct.hpp"
#include "time_conversions.hpp"

namespace time_shield {

    /// \ingroup time_conversions_time_zone_conversions
    /// \{

    /// \brief Convert Central European Time to Greenwich Mean Time.
    /// \param cet Timestamp in seconds in CET.
    /// \return Timestamp in seconds in GMT.
    inline ts_t cet_to_gmt(ts_t cet) {
        DateTimeStruct dt = to_date_time(cet);
        int max_days = num_days_in_month(dt.year, dt.mon);
        const int OLD_START_SUMMER_HOUR = 2;
        const int OLD_STOP_SUMMER_HOUR  = 3;
        const int NEW_SUMMER_HOUR       = 1;

        if(dt.year < 2002) {
            if(dt.mon > MAR && dt.mon < OCT) {
                return cet - SEC_PER_HOUR * 2;
            } else 
            if(dt.mon == MAR) {
                for(int d = max_days; d >= dt.day; --d) {
                    if(day_of_week_date(dt.year, MAR, d) == SUN) {
                        if(d == dt.day) {
                            if(dt.hour >= OLD_START_SUMMER_HOUR)
                                return cet - SEC_PER_HOUR * 2;
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
                            if(dt.hour >= OLD_STOP_SUMMER_HOUR)
                                return cet - SEC_PER_HOUR;
                            return cet - SEC_PER_HOUR;
                        }
                        return cet - SEC_PER_HOUR * 2;
                    }
                }
                return cet - SEC_PER_HOUR;
            }
            return cet - SEC_PER_HOUR;
        } else {
            if(dt.mon > MAR && dt.mon < OCT) {
                return cet - SEC_PER_HOUR * 2;
            } else 
            if(dt.mon == MAR) {
                for(int d = max_days; d >= dt.day; --d) {
                    if(day_of_week_date(dt.year, MAR, d) == SUN) {
                        if(d == dt.day) {
                            if(dt.hour >= (NEW_SUMMER_HOUR + 2))
                                return cet - SEC_PER_HOUR * 2;
                            return cet - SEC_PER_HOUR;
                        }
                        return cet - SEC_PER_HOUR;
                    }
                }
                return cet - SEC_PER_HOUR * 2;
            } else 
            if(dt.mon == OCT) {
                for(int d = max_days; d >= dt.day; --d) {
                    if(day_of_week_date(dt.year, OCT, d) == SUN) {
                        if(d == dt.day) {
                            if(dt.hour >= (NEW_SUMMER_HOUR + 1))
                                return cet - SEC_PER_HOUR;
                            return cet - SEC_PER_HOUR * 2;
                        }
                        return cet - SEC_PER_HOUR * 2;
                    }
                }
            }
        }
        return cet - SEC_PER_HOUR;
    }

    /// \brief Convert Eastern European Time to Greenwich Mean Time.
    /// \param eet Timestamp in seconds in EET.
    /// \return Timestamp in seconds in GMT.
    inline ts_t eet_to_gmt(ts_t eet) {
        return cet_to_gmt(eet - SEC_PER_HOUR);
    }

    /// \brief Check if local US Eastern time uses DST.
    /// \param dt Local time in ET.
    /// \return True if DST applies for the provided local timestamp.
    inline bool is_us_eastern_dst_local(const DateTimeStruct& dt) {
        const int SWITCH_HOUR = 2;
        int start_day = 0;
        int end_day = 0;
        int start_month = 0;
        int end_month = 0;

        if(dt.year >= 2007) {
            start_month = MAR;
            end_month = NOV;
            int first_sunday_march = 1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, MAR, 1)) % DAYS_PER_WEEK;
            start_day = first_sunday_march + 7;
            end_day = 1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, NOV, 1)) % DAYS_PER_WEEK;
        } else {
            start_month = APR;
            end_month = OCT;
            start_day = 1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, APR, 1)) % DAYS_PER_WEEK;
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

    /// \brief Convert US Eastern Time (New York, EST/EDT) to GMT (UTC).
    /// \param et Timestamp in seconds in ET.
    /// \return Timestamp in seconds in GMT (UTC).
    ///
    /// GMT in this library uses UTC. DST rules are guaranteed for 1987+;
    /// earlier years use 1987-2006 rules as a best-effort approximation.
    inline ts_t et_to_gmt(ts_t et) {
        DateTimeStruct dt = to_date_time(et);
        bool is_dst = is_us_eastern_dst_local(dt);
        return et + SEC_PER_HOUR * (is_dst ? 4 : 5);
    }

    /// \brief Convert GMT (UTC) to US Eastern Time (New York, EST/EDT).
    /// \param gmt Timestamp in seconds in GMT (UTC).
    /// \return Timestamp in seconds in ET.
    ///
    /// GMT in this library uses UTC. DST rules are guaranteed for 1987+;
    /// earlier years use 1987-2006 rules as a best-effort approximation.
    inline ts_t gmt_to_et(ts_t gmt) {
        ts_t et_standard = gmt - SEC_PER_HOUR * 5;
        DateTimeStruct dt_local = to_date_time(et_standard);
        bool is_dst = is_us_eastern_dst_local(dt_local);
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
    /// \return Timestamp in seconds in CET.
    inline ts_t gmt_to_cet(ts_t gmt) {
        DateTimeStruct dt = to_date_time(gmt);
        const int SWITCH_HOUR = 1;

        if(dt.mon > MAR && dt.mon < OCT) {
            return gmt + SEC_PER_HOUR * 2;
        } else if(dt.mon == MAR) {
            int last = last_sunday_month_day(dt.year, MAR);
            if(dt.day > last) {
                return gmt + SEC_PER_HOUR * 2;
            } else if(dt.day < last) {
                return gmt + SEC_PER_HOUR;
            } else {
                if(dt.hour >= SWITCH_HOUR)
                    return gmt + SEC_PER_HOUR * 2;
                return gmt + SEC_PER_HOUR;
            }
        } else if(dt.mon == OCT) {
            int last = last_sunday_month_day(dt.year, OCT);
            if(dt.day > last) {
                return gmt + SEC_PER_HOUR;
            } else if(dt.day < last) {
                return gmt + SEC_PER_HOUR * 2;
            } else {
                if(dt.hour >= SWITCH_HOUR)
                    return gmt + SEC_PER_HOUR;
                return gmt + SEC_PER_HOUR * 2;
            }
        }
        return gmt + SEC_PER_HOUR;
    }

    /// \brief Convert Greenwich Mean Time to Eastern European Time.
    /// \param gmt Timestamp in seconds in GMT.
    /// \return Timestamp in seconds in EET.
    inline ts_t gmt_to_eet(ts_t gmt) {
        return gmt_to_cet(gmt) + SEC_PER_HOUR;
    }

    /// \}

} // namespace time_shield

#endif // _TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED
