// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_ASTRONOMY_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_ASTRONOMY_CONVERSIONS_HPP_INCLUDED

/// \file astronomy_conversions.hpp
/// \brief Astronomy entry header with Julian conversions and lunar helpers.
/// \ingroup time_conversions

#include "config.hpp"
#include "julian_conversions.hpp"
#include "MoonPhase.hpp"

#include <cmath>

namespace time_shield {

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
