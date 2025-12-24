// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_NTP_TIME_SERVICE_HPP_INCLUDED
#define _TIME_SHIELD_NTP_TIME_SERVICE_HPP_INCLUDED

#include "config.hpp"

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include "ntp_client_pool.hpp"
#include "ntp_client_pool_runner.hpp"
#include "time_utils.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace time_shield {

    template <class RunnerT>
    class NtpTimeServiceT;

    namespace detail {
#ifdef TIME_SHIELD_TEST_FAKE_NTP
        /// \brief Fake runner for tests without network access.
        class FakeNtpRunner {
        public:
            /// \brief Construct fake runner.
            FakeNtpRunner() = default;
            /// \brief Construct fake runner with an unused pool.
            explicit FakeNtpRunner(NtpClientPool) {}

            FakeNtpRunner(const FakeNtpRunner&) = delete;
            FakeNtpRunner& operator=(const FakeNtpRunner&) = delete;

            /// \brief Stop background thread on destruction.
            ~FakeNtpRunner() {
                stop();
            }

            /// \brief Start fake measurements on a background thread.
            bool start(std::chrono::milliseconds interval = std::chrono::seconds(30),
                       bool measure_immediately = true) {
                if (m_is_running.load()) {
                    return false;
                }
                if (interval.count() <= 0) {
                    interval = std::chrono::milliseconds(1);
                }
                m_interval = interval;
                m_is_stop_requested.store(false);
                m_is_force_requested.store(false);
                m_is_running.store(true);
                m_measure_immediately = measure_immediately;
                try {
                    m_thread = std::thread(&FakeNtpRunner::run_loop, this);
                } catch (...) {
                    m_is_running.store(false);
                    return false;
                }
                return true;
            }

            /// \brief Start fake measurements using milliseconds.
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
            bool running() const noexcept { return m_is_running.load(); }

            /// \brief Wake the worker thread and request a measurement.
            bool force_measure() {
                if (!m_is_running.load()) {
                    return false;
                }
                m_is_force_requested.store(true);
                m_cv.notify_one();
                return true;
            }

            /// \brief Perform one measurement immediately.
            bool measure_now() {
                return do_measure();
            }

            /// \brief Return last estimated offset in microseconds.
            int64_t offset_us() const noexcept { return m_offset_us.load(); }
            /// \brief Return current UTC time in microseconds based on offset.
            int64_t utc_time_us() const noexcept { return now_realtime_us() + m_offset_us.load(); }
            /// \brief Return current UTC time in milliseconds based on offset.
            int64_t utc_time_ms() const noexcept { return utc_time_us() / 1000; }
            /// \brief Return current UTC time in seconds based on offset.
            int64_t utc_time_sec() const noexcept { return utc_time_us() / 1000000; }

            /// \brief Return whether last measurement updated the offset.
            bool last_measure_ok() const noexcept { return m_last_measure_ok.load(); }
            /// \brief Return total number of measurement attempts.
            uint64_t measure_count() const noexcept { return m_measure_count.load(); }
            /// \brief Return number of failed measurement attempts.
            uint64_t fail_count() const noexcept { return m_fail_count.load(); }
            /// \brief Return realtime timestamp of last measurement attempt.
            int64_t last_update_realtime_us() const noexcept { return m_last_update_realtime_us.load(); }
            /// \brief Return realtime timestamp of last successful measurement.
            int64_t last_success_realtime_us() const noexcept { return m_last_success_realtime_us.load(); }

            /// \brief Return copy of most recent samples.
            std::vector<NtpSample> last_samples() const { return {}; }

        private:
            /// \brief Background loop for fake measurements.
            void run_loop() {
                const auto sleep_interval = m_interval;
                bool is_first = m_measure_immediately;
                while (!m_is_stop_requested.load()) {
                    if (is_first) {
                        do_measure();
                        is_first = false;
                    } else {
                        std::unique_lock<std::mutex> lk(m_cv_mtx);
                        m_cv.wait_for(lk, sleep_interval, [this]() {
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

            /// \brief Update fake offset and stats.
            bool do_measure() {
                const uint64_t count = m_measure_count.fetch_add(1) + 1;
                m_offset_us.store(static_cast<int64_t>(count * 1000));
                m_last_measure_ok.store(true);
                const int64_t now = now_realtime_us();
                m_last_update_realtime_us.store(now);
                m_last_success_realtime_us.store(now);
                return true;
            }

        private:
            std::chrono::milliseconds m_interval{std::chrono::seconds(30)};
            bool m_measure_immediately{true};

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
            std::atomic<int64_t> m_offset_us{0};
        };
#endif // _TIME_SHIELD_TEST_FAKE_NTP

#if defined(TIME_SHIELD_TEST_FAKE_NTP)
        using RunnerAlias = detail::FakeNtpRunner;
#else
        using RunnerAlias = NtpClientPoolRunner;
#endif

#if defined(TIME_SHIELD_NTP_TIME_SERVICE_USE_DLL_SINGLETON)
#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef TIME_SHIELD_NTP_TIME_SERVICE_DLL_EXPORTS
#define TIME_SHIELD_NTP_TIME_SERVICE_API __declspec(dllexport)
#else
#define TIME_SHIELD_NTP_TIME_SERVICE_API __declspec(dllimport)
#endif
#else
#define TIME_SHIELD_NTP_TIME_SERVICE_API
#endif

        extern "C" TIME_SHIELD_NTP_TIME_SERVICE_API NtpTimeServiceT<RunnerAlias>& ntp_time_service_instance() noexcept;
#endif

        template <class RunnerT>
        struct NtpTimeServiceSingleton final {
            static NtpTimeServiceT<RunnerT>& instance() noexcept {
                static NtpTimeServiceT<RunnerT>* p_instance = new NtpTimeServiceT<RunnerT>{};
                return *p_instance;
            }
        };

#if defined(TIME_SHIELD_NTP_TIME_SERVICE_USE_DLL_SINGLETON)
        template <>
        struct NtpTimeServiceSingleton<RunnerAlias> final {
            static NtpTimeServiceT<RunnerAlias>& instance() noexcept {
                return ntp_time_service_instance();
            }
        };
#endif
    } // namespace detail

    /// \ingroup ntp
    /// \brief Singleton service for background NTP measurements.
    ///
    /// Uses an internal runner to keep offset updated. It exposes UTC time
    /// computed as realtime clock plus the latest offset. Configure pool
    /// servers and sampling before starting the service.
    template <class RunnerT>
    class NtpTimeServiceT {
    public:
        /// \brief Return the singleton instance.
        /// \return Singleton instance.
        static NtpTimeServiceT& instance() noexcept {
            return detail::NtpTimeServiceSingleton<RunnerT>::instance();
        }

        NtpTimeServiceT(const NtpTimeServiceT&) = delete;
        NtpTimeServiceT& operator=(const NtpTimeServiceT&) = delete;

        /// \brief Start background measurements using stored interval.
        /// \return True when background runner started.
        bool init() {
            return init(m_interval, m_measure_immediately);
        }

        /// \brief Start background measurements with interval and immediate flag.
        /// \param interval Measurement interval.
        /// \param measure_immediately Measure before first sleep if true.
        /// \return True when background runner started.
        bool init(std::chrono::milliseconds interval, bool measure_immediately = true) {
            std::unique_ptr<RunnerT> local_runner;
            {
                std::lock_guard<std::mutex> lk(m_mtx);
                if (is_running_locked()) {
                    return true;
                }
                if (interval.count() <= 0) {
                    interval = std::chrono::milliseconds(1);
                }
                m_interval = interval;
                m_measure_immediately = measure_immediately;

                local_runner = build_runner_locked();
                if (!local_runner) {
                    return false;
                }
            }

            if (!local_runner->start(m_interval, m_measure_immediately)) {
                return false;
            }

            bool is_ok = false;
            try {
                is_ok = local_runner->measure_now();
            } catch (...) {
                is_ok = false;
            }

            {
                std::lock_guard<std::mutex> lk(m_mtx);
                m_runner = std::move(local_runner);
            }
            return is_ok;
        }

        /// \brief Stop background measurements and release resources.
        void shutdown() {
            std::unique_ptr<RunnerT> local_runner;
            {
                std::lock_guard<std::mutex> lk(m_mtx);
                if (!m_runner) {
                    return;
                }
                local_runner = std::move(m_runner);
            }
            try {
                local_runner->stop();
            } catch (...) {
                // no-throw
            }
        }

        /// \brief Return true when background runner is active.
        /// \return True when background runner is active.
        bool running() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            return is_running_locked();
        }

        /// \brief Ensure background runner is started with current config.
        void ensure_started() noexcept {
            if (running()) {
                return;
            }
            (void)init();
        }

        /// \brief Return last estimated offset in microseconds.
        /// \return Offset in microseconds (UTC - local realtime).
        int64_t offset_us() noexcept {
            ensure_started();
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->offset_us();
        }

        /// \brief Return current UTC time in microseconds based on offset.
        /// \return UTC time in microseconds using last offset.
        int64_t utc_time_us() noexcept {
            ensure_started();
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return now_realtime_us();
            return m_runner->utc_time_us();
        }

        /// \brief Return current UTC time in milliseconds based on offset.
        /// \return UTC time in milliseconds using last offset.
        int64_t utc_time_ms() noexcept {
            return utc_time_us() / 1000;
        }

        /// \brief Return current UTC time in seconds based on offset.
        /// \return UTC time in seconds using last offset.
        int64_t utc_time_sec() noexcept {
            return utc_time_us() / 1000000;
        }

        /// \brief Return whether last measurement updated the offset.
        /// \return True when last measurement updated the offset.
        bool last_measure_ok() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return false;
            return m_runner->last_measure_ok();
        }

        /// \brief Return total number of measurement attempts.
        /// \return Number of measurement attempts.
        uint64_t measure_count() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->measure_count();
        }

        /// \brief Return number of failed measurement attempts.
        /// \return Number of failed measurement attempts.
        uint64_t fail_count() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->fail_count();
        }

        /// \brief Return realtime timestamp of last measurement attempt.
        /// \return Realtime microseconds timestamp for last measurement attempt.
        int64_t last_update_realtime_us() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->last_update_realtime_us();
        }

        /// \brief Return realtime timestamp of last successful measurement.
        /// \return Realtime microseconds timestamp for last successful measurement.
        int64_t last_success_realtime_us() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->last_success_realtime_us();
        }

        /// \brief Return true when last measurement is older than max_age.
        /// \param max_age Maximum allowed age.
        /// \return True when last measurement age exceeds max_age.
        bool stale(std::chrono::milliseconds max_age) const noexcept {
            const int64_t last = last_update_realtime_us();
            if (last == 0) {
                return true;
            }
            const int64_t age = now_realtime_us() - last;
            return age > max_age.count() * 1000;
        }

        /// \brief Replace server list used for new runner instances.
        /// \param servers Server configurations to use.
        /// \return False when service is already running.
        bool set_servers(std::vector<NtpServerConfig> servers) {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_servers = true;
            m_servers = std::move(servers);
            return true;
        }

        /// \brief Use conservative default servers for new runner instances.
        /// \return False when service is already running.
        bool set_default_servers() {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_servers = true;
            m_servers = NtpClientPool::build_default_servers();
            return true;
        }

        /// \brief Clear custom server list and return to default behavior.
        /// \return False when service is already running.
        bool clear_servers() {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_servers = false;
            m_servers.clear();
            return true;
        }

        /// \brief Override pool configuration for new runner instances.
        /// \param cfg Pool configuration to apply.
        /// \return False when service is already running.
        bool set_pool_config(NtpPoolConfig cfg) {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_pool_cfg = true;
            m_pool_cfg = std::move(cfg);
            return true;
        }

        /// \brief Return current pool configuration.
        /// \return Current pool configuration.
        NtpPoolConfig pool_config() const {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (m_has_custom_pool_cfg) {
                return m_pool_cfg;
            }
            return NtpPoolConfig{};
        }

        /// \brief Return copy of last measurement samples.
        /// \return Copy of samples from the last measurement.
        std::vector<NtpSample> last_samples() const {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return {};
            return m_runner->last_samples();
        }

        /// \brief Construct service.
        NtpTimeServiceT() = default;
        /// \brief Stop background runner on destruction.
        ~NtpTimeServiceT() {
            shutdown();
        }

        /// \brief Apply current config by rebuilding the runner.
        /// \return True when runner restarted successfully.
        bool apply_config_now() {
            std::unique_ptr<RunnerT> new_runner;
            std::unique_ptr<RunnerT> old_runner;
            std::chrono::milliseconds interval;
            bool measure_immediately = true;
            {
                std::lock_guard<std::mutex> lk(m_mtx);
                new_runner = build_runner_locked();
                if (!new_runner) {
                    return false;
                }
                interval = m_interval;
                measure_immediately = m_measure_immediately;
                old_runner = std::move(m_runner);
            }

            if (old_runner) {
                try {
                    old_runner->stop();
                } catch (...) {
                }
            }

            if (!new_runner->start(interval, measure_immediately)) {
                return false;
            }

            bool is_ok = false;
            try {
                is_ok = new_runner->measure_now();
            } catch (...) {
                is_ok = false;
            }

            {
                std::lock_guard<std::mutex> lk(m_mtx);
                m_runner = std::move(new_runner);
            }
            return is_ok;
        }

    private:
        /// \brief Check runner status under lock.
        bool is_running_locked() const noexcept {
            return m_runner && m_runner->running();
        }

        /// \brief Build a runner with current server list and pool config.
        std::unique_ptr<RunnerT> build_runner_locked() {
            std::vector<NtpServerConfig> servers;
            if (m_has_custom_servers) {
                servers = m_servers;
            } else {
                servers = NtpClientPool::build_default_servers();
            }

            NtpPoolConfig cfg = m_has_custom_pool_cfg ? m_pool_cfg : NtpPoolConfig{};
            NtpClientPool pool(cfg);
            pool.set_servers(std::move(servers));

            std::unique_ptr<RunnerT> runner;
            try {
                runner.reset(new RunnerT(std::move(pool)));
            } catch (...) {
                return nullptr;
            }
            return runner;
        }

    private:
        mutable std::mutex m_mtx;
        std::chrono::milliseconds m_interval{std::chrono::seconds(30)};
        bool m_measure_immediately{true};

        bool m_has_custom_servers{false};
        std::vector<NtpServerConfig> m_servers;

        bool m_has_custom_pool_cfg{false};
        NtpPoolConfig m_pool_cfg{};

        std::unique_ptr<RunnerT> m_runner;

    };

