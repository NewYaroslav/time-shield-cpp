//+------------------------------------------------------------------+
//|                                        time_zone_conversions.mqh |
//|                         Time Shield - MQL5 Time Zone Conversions |
//|                                      Copyright 2025, NewYaroslav |
//|                   https://github.com/NewYaroslav/time-shield-cpp |
//+------------------------------------------------------------------+
#ifndef __TIME_SHIELD_TIME_ZONE_CONVERSIONS_MQH__
#define __TIME_SHIELD_TIME_ZONE_CONVERSIONS_MQH__

/// \file time_zone_conversions.mqh
/// \ingroup mql5
/// \brief Helpers for converting supported regional time zones and UTC.

#property copyright "Copyright 2025, NewYaroslav"
#property link      "https://github.com/NewYaroslav/time-shield-cpp"
#property strict

#include "time_conversions.mqh"

namespace time_shield {

    datetime zone_to_gmt(const datetime local, const TimeZone zone);
    datetime gmt_to_zone(const datetime gmt, const TimeZone zone);

    datetime cet_to_gmt_impl(const datetime cet) {
        DateTimeStruct dt = to_date_time(cet);
        int max_days = num_days_in_month(dt.year, dt.mon);
        const int OLD_START_SUMMER_HOUR = 2;
        const int OLD_STOP_SUMMER_HOUR  = 3;
        const int NEW_SUMMER_HOUR       = 1;

        if(dt.year < 2002) {
            if(dt.mon > MAR && dt.mon < OCT) {
                return cet - (datetime)SEC_PER_HOUR * 2;
            } else if(dt.mon == MAR) {
                for(int d = max_days; d >= dt.day; --d) {
                    if(day_of_week_date(dt.year, MAR, d) == SUN) {
                        if(d == dt.day) {
                            if(dt.hour >= OLD_START_SUMMER_HOUR) {
                                return cet - (datetime)SEC_PER_HOUR * 2;
                            }
                            return cet - (datetime)SEC_PER_HOUR;
                        }
                        return cet - (datetime)SEC_PER_HOUR;
                    }
                }
                return cet - (datetime)SEC_PER_HOUR * 2;
            } else if(dt.mon == OCT) {
                for(int d = max_days; d >= dt.day; --d) {
                    if(day_of_week_date(dt.year, OCT, d) == SUN) {
                        if(d == dt.day) {
                            if(dt.hour >= OLD_STOP_SUMMER_HOUR) {
                                return cet - (datetime)SEC_PER_HOUR;
                            }
                            return cet - (datetime)SEC_PER_HOUR;
                        }
                        return cet - (datetime)SEC_PER_HOUR * 2;
                    }
                }
                return cet - (datetime)SEC_PER_HOUR;
            }
            return cet - (datetime)SEC_PER_HOUR;
        }

        if(dt.mon > MAR && dt.mon < OCT) {
            return cet - (datetime)SEC_PER_HOUR * 2;
        }
        if(dt.mon == MAR) {
            for(int d = max_days; d >= dt.day; --d) {
                if(day_of_week_date(dt.year, MAR, d) == SUN) {
                    if(d == dt.day) {
                        if(dt.hour >= (NEW_SUMMER_HOUR + 2)) {
                            return cet - (datetime)SEC_PER_HOUR * 2;
                        }
                        return cet - (datetime)SEC_PER_HOUR;
                    }
                    return cet - (datetime)SEC_PER_HOUR;
                }
            }
            return cet - (datetime)SEC_PER_HOUR * 2;
        }
        if(dt.mon == OCT) {
            for(int d = max_days; d >= dt.day; --d) {
                if(day_of_week_date(dt.year, OCT, d) == SUN) {
                    if(d == dt.day) {
                        if(dt.hour >= (NEW_SUMMER_HOUR + 1)) {
                            return cet - (datetime)SEC_PER_HOUR;
                        }
                        return cet - (datetime)SEC_PER_HOUR * 2;
                    }
                    return cet - (datetime)SEC_PER_HOUR * 2;
                }
            }
        }
        return cet - (datetime)SEC_PER_HOUR;
    }

