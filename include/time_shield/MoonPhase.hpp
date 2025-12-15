// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_MOON_PHASE_HPP_INCLUDED
#define _TIME_SHIELD_MOON_PHASE_HPP_INCLUDED

/// \file MoonPhase.hpp
/// \brief Geocentric Moon phase calculator and result helpers.
/// \ingroup time_conversions

#include "date_time_conversions.hpp"
#include "types.hpp"

#include <array>
#include <cmath>
#include <cstddef>

namespace time_shield {

namespace astronomy {

    /// \brief Result of Moon phase computation (geocentric approximation).
    struct MoonPhaseResult {
        double phase = 0.0;              ///< Phase fraction in [0..1). 0=new moon, 0.5=full moon.
        double illumination = 0.0;       ///< Illuminated fraction in [0..1].
        double age_days = 0.0;           ///< Age of the Moon in days since new moon (approx).
        double distance_km = 0.0;        ///< Distance to Moon in km (approx).
        double diameter_deg = 0.0;       ///< Angular diameter of Moon in degrees (approx).
        double age_deg = 0.0;            ///< Phase angle in degrees (0..360).
        double phase_angle_rad = 0.0;    ///< Phase angle in radians (0..2*pi).
        double phase_sin = 0.0;          ///< sin(phase_angle_rad) helper for continuous signal.
        double phase_cos = 0.0;          ///< cos(phase_angle_rad) helper for continuous signal.
        double sun_distance_km = 0.0;
        double sun_diameter_deg = 0.0;
    };

    /// \brief Lunar quarter instants (Unix UTC seconds, floating).
    struct MoonQuarterInstants {
        double previous_new_unix_s = 0.0; ///< Previous new moon instant (Unix UTC seconds, double).
        double previous_first_quarter_unix_s = 0.0; ///< Previous first quarter instant (Unix UTC seconds, double).
        double previous_full_unix_s = 0.0; ///< Previous full moon instant (Unix UTC seconds, double).
        double previous_last_quarter_unix_s = 0.0; ///< Previous last quarter instant (Unix UTC seconds, double).
        double next_new_unix_s = 0.0; ///< Next new moon instant (Unix UTC seconds, double).
        double next_first_quarter_unix_s = 0.0; ///< Next first quarter instant (Unix UTC seconds, double).
        double next_full_unix_s = 0.0; ///< Next full moon instant (Unix UTC seconds, double).
        double next_last_quarter_unix_s = 0.0; ///< Next last quarter instant (Unix UTC seconds, double).
    };

    /// \brief Moon phase calculator (geocentric approximation).
    ///
    /// References:
    /// - John Walker, "moontool" (Fourmilab). See: https://www.fourmilab.ch/moontoolw/
    /// - solarissmoke/php-moon-phase (port). See: https://github.com/solarissmoke/php-moon-phase/blob/master/Solaris/MoonPhase.php
    ///
    /// Notes:
    /// - Input timestamps are assumed to be UTC Unix seconds (can be floating).
    /// - Computation is geocentric (no observer latitude/longitude corrections).
    ///
    /// Example usage:
    /// \code{.cpp}
    /// using namespace time_shield;
    /// MoonPhase calculator{};
    /// double ts = 1704067200.0; // 2024-01-01T00:00:00Z
    /// MoonPhaseResult res = calculator.compute(ts);          // illumination, angles, sin/cos
    /// MoonPhase::quarters_unix_s_t quarters = calculator.quarter_times_unix(ts); // Unix seconds as double
    /// MoonQuarterInstants mapped = calculator.quarter_instants_unix(ts); // structured view
    /// bool near_full = calculator.is_full_moon_window(ts, 3600.0); // +/-1h window check
    /// \endcode
    class MoonPhase {
    public:
        using quarters_unix_s_t = std::array<double, 8>; ///< Quarter instants as Unix UTC seconds ({new, firstQ, full, lastQ} for previous and next cycles).
        static constexpr double kDefaultQuarterWindow_s = 43200.0; ///< Default window around phase events (12h).