#if defined(TIME_SHIELD_NTP_TIME_SERVICE_USE_DLL_SINGLETON) && defined(TIME_SHIELD_NTP_TIME_SERVICE_DLL_IMPLEMENTATION)
namespace detail {
    extern "C" TIME_SHIELD_NTP_TIME_SERVICE_API NtpTimeServiceT<RunnerAlias>& ntp_time_service_instance() noexcept {
        static NtpTimeServiceT<RunnerAlias>* p_instance = new NtpTimeServiceT<RunnerAlias>{};
        return *p_instance;
    }
} // namespace detail
#endif

#if defined(TIME_SHIELD_TEST_FAKE_NTP)
    /// \ingroup ntp
    /// \brief NTP time service alias that uses a fake runner for tests.
    using NtpTimeService = NtpTimeServiceT<detail::FakeNtpRunner>;
#else
    /// \ingroup ntp
    /// \brief NTP time service alias that uses the real pool runner.
    using NtpTimeService = NtpTimeServiceT<NtpClientPoolRunner>;
#endif

namespace ntp {

    /// \ingroup ntp
    /// \brief Initialize NTP time service and start background measurements.
    /// \param interval Measurement interval.
    /// \param measure_immediately Measure before first sleep if true.
    /// \return True when background runner started.
    inline bool init(std::chrono::milliseconds interval = std::chrono::seconds(30),
                     bool measure_immediately = true) {
        return NtpTimeService::instance().init(interval, measure_immediately);
    }

