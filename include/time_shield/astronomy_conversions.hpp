// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_ASTRONOMY_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_ASTRONOMY_CONVERSIONS_HPP_INCLUDED

/// \file astronomy_conversions.hpp
/// \brief Julian Date / MJD / JDN and simple lunar helpers.
/// \ingroup time_conversions
///
/// JD epoch used here:
/// - Unix epoch (1970-01-01 00:00:00 UTC) is JD 2440587.5
///
/// Notes:
/// - JD and MJD are returned as double (jd_t/mjd_t).
/// - These functions are intended for utility/analytics, not for high-precision astronomy.

#include "config.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "MoonPhase.hpp"
#include <cmath>

namespace time_shield {

    /// \brief Convert Unix timestamp (floating seconds) to Julian Date (JD).
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Julian Date value.
    inline jd_t fts_to_jd(fts_t ts) noexcept {
        // JD at Unix epoch:
        // 1970-01-01 00:00:00 UTC -> 2440587.5
        return static_cast<jd_t>(2440587.5)
             + static_cast<jd_t>(ts) / static_cast<jd_t>(SEC_PER_DAY);
    }

    /// \brief Convert Unix timestamp (seconds) to Julian Date (JD).
    /// \param ts Unix timestamp in seconds since Unix epoch.
    /// \return Julian Date value.
    inline jd_t ts_to_jd(ts_t ts) noexcept {
        return fts_to_jd(static_cast<fts_t>(ts));
    }

    /// \brief Convert Gregorian date (with optional fractional day) to Julian Date (JD).
    /// \param day Day of month (may include fractional part).
    /// \param month Month [1..12].
    /// \param year Full year (e.g. 2025).
    /// \return Julian Date value.
    inline jd_t gregorian_to_jd(double day, int64_t month, int64_t year) noexcept {
        // Algorithm source (as per your original code): krutov.org Julianday
        if (month == 1 || month == 2) {
            year -= 1;
            month += 12;
        }
        const double a = std::floor(static_cast<double>(year) / 100.0);
        const double b = 2.0 - a + std::floor(a / 4.0);
        const double jdn = std::floor(365.25 * (static_cast<double>(year) + 4716.0))
                         + std::floor(30.6000001 * (static_cast<double>(month) + 1.0))
                         + day + b - 1524.5;
        return static_cast<jd_t>(jdn);
    }
    
    /// \brief Convert Gregorian date/time components to Julian Date (JD).
    /// \param day Day of month [1..31].
    /// \param month Month [1..12].
    /// \param year Full year (e.g. 2025).
    /// \param hour Hour of day [0..23].
    /// \param minute Minute of hour [0..59].
    /// \param second Second of minute [0..59].
    /// \param millisecond Millisecond of second [0..999].
    /// \return Julian Date value.
    inline jd_t gregorian_to_jd(
        uint32_t day,
        uint32_t month,
        uint32_t year,
        uint32_t hour,
        uint32_t minute,
        uint32_t second = 0,
        uint32_t millisecond = 0) noexcept {
        const double frac =
            (static_cast<double>(hour) / 24.0) +
            (static_cast<double>(minute) / (24.0 * 60.0)) +
            ((static_cast<double>(second) + static_cast<double>(millisecond) / 1000.0)
              / static_cast<double>(SEC_PER_DAY));
        return gregorian_to_jd(static_cast<double>(day) + frac,
                               static_cast<int64_t>(month),
                               static_cast<int64_t>(year));
    }
    
    /// \brief Convert Unix timestamp (floating seconds) to Modified Julian Date (MJD).
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Modified Julian Date value.
    inline mjd_t fts_to_mjd(fts_t ts) noexcept {
        return static_cast<mjd_t>(fts_to_jd(ts) - 2400000.5);
    }

    /// \brief Convert Unix timestamp (seconds) to Modified Julian Date (MJD).
    /// \param ts Unix timestamp in seconds since Unix epoch.
    /// \return Modified Julian Date value.
    inline mjd_t ts_to_mjd(ts_t ts) noexcept {
        return static_cast<mjd_t>(fts_to_mjd(static_cast<fts_t>(ts)));
    }

    /// \brief Convert Gregorian date to Julian Day Number (JDN).
    /// \details JDN is an integer day count (no fractional part).
    /// \param day Day of month [1..31].
    /// \param month Month [1..12].
    /// \param year Full year (e.g. 2025).
    /// \return Julian Day Number value.
    inline jdn_t gregorian_to_jdn(uint32_t day, uint32_t month, uint32_t year) noexcept {
        const uint64_t a = (14ULL - static_cast<uint64_t>(month)) / 12ULL;
        const uint64_t y = static_cast<uint64_t>(year) + 4800ULL - a;
        const uint64_t m = static_cast<uint64_t>(month) + 12ULL * a - 3ULL;
        const uint64_t jdn = static_cast<uint64_t>(day)
                           + (153ULL * m + 2ULL) / 5ULL
                           + 365ULL * y
                           + y / 4ULL
                           - y / 100ULL
                           + y / 400ULL
                           - 32045ULL;
        return static_cast<jdn_t>(jdn);
    }

