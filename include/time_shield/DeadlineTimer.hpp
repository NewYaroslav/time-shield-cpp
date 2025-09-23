// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_DEADLINE_TIMER_HPP_INCLUDED
#define _TIME_SHIELD_DEADLINE_TIMER_HPP_INCLUDED

/// \file DeadlineTimer.hpp
/// \brief Monotonic deadline timer utility similar to Qt's QDeadlineTimer.
///
/// DeadlineTimer provides a lightweight helper around std::chrono::steady_clock
/// that tracks an absolute deadline and exposes helpers to query the remaining
/// time or whether the deadline has already passed.

#include "config.hpp"
#include "types.hpp"

#include <chrono>

namespace time_shield {

    /// \brief Helper that models a monotonic deadline for timeout management.
    ///
    /// DeadlineTimer invariants:
    /// * Not thread-safe. Access must stay within a single thread.
    /// * `start(timeout <= 0)` marks the deadline as already due.
    /// * Use `set_forever()` to represent "no timeout" semantics.
    class DeadlineTimer {
    public:
        using clock = std::chrono::steady_clock;
        using duration = clock::duration;
        using time_point = clock::time_point;

        /// \brief Constructs an inactive timer.
        DeadlineTimer() noexcept = default;

        /// \brief Constructs a timer with the specified absolute deadline.
        explicit DeadlineTimer(time_point deadline) noexcept {
            start(deadline);
        }

        /// \brief Constructs a timer that expires after the given timeout.
        template<class Rep, class Period>
        explicit DeadlineTimer(std::chrono::duration<Rep, Period> timeout) noexcept {
            start(timeout);
        }

        /// \brief Constructs a timer that expires after the given number of milliseconds.
        explicit DeadlineTimer(ts_ms_t timeout_ms) noexcept {
            start_ms(timeout_ms);
        }

        /// \brief Creates a timer that expires after the specified timeout.
        static DeadlineTimer from_timeout(duration timeout) noexcept {
            DeadlineTimer timer;
            timer.start(timeout);
            return timer;
        }

        /// \brief Creates a timer that expires after the specified timeout.
        template<class Rep, class Period>
        static DeadlineTimer from_timeout(std::chrono::duration<Rep, Period> timeout) noexcept {
            DeadlineTimer timer;
            timer.start(timeout);
            return timer;
        }

        /// \brief Creates a timer that expires after the specified number of seconds.
        static DeadlineTimer from_timeout_sec(ts_t timeout_sec) noexcept {
            DeadlineTimer timer;
            timer.start_sec(timeout_sec);
            return timer;
        }

        /// \brief Creates a timer that expires after the specified number of milliseconds.
        static DeadlineTimer from_timeout_ms(ts_ms_t timeout_ms) noexcept {
            DeadlineTimer timer;
            timer.start_ms(timeout_ms);
            return timer;
        }

        /// \brief Sets the absolute deadline and marks the timer as active.
        void start(time_point deadline) noexcept {
            m_deadline = deadline;
            m_is_running = true;
        }

        /// \brief Starts the timer so it expires after the specified timeout.
        ///
        /// Negative durations result in an immediate expiration. Durations that
        /// are shorter than the steady clock tick are rounded up to a single
        /// tick to preserve the monotonic nature of the timer.
        template<class Rep, class Period>
        void start(std::chrono::duration<Rep, Period> timeout) noexcept {
            const time_point now = clock::now();
            if (timeout <= decltype(timeout)::zero()) {
                start(now);
                return;
            }

            duration safe_duration = std::chrono::duration_cast<duration>(timeout);
            if (safe_duration <= duration::zero()) {
                safe_duration = duration(1);
            }

            const time_point max_time = time_point::max();
            const duration max_offset = max_time - now;
            if (safe_duration >= max_offset) {
                start(max_time);
                return;
            }

            start(now + safe_duration);
        }

        /// \brief Starts the timer so it expires after the specified number of seconds.
        void start_sec(ts_t timeout_sec) noexcept {
            start(std::chrono::seconds(timeout_sec));
        }

        /// \brief Starts the timer so it expires after the specified number of milliseconds.
        void start_ms(ts_ms_t timeout_ms) noexcept {
            start(std::chrono::milliseconds(timeout_ms));
        }

        /// \brief Stops the timer and invalidates the stored deadline.
        void stop() noexcept {
            m_is_running = false;
            m_deadline = time_point{};
        }