    /// \ingroup ntp
    /// \brief Initialize NTP time service using milliseconds.
    /// \param interval_ms Measurement interval in milliseconds.
    /// \param measure_immediately Measure before first sleep if true.
    /// \return True when background runner started.
    inline bool init(int interval_ms,
                     bool measure_immediately = true) {
        return NtpTimeService::instance().init(std::chrono::milliseconds(interval_ms), measure_immediately);
    }
    
    /// \ingroup ntp
    /// \brief Stop NTP time service.
    inline void shutdown() {
        NtpTimeService::instance().shutdown();
    }
    
    /// \ingroup ntp
    /// \brief Return last estimated offset in microseconds.
    /// \return Offset in microseconds (UTC - local realtime).
    inline int64_t offset_us() noexcept {
        return NtpTimeService::instance().offset_us();
    }
    
    /// \ingroup ntp
    /// \brief Return current UTC time in microseconds based on offset.
    /// \return UTC time in microseconds using last offset.
    inline int64_t utc_time_us() noexcept {
        return NtpTimeService::instance().utc_time_us();
    }
    
    /// \ingroup ntp
    /// \brief Return current UTC time in milliseconds based on offset.
    /// \return UTC time in milliseconds using last offset.
    inline int64_t utc_time_ms() noexcept {
        return NtpTimeService::instance().utc_time_ms();
    }
    