    datetime gmt_to_cet_impl(const datetime gmt) {
        DateTimeStruct dt = to_date_time(gmt);
        const int SWITCH_HOUR = 1;

        if(dt.mon > MAR && dt.mon < OCT) {
            return gmt + (datetime)SEC_PER_HOUR * 2;
        }
        if(dt.mon == MAR) {
            int last = last_sunday_month_day(dt.year, MAR);
            if(dt.day > last) {
                return gmt + (datetime)SEC_PER_HOUR * 2;
            }
            if(dt.day < last) {
                return gmt + (datetime)SEC_PER_HOUR;
            }
            if(dt.hour >= SWITCH_HOUR) {
                return gmt + (datetime)SEC_PER_HOUR * 2;
            }
            return gmt + (datetime)SEC_PER_HOUR;
        }
        if(dt.mon == OCT) {
            int last = last_sunday_month_day(dt.year, OCT);
            if(dt.day > last) {
                return gmt + (datetime)SEC_PER_HOUR;
            }
            if(dt.day < last) {
                return gmt + (datetime)SEC_PER_HOUR * 2;
            }
            if(dt.hour >= SWITCH_HOUR) {
                return gmt + (datetime)SEC_PER_HOUR;
            }
            return gmt + (datetime)SEC_PER_HOUR * 2;
        }
        return gmt + (datetime)SEC_PER_HOUR;
    }

    datetime european_local_to_gmt(const datetime local, const int standard_offset_hours) {
        return cet_to_gmt_impl(local - (datetime)SEC_PER_HOUR * (standard_offset_hours - 1));
    }

    datetime gmt_to_european_local(const datetime gmt, const int standard_offset_hours) {
        return gmt_to_cet_impl(gmt) + (datetime)SEC_PER_HOUR * (standard_offset_hours - 1);
    }