        /// \brief Marks the timer as running forever (no timeout).
        void set_forever() noexcept {
            m_is_running = true;
            m_deadline = time_point::max();
        }

        /// \brief Checks whether the timer currently tracks a deadline.
        TIME_SHIELD_NODISCARD bool is_running() const noexcept {
            return m_is_running;
        }

        /// \brief Checks whether the timer is configured for an infinite timeout.
        TIME_SHIELD_NODISCARD bool is_forever() const noexcept {
            return m_is_running && m_deadline == time_point::max();
        }

        /// \brief Returns stored deadline.
        TIME_SHIELD_NODISCARD time_point deadline() const noexcept {
            return m_deadline;
        }

        /// \brief Returns stored deadline as milliseconds since the steady epoch.
        TIME_SHIELD_NODISCARD ts_ms_t deadline_ms() const noexcept {
            return std::chrono::duration_cast<std::chrono::milliseconds>(m_deadline.time_since_epoch()).count();
        }

        /// \brief Returns stored deadline as seconds since the steady epoch.
        TIME_SHIELD_NODISCARD ts_t deadline_sec() const noexcept {
            return std::chrono::duration_cast<std::chrono::seconds>(m_deadline.time_since_epoch()).count();
        }

        /// \brief Checks if the deadline has already expired.
        TIME_SHIELD_NODISCARD bool has_expired() const noexcept {
            return has_expired(clock::now());
        }

        /// \brief Checks if the deadline has expired relative to the provided millisecond timestamp.
        TIME_SHIELD_NODISCARD bool has_expired_ms(ts_ms_t now_ms) const noexcept {
            const duration since_epoch = std::chrono::duration_cast<duration>(std::chrono::milliseconds(now_ms));
            return has_expired(time_point(since_epoch));
        }

        /// \brief Checks if the deadline has expired relative to the provided second timestamp.
        TIME_SHIELD_NODISCARD bool has_expired_sec(ts_t now_sec) const noexcept {
            const duration since_epoch = std::chrono::duration_cast<duration>(std::chrono::seconds(now_sec));
            return has_expired(time_point(since_epoch));
        }

        /// \brief Checks if the deadline has expired relative to the provided time point.
        TIME_SHIELD_NODISCARD bool has_expired(time_point now) const noexcept {
            return m_is_running && now >= m_deadline;
        }

        /// \brief Returns time remaining until the deadline.
        TIME_SHIELD_NODISCARD duration remaining_time() const noexcept {
            return remaining_time(clock::now());
        }

        /// \brief Returns remaining time in milliseconds until the deadline.
        TIME_SHIELD_NODISCARD ts_ms_t remaining_time_ms() const noexcept {
            return std::chrono::duration_cast<std::chrono::milliseconds>(remaining_time()).count();
        }

        /// \brief Returns remaining time in seconds until the deadline.
        TIME_SHIELD_NODISCARD ts_t remaining_time_sec() const noexcept {
            return std::chrono::duration_cast<std::chrono::seconds>(remaining_time()).count();
        }

        /// \brief Returns remaining time relative to the provided time point.
        ///
        /// Non-running timers and already expired timers report zero duration.
        TIME_SHIELD_NODISCARD duration remaining_time(time_point now) const noexcept {
            if (!m_is_running || now >= m_deadline) {
                return duration::zero();
            }
            return m_deadline - now;
        }

        /// \brief Extends deadline by the specified duration while preventing overflow.
        void add(duration extend_by) noexcept {
            if (!m_is_running || extend_by <= duration::zero()) {
                return;
            }

            const time_point now = clock::now();
            const time_point base = m_deadline > now ? m_deadline : now;
            const duration max_offset = time_point::max() - base;
            const duration safe_offset = extend_by < max_offset ? extend_by : max_offset;
            m_deadline = base + safe_offset;
        }

        /// \brief Extends deadline by the specified number of seconds while preventing overflow.
        void add_sec(ts_t extend_by_sec) noexcept {
            if (extend_by_sec <= 0) {
                return;
            }
            add(std::chrono::seconds(extend_by_sec));
        }

        /// \brief Extends deadline by the specified number of milliseconds while preventing overflow.
        void add_ms(ts_ms_t extend_by_ms) noexcept {
            if (extend_by_ms <= 0) {
                return;
            }
            add(std::chrono::milliseconds(extend_by_ms));
        }

    private:
        time_point m_deadline{};
        bool m_is_running{false};
    };

} // namespace time_shield

#endif // _TIME_SHIELD_DEADLINE_TIMER_HPP_INCLUDED