    /// \ingroup ntp
    /// \brief Return current UTC time in seconds based on offset.
    /// \return UTC time in seconds using last offset.
    inline int64_t utc_time_sec() noexcept {
        return NtpTimeService::instance().utc_time_sec();
    }
    
    /// \ingroup ntp
    /// \brief Return whether last measurement updated the offset.
    /// \return True when last measurement updated the offset.
    inline bool last_measure_ok() noexcept {
        return NtpTimeService::instance().last_measure_ok();
    }
    
    /// \ingroup ntp
    /// \brief Return total number of measurement attempts.
    /// \return Number of measurement attempts.
    inline uint64_t measure_count() noexcept {
        return NtpTimeService::instance().measure_count();
    }
    
    /// \ingroup ntp
    /// \brief Return number of failed measurement attempts.
    /// \return Number of failed measurement attempts.
    inline uint64_t fail_count() noexcept {
        return NtpTimeService::instance().fail_count();
    }
    
    /// \ingroup ntp
    /// \brief Return realtime timestamp of last measurement attempt.
    /// \return Realtime microseconds timestamp for last measurement attempt.
    inline int64_t last_update_realtime_us() noexcept {
        return NtpTimeService::instance().last_update_realtime_us();
    }
    
    /// \ingroup ntp
    /// \brief Return realtime timestamp of last successful measurement.
    /// \return Realtime microseconds timestamp for last successful measurement.
    inline int64_t last_success_realtime_us() noexcept {
        return NtpTimeService::instance().last_success_realtime_us();
    }
    
    /// \ingroup ntp
    /// \brief Return true when last measurement is older than max_age.
    /// \param max_age Maximum allowed age.
    /// \return True when last measurement age exceeds max_age.
    inline bool stale(std::chrono::milliseconds max_age) noexcept {
        return NtpTimeService::instance().stale(max_age);
    }

} // namespace ntp

} // namespace time_shield

#else // TIME_SHIELD_ENABLE_NTP_CLIENT

namespace time_shield {
    class NtpTimeService {
    public:
        static NtpTimeService& instance() {
            static_assert(sizeof(void*) == 0, "NtpTimeService is disabled by configuration.");
            return *reinterpret_cast<NtpTimeService*>(0);
        }
    };
} // namespace time_shield

#endif // _TIME_SHIELD_ENABLE_NTP_CLIENT

#endif // _TIME_SHIELD_NTP_TIME_SERVICE_HPP_INCLUDED