    bool is_us_eastern_dst_local(const DateTimeStruct &dt) {
        const int SWITCH_HOUR = 2;
        int start_day = 0;
        int end_day = 0;
        int start_month = 0;
        int end_month = 0;

        if(dt.year >= 2007) {
            start_month = MAR;
            end_month = NOV;
            int first_sunday_march =
                1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, MAR, 1)) % DAYS_PER_WEEK;
            start_day = first_sunday_march + 7;
            end_day =
                1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, NOV, 1)) % DAYS_PER_WEEK;
        } else {
            start_month = APR;
            end_month = OCT;
            start_day =
                1 + (DAYS_PER_WEEK - day_of_week_date(dt.year, APR, 1)) % DAYS_PER_WEEK;
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

    bool fixed_zone_offset(const TimeZone zone, long &utc_offset) {
        switch(zone) {
            case GMT:
            case UTC:
            case WET:
                utc_offset = 0;
                return true;
            case WEST:
                utc_offset = SEC_PER_HOUR;
                return true;
            case CET:
                utc_offset = SEC_PER_HOUR;
                return true;
            case CEST:
                utc_offset = SEC_PER_HOUR * 2;
                return true;
            case EET:
                utc_offset = SEC_PER_HOUR * 2;
                return true;
            case EEST:
                utc_offset = SEC_PER_HOUR * 3;
                return true;
            case IST:
                utc_offset = SEC_PER_HOUR * 5 + SEC_PER_MIN * 30;
                return true;
            case MYT:
            case WITA:
            case SGT:
            case PHT:
            case HKT:
                utc_offset = SEC_PER_HOUR * 8;
                return true;
            case WIB:
            case ICT:
                utc_offset = SEC_PER_HOUR * 7;
                return true;
            case WIT:
            case JST:
            case KST:
                utc_offset = SEC_PER_HOUR * 9;
                return true;
            case KZT:
                utc_offset = SEC_PER_HOUR * 5;
                return true;
            case TRT:
            case BYT:
                utc_offset = SEC_PER_HOUR * 3;
                return true;
            case GST:
                utc_offset = SEC_PER_HOUR * 4;
                return true;
            default:
                utc_offset = 0;
                return false;
        }
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert Central European Time to Greenwich Mean Time.
    datetime cet_to_gmt(const datetime cet) {
        return cet_to_gmt_impl(cet);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert Eastern European Time to Greenwich Mean Time.
    datetime eet_to_gmt(const datetime eet) {
        return european_local_to_gmt(eet, 2);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert GMT to Central European Time.
    datetime gmt_to_cet(const datetime gmt) {
        return gmt_to_cet_impl(gmt);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert GMT to Eastern European Time.
    datetime gmt_to_eet(const datetime gmt) {
        return gmt_to_european_local(gmt, 2);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert US Eastern Time to GMT.
    datetime et_to_gmt(const datetime et) {
        DateTimeStruct dt = to_date_time(et);
        bool is_dst = is_us_eastern_dst_local(dt);
        return et + (datetime)SEC_PER_HOUR * (is_dst ? 4 : 5);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert GMT to US Eastern Time.
    datetime gmt_to_et(const datetime gmt) {
        datetime et_standard = gmt - (datetime)SEC_PER_HOUR * 5;
        DateTimeStruct dt_local = to_date_time(et_standard);
        bool is_dst = is_us_eastern_dst_local(dt_local);
        return gmt - (datetime)SEC_PER_HOUR * (is_dst ? 4 : 5);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert New York Time to GMT.
    datetime ny_to_gmt(const datetime ny) {
        return et_to_gmt(ny);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert GMT to New York Time.
    datetime gmt_to_ny(const datetime gmt) {
        return gmt_to_et(gmt);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert US Central Time to GMT.
    datetime ct_to_gmt(const datetime ct) {
        return et_to_gmt(ct + (datetime)SEC_PER_HOUR);
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert GMT to US Central Time.
    datetime gmt_to_ct(const datetime gmt) {
        return gmt_to_et(gmt) - (datetime)SEC_PER_HOUR;
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert supported local civil time to GMT.
    datetime zone_to_gmt(const datetime local, const TimeZone zone) {
        if(local == (datetime)ERROR_TIMESTAMP) {
            return (datetime)ERROR_TIMESTAMP;
        }

        long utc_offset = 0;
        switch(zone) {
            case GMT:
            case UTC:
                return local;
            case WET:
                return european_local_to_gmt(local, 0);
            case CET:
                return cet_to_gmt(local);
            case EET:
                return eet_to_gmt(local);
            case WEST:
            case CEST:
            case EEST:
                fixed_zone_offset(zone, utc_offset);
                return local - (datetime)utc_offset;
            case ET:
                return et_to_gmt(local);
            case CT:
                return ct_to_gmt(local);
            case UNKNOWN:
                return (datetime)ERROR_TIMESTAMP;
            default:
                if(fixed_zone_offset(zone, utc_offset)) {
                    return local - (datetime)utc_offset;
                }
                return (datetime)ERROR_TIMESTAMP;
        }
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert GMT to a supported local civil time zone.
    datetime gmt_to_zone(const datetime gmt, const TimeZone zone) {
        if(gmt == (datetime)ERROR_TIMESTAMP) {
            return (datetime)ERROR_TIMESTAMP;
        }

        long utc_offset = 0;
        switch(zone) {
            case GMT:
            case UTC:
                return gmt;
            case WET:
                return gmt_to_european_local(gmt, 0);
            case CET:
                return gmt_to_cet(gmt);
            case EET:
                return gmt_to_eet(gmt);
            case WEST:
            case CEST:
            case EEST:
                fixed_zone_offset(zone, utc_offset);
                return gmt + (datetime)utc_offset;
            case ET:
                return gmt_to_et(gmt);
            case CT:
                return gmt_to_ct(gmt);
            case UNKNOWN:
                return (datetime)ERROR_TIMESTAMP;
            default:
                if(fixed_zone_offset(zone, utc_offset)) {
                    return gmt + (datetime)utc_offset;
                }
                return (datetime)ERROR_TIMESTAMP;
        }
    }

    /// \ingroup mql5_time_conversions
    /// \brief Convert a timestamp between two supported local civil time zones.
    datetime convert_time_zone(const datetime local, const TimeZone from, const TimeZone to) {
        datetime gmt = zone_to_gmt(local, from);
        if(gmt == (datetime)ERROR_TIMESTAMP) {
            return (datetime)ERROR_TIMESTAMP;
        }
        return gmt_to_zone(gmt, to);
    }

    datetime ist_to_gmt(const datetime ist) { return zone_to_gmt(ist, IST); }
    datetime gmt_to_ist(const datetime gmt) { return gmt_to_zone(gmt, IST); }

    datetime myt_to_gmt(const datetime myt) { return zone_to_gmt(myt, MYT); }
    datetime gmt_to_myt(const datetime gmt) { return gmt_to_zone(gmt, MYT); }

    datetime wib_to_gmt(const datetime wib) { return zone_to_gmt(wib, WIB); }
    datetime gmt_to_wib(const datetime gmt) { return gmt_to_zone(gmt, WIB); }

    datetime wita_to_gmt(const datetime wita) { return zone_to_gmt(wita, WITA); }
    datetime gmt_to_wita(const datetime gmt) { return gmt_to_zone(gmt, WITA); }

    datetime wit_to_gmt(const datetime wit) { return zone_to_gmt(wit, WIT); }
    datetime gmt_to_wit(const datetime gmt) { return gmt_to_zone(gmt, WIT); }

    datetime kzt_to_gmt(const datetime kzt) { return zone_to_gmt(kzt, KZT); }
    datetime gmt_to_kzt(const datetime gmt) { return gmt_to_zone(gmt, KZT); }

    datetime trt_to_gmt(const datetime trt) { return zone_to_gmt(trt, TRT); }
    datetime gmt_to_trt(const datetime gmt) { return gmt_to_zone(gmt, TRT); }

    datetime byt_to_gmt(const datetime byt) { return zone_to_gmt(byt, BYT); }
    datetime gmt_to_byt(const datetime gmt) { return gmt_to_zone(gmt, BYT); }

    datetime sgt_to_gmt(const datetime sgt) { return zone_to_gmt(sgt, SGT); }
    datetime gmt_to_sgt(const datetime gmt) { return gmt_to_zone(gmt, SGT); }

    datetime ict_to_gmt(const datetime ict) { return zone_to_gmt(ict, ICT); }
    datetime gmt_to_ict(const datetime gmt) { return gmt_to_zone(gmt, ICT); }

    datetime pht_to_gmt(const datetime pht) { return zone_to_gmt(pht, PHT); }
    datetime gmt_to_pht(const datetime gmt) { return gmt_to_zone(gmt, PHT); }

    datetime gst_to_gmt(const datetime gst) { return zone_to_gmt(gst, GST); }
    datetime gmt_to_gst(const datetime gmt) { return gmt_to_zone(gmt, GST); }

    datetime hkt_to_gmt(const datetime hkt) { return zone_to_gmt(hkt, HKT); }
    datetime gmt_to_hkt(const datetime gmt) { return gmt_to_zone(gmt, HKT); }

    datetime jst_to_gmt(const datetime jst) { return zone_to_gmt(jst, JST); }
    datetime gmt_to_jst(const datetime gmt) { return gmt_to_zone(gmt, JST); }

    datetime kst_to_gmt(const datetime kst) { return zone_to_gmt(kst, KST); }
    datetime gmt_to_kst(const datetime gmt) { return gmt_to_zone(gmt, KST); }

    datetime kyiv_to_gmt(const datetime kyiv) { return eet_to_gmt(kyiv); }
    datetime gmt_to_kyiv(const datetime gmt) { return gmt_to_eet(gmt); }

}; // namespace time_shield

#endif // __TIME_SHIELD_TIME_ZONE_CONVERSIONS_MQH__