        /// \brief Compute full set of Moon phase parameters for given UTC timestamp.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \return Geocentric Moon phase parameters for the given instant.
        MoonPhaseResult compute(double unix_utc_s) const noexcept {
            MoonPhaseResult out{};
            const double jd = julian_day_from_unix_seconds(unix_utc_s);

            // --- Sun position ---
            const double day = jd - kEpochJd;
            const double N = fix_angle((360.0 / 365.2422) * day);
            const double M = fix_angle(N + kElonge - kElongp);

            double Ec = kepler(M, kEccent);
            Ec = std::sqrt((1.0 + kEccent) / (1.0 - kEccent)) * std::tan(Ec / 2.0);
            Ec = 2.0 * rad2deg(std::atan(Ec));
            const double lambda_sun = fix_angle(Ec + kElongp);

            const double F = ((1.0 + kEccent * std::cos(deg2rad(Ec))) / (1.0 - kEccent * kEccent));
            const double sun_dist = kSunSmax / F;
            const double sun_ang = F * kSunAngSiz;

            // --- Moon position ---
            const double ml = fix_angle(13.1763966 * day + kMmLong);
            const double MM = fix_angle(ml - 0.1114041 * day - kMmLongp);

            const double Ev = 1.2739 * std::sin(deg2rad(2.0 * (ml - lambda_sun) - MM));
            const double Ae = 0.1858 * std::sin(deg2rad(M));
            const double A3 = 0.37   * std::sin(deg2rad(M));
            const double MmP = MM + Ev - Ae - A3;

            const double mEc = 6.2886 * std::sin(deg2rad(MmP));
            const double A4  = 0.214  * std::sin(deg2rad(2.0 * MmP));
            const double lP  = ml + Ev + mEc - Ae + A4;

            const double V   = 0.6583 * std::sin(deg2rad(2.0 * (lP - lambda_sun)));
            const double lPP = lP + V;

            // --- Phase ---
            const double moon_age_deg_wrapped = fix_angle(lPP - lambda_sun);
            const double moon_age_rad = deg2rad(moon_age_deg_wrapped);
            const double illum = (1.0 - std::cos(moon_age_rad)) / 2.0;

            const double moon_dist = (kMsMax * (1.0 - kMecc * kMecc))
                / (1.0 + kMecc * std::cos(deg2rad(MmP + mEc)));

            const double moon_dfrac = moon_dist / kMsMax;
            const double moon_ang = kMAngSiz / moon_dfrac;

            out.phase = moon_age_deg_wrapped / 360.0;
            out.illumination = illum;
            out.age_days = kSynMonth * out.phase;
            out.distance_km = moon_dist;
            out.diameter_deg = moon_ang;
            out.age_deg = moon_age_deg_wrapped;
            out.phase_angle_rad = moon_age_rad;
            out.phase_sin = std::sin(moon_age_rad);
            out.phase_cos = std::cos(moon_age_rad);
            out.sun_distance_km = sun_dist;
            out.sun_diameter_deg = sun_ang;
            return out;
        }

        /// \brief Compute only phase fraction in [0..1) for given UTC timestamp.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \return Phase fraction in the \f$[0, 1)\f$ interval where 0=new moon, 0.5=full moon.
        double compute_phase(double unix_utc_s) const noexcept {
            return compute(unix_utc_s).phase;
        }

