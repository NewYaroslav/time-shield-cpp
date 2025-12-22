// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_NTP_CLIENT_POOL_RUNNER_HPP_INCLUDED
#define _TIME_SHIELD_NTP_CLIENT_POOL_RUNNER_HPP_INCLUDED

#include "config.hpp"

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include "ntp_client_pool.hpp"
#include "time_utils.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace time_shield {

    /// \ingroup ntp
    /// \brief Background runner that periodically measures NTP offsets using a pool.
    ///
    /// \code
    /// time_shield::NtpClientPool pool;
    /// pool.set_default_servers();
    ///
    /// time_shield::NtpClientPoolRunner runner(std::move(pool));
    /// runner.start(std::chrono::seconds(30));
    ///
    /// auto now_ms = runner.utc_time_ms();
    /// auto offset = runner.offset_us();
    ///
    /// runner.stop();
    /// \endcode
    template <class PoolT>
    class BasicPoolRunner {
    public:
        /// \brief Construct runner with a pool instance.
        /// \param pool Pool instance to use.
        explicit BasicPoolRunner(PoolT pool = PoolT{})
            : m_pool(std::move(pool)) {}

        /// \brief Stop background thread on destruction.
        ~BasicPoolRunner() {
            stop();
        }

        /// \brief Start periodic measurements on a background thread.
        /// \param interval Measurement interval.
        /// \param measure_immediately Measure before first sleep if true.
        /// \return True when background thread started.
        bool start(std::chrono::milliseconds interval = std::chrono::seconds(30),
                   bool measure_immediately = true) {
            if (m_is_running.load()) {
                return false;
            }
            if (interval.count() <= 0) {
                interval = std::chrono::milliseconds(1);
            }

            m_is_stop_requested.store(false);
            m_is_force_requested.store(false);
            m_is_running.store(true);

            try {
                m_thread = std::thread(&BasicPoolRunner::run_loop, this, interval, measure_immediately);
            } catch (...) {
                m_is_running.store(false);
                return false;
            }

            return true;
        }

        /// \brief Start periodic measurements using milliseconds.
        /// \param interval_ms Measurement interval in milliseconds.
        /// \param measure_immediately Measure before first sleep if true.
        /// \return True when background thread started.
        bool start(int interval_ms, bool measure_immediately = true) {
            return start(std::chrono::milliseconds(interval_ms), measure_immediately);
        }

        /// \brief Stop background measurements.
        void stop() {
            m_is_stop_requested.store(true);
            m_cv.notify_all();
            if (m_thread.joinable()) {
                m_thread.join();
            }
            m_is_running.store(false);
        }

        /// \brief Return true when background thread is running.
        /// \return True when background measurements are active.
        bool running() const noexcept { return m_is_running.load(); }

        /// \brief Wake the worker thread and request a measurement.
        /// \return True when request accepted.
        bool force_measure() {
            if (!m_is_running.load()) {
                return false;
            }
            m_is_force_requested.store(true);
            m_cv.notify_one();
            return true;
        }

        /// \brief Perform one measurement immediately.
        /// \return True when pool offset updated.
        bool measure_now() {
            return do_measure();
        }

        /// \brief Return last estimated offset in microseconds.
        /// \return Offset in microseconds (UTC - local realtime).
        int64_t offset_us() const noexcept { return m_pool.offset_us(); }
        /// \brief Return current UTC time in microseconds using pool offset.
        /// \return UTC time in microseconds using pool offset.
        int64_t utc_time_us() const noexcept { return m_pool.utc_time_us(); }
        /// \brief Return current UTC time in milliseconds using pool offset.
        /// \return UTC time in milliseconds using pool offset.
        int64_t utc_time_ms() const noexcept { return m_pool.utc_time_ms(); }
        /// \brief Return current UTC time in seconds using pool offset.
        /// \return UTC time in seconds using pool offset.
        int64_t utc_time_sec() const noexcept { return utc_time_us() / 1000000; }

        /// \brief Return whether last measurement updated the offset.
        /// \return True when last measurement updated the offset.
        bool last_measure_ok() const noexcept { return m_last_measure_ok.load(); }
        /// \brief Return total number of measurement attempts.
        /// \return Number of measurement attempts.
        uint64_t measure_count() const noexcept { return m_measure_count.load(); }
        /// \brief Return number of failed measurement attempts.
        /// \return Number of failed measurement attempts.
        uint64_t fail_count() const noexcept { return m_fail_count.load(); }
        /// \brief Return realtime timestamp of last measurement attempt.
        /// \return Realtime microseconds timestamp for last measurement attempt.
        int64_t last_update_realtime_us() const noexcept { return m_last_update_realtime_us.load(); }
        /// \brief Return realtime timestamp of last successful measurement.
        /// \return Realtime microseconds timestamp for last successful measurement.
        int64_t last_success_realtime_us() const noexcept { return m_last_success_realtime_us.load(); }

        /// \brief Return copy of the most recent samples.
        /// \return Copy of samples from the last measurement.
        std::vector<NtpSample> last_samples() const { return m_pool.last_samples(); }

    private:
        void run_loop(std::chrono::milliseconds interval, bool measure_immediately) {
            bool is_first = measure_immediately;
            while (!m_is_stop_requested.load()) {
                if (is_first) {
                    do_measure();
                    is_first = false;
                } else {
                    std::unique_lock<std::mutex> lk(m_cv_mtx);
                    m_cv.wait_for(lk, interval, [this]() {
                        return m_is_stop_requested.load() || m_is_force_requested.load();
                    });
                    if (m_is_stop_requested.load()) {
                        break;
                    }
                    m_is_force_requested.store(false);
                    do_measure();
                }
            }
            m_is_running.store(false);
        }

        bool do_measure() {
            bool is_ok = false;
            try {
                std::lock_guard<std::mutex> lk(m_pool_mtx);
                is_ok = m_pool.measure();
            } catch (...) {
                is_ok = false;
            }

            m_measure_count.fetch_add(1);
            if (!is_ok) {
                m_fail_count.fetch_add(1);
            }

            m_last_measure_ok.store(is_ok);

            const int64_t now = now_realtime_us();
            m_last_update_realtime_us.store(now);
            if (is_ok) {
                m_last_success_realtime_us.store(now);
            }

            return is_ok;
        }

    private:
        PoolT m_pool;
        mutable std::mutex m_pool_mtx;

        std::thread m_thread;
        std::condition_variable m_cv;
        std::mutex m_cv_mtx;

        std::atomic<bool> m_is_running{false};
        std::atomic<bool> m_is_stop_requested{false};
        std::atomic<bool> m_is_force_requested{false};

        std::atomic<bool> m_last_measure_ok{false};
        std::atomic<uint64_t> m_measure_count{0};
        std::atomic<uint64_t> m_fail_count{0};
        std::atomic<int64_t> m_last_update_realtime_us{0};
        std::atomic<int64_t> m_last_success_realtime_us{0};
    };

    using NtpClientPoolRunner = BasicPoolRunner<NtpClientPool>;

} // namespace time_shield

#else // TIME_SHIELD_ENABLE_NTP_CLIENT

namespace time_shield {
    class NtpClientPoolRunner {
    public:
        NtpClientPoolRunner() {
            static_assert(sizeof(void*) == 0, "NtpClientPoolRunner is disabled by configuration.");
        }
    };
} // namespace time_shield

#endif // _TIME_SHIELD_ENABLE_NTP_CLIENT

#endif // _TIME_SHIELD_NTP_CLIENT_POOL_RUNNER_HPP_INCLUDED
