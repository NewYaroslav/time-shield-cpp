// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_ELAPSED_TIMER_HPP_INCLUDED
#define _TIME_SHIELD_ELAPSED_TIMER_HPP_INCLUDED

/// \file ElapsedTimer.hpp
/// \brief High-precision elapsed time measurement helper similar to Qt's QElapsedTimer.
///
/// ElapsedTimer provides lightweight access to std::chrono::steady_clock for
/// precise interval measurements without being affected by system clock
/// adjustments.

#include "config.hpp"
#include "types.hpp"

#include <chrono>
#include <cstdint>

namespace time_shield {

    /// \brief Helper that measures elapsed monotonic time spans.
    ///
    /// Instances are expected to be used from a single thread without
    /// additional synchronization.
    class ElapsedTimer {
    public:
        using clock = std::chrono::steady_clock;
        using duration = clock::duration;
        using time_point = clock::time_point;

        /// \brief Constructs an invalid timer.
        ElapsedTimer() noexcept = default;

        /// \brief Constructs a timer that starts immediately when requested.
        explicit ElapsedTimer(bool start_immediately) noexcept {
            if (start_immediately) {
                start();
            }
        }

        /// \brief Starts the timer using the current steady clock time.
        void start() noexcept {
            m_start_time = clock::now();
            m_is_running = true;
        }

        /// \brief Restarts the timer and returns the elapsed duration so far.
        TIME_SHIELD_NODISCARD duration restart() noexcept {
            const time_point now = clock::now();
            const duration delta = m_is_running ? now - m_start_time : duration::zero();
            m_start_time = now;
            m_is_running = true;
            return delta;
        }

        /// \brief Restarts the timer using a millisecond timestamp and returns elapsed milliseconds.
        TIME_SHIELD_NODISCARD ts_ms_t restart_ms(ts_ms_t now_ms) noexcept {
            const duration since_epoch = std::chrono::duration_cast<duration>(std::chrono::milliseconds(now_ms));
            const time_point now(since_epoch);
            const duration delta = m_is_running ? now - m_start_time : duration::zero();
            m_start_time = now;
            m_is_running = true;
            return std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
        }

        /// \brief Restarts the timer using a second timestamp and returns elapsed seconds.
        TIME_SHIELD_NODISCARD ts_t restart_sec(ts_t now_sec) noexcept {
            const duration since_epoch = std::chrono::duration_cast<duration>(std::chrono::seconds(now_sec));
            const time_point now(since_epoch);
            const duration delta = m_is_running ? now - m_start_time : duration::zero();
            m_start_time = now;
            m_is_running = true;
            return std::chrono::duration_cast<std::chrono::seconds>(delta).count();
        }

        /// \brief Invalidates the timer so subsequent elapsed() calls return zero.
        void invalidate() noexcept {
            m_is_running = false;
        }

        /// \brief Checks whether the timer currently measures elapsed time.
        TIME_SHIELD_NODISCARD bool is_running() const noexcept {
            return m_is_running;
        }

        /// \brief Alias for is_running() to match Qt naming conventions.
        TIME_SHIELD_NODISCARD bool is_valid() const noexcept {
            return m_is_running;
        }

        /// \brief Returns start time stored by the timer.
        TIME_SHIELD_NODISCARD time_point start_time() const noexcept {
            return m_start_time;
        }

        /// \brief Returns elapsed duration since the timer was started.
        TIME_SHIELD_NODISCARD duration elapsed() const noexcept {
            return elapsed(clock::now());
        }

        /// \brief Returns elapsed duration relative to the provided time point.
        TIME_SHIELD_NODISCARD duration elapsed(time_point now) const noexcept {
            if (!m_is_running) {
                return duration::zero();
            }
            return now - m_start_time;
        }

        /// \brief Returns elapsed nanoseconds since the timer was started.
        TIME_SHIELD_NODISCARD std::int64_t elapsed_ns() const noexcept {
            return elapsed_count<std::chrono::nanoseconds>();
        }

        /// \brief Returns elapsed nanoseconds relative to the provided timestamp in nanoseconds.
        TIME_SHIELD_NODISCARD std::int64_t elapsed_ns(std::int64_t now_ns) const noexcept {
            const duration since_epoch = std::chrono::duration_cast<duration>(std::chrono::nanoseconds(now_ns));
            const time_point now(since_epoch);
            return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed(now)).count();
        }

        /// \brief Returns elapsed milliseconds since the timer was started.
        TIME_SHIELD_NODISCARD ts_ms_t elapsed_ms() const noexcept {
            return elapsed_count<std::chrono::milliseconds>();
        }

        /// \brief Returns elapsed milliseconds relative to the provided timestamp in milliseconds.
        TIME_SHIELD_NODISCARD ts_ms_t elapsed_ms(ts_ms_t now_ms) const noexcept {
            const duration since_epoch = std::chrono::duration_cast<duration>(std::chrono::milliseconds(now_ms));
            const time_point now(since_epoch);
            return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed(now)).count();
        }

        /// \brief Returns elapsed seconds since the timer was started.
        TIME_SHIELD_NODISCARD ts_t elapsed_sec() const noexcept {
            return elapsed_count<std::chrono::seconds>();
        }

        /// \brief Returns elapsed seconds relative to the provided timestamp in seconds.
        TIME_SHIELD_NODISCARD ts_t elapsed_sec(ts_t now_sec) const noexcept {
            const duration since_epoch = std::chrono::duration_cast<duration>(std::chrono::seconds(now_sec));
            const time_point now(since_epoch);
            return std::chrono::duration_cast<std::chrono::seconds>(elapsed(now)).count();
        }

        /// \brief Returns elapsed duration in the desired chrono duration type.
        template <class Duration>
        TIME_SHIELD_NODISCARD typename Duration::rep elapsed_count() const noexcept {
            return std::chrono::duration_cast<Duration>(elapsed()).count();
        }

        /// \brief Checks if the given timeout in milliseconds has expired.
        TIME_SHIELD_NODISCARD bool has_expired(ts_ms_t timeout_ms) const noexcept {
            if (!m_is_running) {
                return false;
            }
            if (timeout_ms <= 0) {
                return true;
            }
            return elapsed_ms() >= timeout_ms;
        }

        /// \brief Checks if the given timeout in seconds has expired.
        TIME_SHIELD_NODISCARD bool has_expired_sec(ts_t timeout_sec) const noexcept {
            if (!m_is_running) {
                return false;
            }
            if (timeout_sec <= 0) {
                return true;
            }
            return elapsed() >= std::chrono::seconds(timeout_sec);
        }

        /// \brief Returns milliseconds since the internal clock reference.
        TIME_SHIELD_NODISCARD std::int64_t ms_since_reference() const noexcept {
            if (!m_is_running) {
                return 0;
            }
            return std::chrono::duration_cast<std::chrono::milliseconds>(m_start_time.time_since_epoch()).count();
        }

    private:
        time_point m_start_time{};
        bool m_is_running{false};
    };

} // namespace time_shield

#endif // _TIME_SHIELD_ELAPSED_TIMER_HPP_INCLUDED