        /// \brief Compute quarter/new/full instants around given timestamp.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \return Array of 8 instants (Unix UTC seconds as double): {prev new, prev firstQ, prev full, prev lastQ, next new, next firstQ, next full, next lastQ}.
        quarters_unix_s_t quarter_times_unix(double unix_utc_s) const noexcept {
            const double sdate = julian_day_from_unix_seconds(unix_utc_s);
            double adate = sdate - 45.0;

            const double ats = unix_utc_s - 86400.0 * 45.0;
            const int yy = year_from_unix_seconds(ats);
            const int mm = month_from_unix_seconds(ats);

            // IMPORTANT: use floating division
            double k1 = std::floor((yy + ((mm - 1) * (1.0 / 12.0)) - 1900.0) * 12.3685);
            double k2 = 0.0;

            double nt1 = mean_phase_jd(adate, k1);
            adate = nt1;

            while (true) {
                adate += kSynMonth;
                k2 = k1 + 1.0;

                double nt2 = mean_phase_jd(adate, k2);
                if (std::abs(nt2 - sdate) < 0.75) {
                    nt2 = true_phase_jd(k2, 0.0); // new moon correction
                }

                if (nt1 <= sdate && nt2 > sdate) {
                    break;
                }

                nt1 = nt2;
                k1 = k2;
            }

            const double dates_jd[8] = {
                true_phase_jd(k1, 0.0),
                true_phase_jd(k1, 0.25),
                true_phase_jd(k1, 0.5),
                true_phase_jd(k1, 0.75),
                true_phase_jd(k2, 0.0),
                true_phase_jd(k2, 0.25),
                true_phase_jd(k2, 0.5),
                true_phase_jd(k2, 0.75)
            };

            quarters_unix_s_t out{};
            for (std::size_t i = 0; i < 8; ++i) {
                out[i] = jd_to_unix_seconds(dates_jd[i]);
            }
            return out;
        }

        /// \brief Compatibility wrapper returning quarter instants as Unix UTC seconds.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \return Same payload as quarter_times_unix() for compatibility.
        quarters_unix_s_t quarter_times(double unix_utc_s) const noexcept {
            return quarter_times_unix(unix_utc_s);
        }

        /// \brief Quarter instants around the provided timestamp as a structured result (Unix UTC seconds).
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \return Previous/next quarter instants mapped into a structured result with Unix UTC seconds.
        MoonQuarterInstants quarter_instants_unix(double unix_utc_s) const noexcept {
            const auto quarters = quarter_times_unix(unix_utc_s);
            MoonQuarterInstants out{};
            out.previous_new_unix_s = quarters[0];
            out.previous_first_quarter_unix_s = quarters[1];
            out.previous_full_unix_s = quarters[2];
            out.previous_last_quarter_unix_s = quarters[3];
            out.next_new_unix_s = quarters[4];
            out.next_first_quarter_unix_s = quarters[5];
            out.next_full_unix_s = quarters[6];
            out.next_last_quarter_unix_s = quarters[7];
            return out;
        }

        /// \brief Check whether timestamp is inside a window around new moon.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \param window_seconds Symmetric window size in seconds around the event time.
        /// \return true if the timestamp lies within the window of the previous or next new moon.
        bool is_new_moon_window(double unix_utc_s, double window_seconds = kDefaultQuarterWindow_s) const noexcept {
            const auto instants = quarter_instants_unix(unix_utc_s);
            return is_within_window(unix_utc_s, instants.previous_new_unix_s, instants.next_new_unix_s, window_seconds);
        }

        /// \brief Check whether timestamp is inside a window around full moon.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \param window_seconds Symmetric window size in seconds around the event time.
        /// \return true if the timestamp lies within the window of the previous or next full moon.
        bool is_full_moon_window(double unix_utc_s, double window_seconds = kDefaultQuarterWindow_s) const noexcept {
            const auto instants = quarter_instants_unix(unix_utc_s);
            return is_within_window(unix_utc_s, instants.previous_full_unix_s, instants.next_full_unix_s, window_seconds);
        }

        /// \brief Check whether timestamp is inside a window around first quarter.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \param window_seconds Symmetric window size in seconds around the event time.
        /// \return true if the timestamp lies within the window of the previous or next first quarter.
        bool is_first_quarter_window(double unix_utc_s, double window_seconds = kDefaultQuarterWindow_s) const noexcept {
            const auto instants = quarter_instants_unix(unix_utc_s);
            return is_within_window(unix_utc_s, instants.previous_first_quarter_unix_s, instants.next_first_quarter_unix_s, window_seconds);
        }

