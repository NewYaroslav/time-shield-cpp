// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_ZONE_CONVERSIONS_HPP_INCLUDED

/// \file time_zone_conversions.hpp
/// \brief Helpers for converting CET/EET timestamps to GMT.
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