    /// \brief sin/cos helper for the Moon phase angle.
    struct MoonPhaseSineCosine {
        double phase_sin = 0.0; ///< sin(phase angle), continuous around 0/2pi.
        double phase_cos = 0.0; ///< cos(phase angle), continuous around 0/2pi.
        double phase_angle_rad = 0.0; ///< Phase angle in radians [0..2*pi).
        constexpr MoonPhaseSineCosine() = default;
        constexpr MoonPhaseSineCosine(double phase_sin_value, double phase_cos_value, double phase_angle_rad_value) noexcept
            : phase_sin(phase_sin_value),
              phase_cos(phase_cos_value),
              phase_angle_rad(phase_angle_rad_value) {}
    };

    /// \brief Get lunar phase in range [0..1) using a simple Julian Day approximation.
    /// \details This helper mirrors the legacy Julian Day based approximation and is less precise
    /// than the geocentric MoonPhase calculator.
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Approximate lunar phase fraction where 0 is new moon.
    inline double moon_phase_jd_approx(fts_t ts) noexcept {
        double temp = (static_cast<double>(fts_to_jd(ts)) - 2451550.1) / 29.530588853;
        temp = temp - std::floor(temp);
        if (temp < 0.0) temp += 1.0;
        return temp;
    }

    /// \brief Get lunar phase in range [0..1) using the geocentric MoonPhase calculator.
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Lunar phase fraction where 0 is new moon.
    inline double moon_phase(fts_t ts) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.compute(static_cast<double>(ts)).phase;
    }

    /// \brief Get sin/cos of the lunar phase angle (continuous signal without wrap-around).
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Structure containing sin/cos and the angle in radians.
    inline MoonPhaseSineCosine moon_phase_sincos(fts_t ts) noexcept {
        static const astronomy::MoonPhase calculator{};
        const auto result = calculator.compute(static_cast<double>(ts));
        return MoonPhaseSineCosine{result.phase_sin, result.phase_cos, result.phase_angle_rad};
    }

    /// \brief Get illuminated fraction in range [0..1] using the geocentric MoonPhase calculator.
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Illuminated fraction of the Moon.
    inline double moon_illumination(fts_t ts) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.compute(static_cast<double>(ts)).illumination;
    }

    /// \brief Get lunar age in days (~0..29.53) using a simple Julian Day approximation.
    /// \details This helper mirrors the legacy Julian Day based approximation and is less precise
    /// than the geocentric MoonPhase calculator.
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Approximate lunar age in days.
    inline double moon_age_days_jd_approx(fts_t ts) noexcept {
        return moon_phase_jd_approx(ts) * 29.530588853;
    }

    /// \brief Get lunar age in days (~0..29.53).
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Approximate lunar age in days.
    inline double moon_age_days(fts_t ts) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.compute(static_cast<double>(ts)).age_days;
    }

    /// \brief Quarter instants around the provided timestamp.
    /// \param ts Unix timestamp in floating seconds since Unix epoch.
    /// \return Quarter windows around the timestamp (Unix seconds as double).
    inline astronomy::MoonQuarterInstants moon_quarters(fts_t ts) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.quarter_instants_unix(static_cast<double>(ts));
    }

    /// \brief Check if timestamp falls into the new moon window (default \pm12h).
    inline bool is_new_moon_window(fts_t ts, double window_seconds = astronomy::MoonPhase::kDefaultQuarterWindow_s) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.is_new_moon_window(static_cast<double>(ts), window_seconds);
    }

    /// \brief Check if timestamp falls into the full moon window (default \pm12h).
    inline bool is_full_moon_window(fts_t ts, double window_seconds = astronomy::MoonPhase::kDefaultQuarterWindow_s) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.is_full_moon_window(static_cast<double>(ts), window_seconds);
    }

    /// \brief Check if timestamp falls into the first quarter window (default \pm12h).
    inline bool is_first_quarter_window(fts_t ts, double window_seconds = astronomy::MoonPhase::kDefaultQuarterWindow_s) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.is_first_quarter_window(static_cast<double>(ts), window_seconds);
    }

    /// \brief Check if timestamp falls into the last quarter window (default \pm12h).
    inline bool is_last_quarter_window(fts_t ts, double window_seconds = astronomy::MoonPhase::kDefaultQuarterWindow_s) noexcept {
        static const astronomy::MoonPhase calculator{};
        return calculator.is_last_quarter_window(static_cast<double>(ts), window_seconds);
    }

} // namespace time_shield

#endif // _TIME_SHIELD_ASTRONOMY_CONVERSIONS_HPP_INCLUDED