        /// \brief Check whether timestamp is inside a window around last quarter.
        /// \param unix_utc_s Timestamp in Unix UTC seconds (can be fractional).
        /// \param window_seconds Symmetric window size in seconds around the event time.
        /// \return true if the timestamp lies within the window of the previous or next last quarter.
        bool is_last_quarter_window(double unix_utc_s, double window_seconds = kDefaultQuarterWindow_s) const noexcept {
            const auto instants = quarter_instants_unix(unix_utc_s);
            return is_within_window(unix_utc_s, instants.previous_last_quarter_unix_s, instants.next_last_quarter_unix_s, window_seconds);
        }

    private:
        static double julian_day_from_unix_seconds(double unix_utc_s) noexcept {
            return 2440587.5 + unix_utc_s / 86400.0;
        }

        static int year_from_unix_seconds(double unix_utc_s) noexcept {
            return static_cast<int>(year_of<year_t>(static_cast<ts_t>(unix_utc_s)));
        }

        static int month_from_unix_seconds(double unix_utc_s) noexcept {
            return static_cast<int>(month_of_year<int>(static_cast<ts_t>(unix_utc_s)));
        }

        static double jd_to_unix_seconds(double julian_day) noexcept {
            return (julian_day - 2440587.5) * 86400.0;
        }

        // --- Constants (1980 January 0.0) ---
        static constexpr double kEpochJd   = 2444238.5;

        static constexpr double kElonge   = 278.833540;
        static constexpr double kElongp   = 282.596403;
        static constexpr double kEccent   = 0.016718;
        static constexpr double kSunSmax  = 1.495985e8; // km
        static constexpr double kSunAngSiz= 0.533128;   // deg

        static constexpr double kMmLong   = 64.975464;
        static constexpr double kMmLongp  = 349.383063;
        [[maybe_unused]] static constexpr double kMNode    = 151.950429;
        [[maybe_unused]] static constexpr double kMInc     = 5.145396;
        static constexpr double kMecc     = 0.054900;
        static constexpr double kMAngSiz  = 0.5181;     // deg
        static constexpr double kMsMax    = 384401.0;   // km
        [[maybe_unused]] static constexpr double kMParallax= 0.9507;     // deg
        static constexpr double kSynMonth = 29.53058868;

        static constexpr double kPi = 3.14159265358979323846;

        static double deg2rad(double deg) noexcept { return deg * (kPi / 180.0); }
        static double rad2deg(double rad) noexcept { return rad * (180.0 / kPi); }

        static double fix_angle(double a) noexcept {
            a = std::fmod(a, 360.0);
            if (a < 0.0) a += 360.0;
            return a;
        }

        static double kepler(double m_deg, double ecc) noexcept {
            constexpr double eps = 1e-6;
            const double m = deg2rad(m_deg);
            double e = m;
            for (int i = 0; i < 50; ++i) {
                const double delta = e - ecc * std::sin(e) - m;
                e -= delta / (1.0 - ecc * std::cos(e));
                if (std::abs(delta) <= eps) break;
            }
            return e;
        }

        double mean_phase_jd(double julian_day, double lunation_index) const noexcept {
            const double jt = (julian_day - 2415020.0) / 36525.0;
            const double t2 = jt * jt;
            const double t3 = t2 * jt;

            return 2415020.75933 + kSynMonth * lunation_index
                + 0.0001178 * t2
                - 0.000000155 * t3
                + 0.00033 * std::sin(deg2rad(166.56 + 132.87 * jt - 0.009173 * t2));
        }

