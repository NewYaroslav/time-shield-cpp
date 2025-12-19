// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_CPU_TICK_TIMER_HPP_INCLUDED
#define _TIME_SHIELD_CPU_TICK_TIMER_HPP_INCLUDED

/// \file CpuTickTimer.hpp
/// \brief Helper class for measuring CPU time using get_cpu_time().

#include "time_utils.hpp"

#include <cstddef>
#include <limits>

namespace time_shield {

    /// \ingroup time_utils
    /// \brief Timer that measures CPU time ticks using get_cpu_time().
    /// \details Class is intended for single-threaded use and assumes that
    /// get_cpu_time() is monotonic within the current process or thread. For
    /// long-running measurements (for example, durations longer than a day) it
    /// is recommended to periodically call record_sample() or restart() to
    /// limit floating-point precision loss.
    /// \note All reported durations are expressed in CPU tick units provided
    /// by get_cpu_time().
    class CpuTickTimer {
    public:
        /// \brief Construct timer and optionally start it immediately.
        /// \param is_start_immediately Indicates whether the timer should start right away.
        explicit CpuTickTimer(bool is_start_immediately = true) noexcept {
            if (is_start_immediately) {
                start();
            }
        }

        /// \brief Start measuring CPU time.
        void start() noexcept {
            m_start_ticks = get_cpu_time();
            m_end_ticks = m_start_ticks;
            m_is_running = true;
        }

        /// \brief Restart timer and reset collected statistics.
        void restart() noexcept {
            reset_samples();
            start();
        }

        /// \brief Stop measuring CPU time and freeze elapsed ticks.
        void stop() noexcept {
            if (m_is_running) {
                m_end_ticks = get_cpu_time();
                m_is_running = false;
            }
        }

        /// \brief Get elapsed CPU ticks since the last start.
        /// \return Elapsed CPU tick units produced by get_cpu_time().
        TIME_SHIELD_NODISCARD double elapsed() const noexcept {
            const double final_ticks = m_is_running ? get_cpu_time() : m_end_ticks;
            return final_ticks - m_start_ticks;
        }

        /// \brief Record sample using elapsed ticks and restart timer.
        /// \return Collected sample value in CPU tick units or 0.0 when the
        /// timer was not previously running.
        double record_sample() noexcept {
            if (!m_is_running) {
                start();
                m_last_sample_ticks = 0.0;
                return 0.0;
            }

            const double now_ticks = get_cpu_time();
            m_last_sample_ticks = now_ticks - m_start_ticks;
            m_start_ticks = now_ticks;

            accumulate_ticks(m_last_sample_ticks);
            ++m_sample_count;

            return m_last_sample_ticks;
        }

        /// \brief Reset collected samples without touching running state.
        void reset_samples() noexcept {
            m_total_ticks = 0.0;
            m_total_compensation = 0.0;
            m_last_sample_ticks = 0.0;
            m_sample_count = 0;
        }

        /// \brief Get the number of recorded samples.
        /// \return Count of recorded samples.
        TIME_SHIELD_NODISCARD std::size_t sample_count() const noexcept {
            return m_sample_count;
        }

        /// \brief Get total recorded CPU ticks across samples.
        /// \return Sum of recorded CPU tick units.
        TIME_SHIELD_NODISCARD double total_ticks() const noexcept {
            return m_total_ticks;
        }

        /// \brief Get average CPU ticks per sample.
        /// \return Average CPU tick units or NaN if there are no samples.
        TIME_SHIELD_NODISCARD double average_ticks() const noexcept {
            if (m_sample_count == 0U) {
                return std::numeric_limits<double>::quiet_NaN();
            }
            return m_total_ticks / static_cast<double>(m_sample_count);
        }

        /// \brief Get ticks collected during the last recorded sample.
        /// \return Ticks from the most recent sample in CPU tick units.
        TIME_SHIELD_NODISCARD double last_sample_ticks() const noexcept {
            return m_last_sample_ticks;
        }

    private:
        void accumulate_ticks(double sample_ticks) noexcept {
            const double compensated = sample_ticks - m_total_compensation;
            const double updated_total = m_total_ticks + compensated;
            m_total_compensation = (updated_total - m_total_ticks) - compensated;
            m_total_ticks = updated_total;
        }

        double m_start_ticks { 0.0 };
        double m_end_ticks { 0.0 };
        double m_total_ticks { 0.0 };
        double m_total_compensation { 0.0 };
        double m_last_sample_ticks { 0.0 };
        std::size_t m_sample_count { 0 };
        bool m_is_running { false };
    };

} // namespace time_shield

#endif // _TIME_SHIELD_CPU_TICK_TIMER_HPP_INCLUDED