        double true_phase_jd(double lunation_index, double phase_fraction) const noexcept {
            // This algorithm is designed for phase_fraction in {0, 0.25, 0.5, 0.75}.
            const double kx = lunation_index + phase_fraction;
            const double t  = kx / 1236.85;
            const double t2 = t * t;
            const double t3 = t2 * t;

            double pt = 2415020.75933
                + kSynMonth * kx
                + 0.0001178 * t2
                - 0.000000155 * t3
                + 0.00033 * std::sin(deg2rad(166.56 + 132.87 * t - 0.009173 * t2));

            const double m      = 359.2242 + 29.10535608  * kx - 0.0000333 * t2 - 0.00000347 * t3;
            const double mprime = 306.0253 + 385.81691806 * kx + 0.0107306  * t2 + 0.00001236 * t3;
            const double f      = 21.2964  + 390.67050646 * kx - 0.0016528  * t2 - 0.00000239 * t3;

            // Corrections (same structure as common ports of moontool)
            if (phase_fraction < 0.01 || std::abs(phase_fraction - 0.5) < 0.01) {
                pt += (0.1734 - 0.000393 * t) * std::sin(deg2rad(m))
                    + 0.0021 * std::sin(deg2rad(2 * m))
                    - 0.4068 * std::sin(deg2rad(mprime))
                    + 0.0161 * std::sin(deg2rad(2 * mprime))
                    - 0.0004 * std::sin(deg2rad(3 * mprime))
                    + 0.0104 * std::sin(deg2rad(2 * f))
                    - 0.0051 * std::sin(deg2rad(m + mprime))
                    - 0.0074 * std::sin(deg2rad(m - mprime))
                    + 0.0004 * std::sin(deg2rad(2 * f + m))
                    - 0.0004 * std::sin(deg2rad(2 * f - m))
                    - 0.0006 * std::sin(deg2rad(2 * f + mprime))
                    + 0.0010 * std::sin(deg2rad(2 * f - mprime))
                    + 0.0005 * std::sin(deg2rad(m + 2 * mprime));
                return pt;
            }

            if (std::abs(phase_fraction - 0.25) < 0.01 || std::abs(phase_fraction - 0.75) < 0.01) {
                pt += (0.1721 - 0.0004 * t) * std::sin(deg2rad(m))
                    + 0.0021 * std::sin(deg2rad(2 * m))
                    - 0.6280 * std::sin(deg2rad(mprime))
                    + 0.0089 * std::sin(deg2rad(2 * mprime))
                    - 0.0004 * std::sin(deg2rad(3 * mprime))
                    + 0.0079 * std::sin(deg2rad(2 * f))
                    - 0.0119 * std::sin(deg2rad(m + mprime))
                    - 0.0047 * std::sin(deg2rad(m - mprime))
                    + 0.0003 * std::sin(deg2rad(2 * f + m))
                    - 0.0004 * std::sin(deg2rad(2 * f - m))
                    - 0.0006 * std::sin(deg2rad(2 * f + mprime))
                    + 0.0021 * std::sin(deg2rad(2 * f - mprime))
                    + 0.0003 * std::sin(deg2rad(m + 2 * mprime))
                + 0.0004 * std::sin(deg2rad(m - 2 * mprime))
                - 0.0003 * std::sin(deg2rad(2 * m + mprime));

                if (phase_fraction < 0.5) {
                    pt += 0.0028 - 0.0004 * std::cos(deg2rad(m)) + 0.0003 * std::cos(deg2rad(mprime));
                } else {
                    pt += -0.0028 + 0.0004 * std::cos(deg2rad(m)) - 0.0003 * std::cos(deg2rad(mprime));
                }
                return pt;
            }

            // Fallback: return uncorrected estimate
            return pt;
        }

        static bool is_within_window(double unix_utc_s, double previous_instant, double next_instant, double window_seconds) noexcept {
            const double prev_delta = std::abs(unix_utc_s - previous_instant);
            const double next_delta = std::abs(unix_utc_s - next_instant);
            return (prev_delta <= window_seconds) || (next_delta <= window_seconds);
        }
    };

} // namespace astronomy

    /// \brief Convenience alias for the geocentric Moon phase calculator.
    using MoonPhaseCalculator = astronomy::MoonPhase;

} // namespace time_shield

#endif // _TIME_SHIELD_MOON_PHASE_HPP_INCLUDED
