// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_NTP_TIME_SERVICE_HPP_INCLUDED
#define TIME_SHIELD_NTP_TIME_SERVICE_HPP_INCLUDED

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
        /// \brief Fake runner used for tests without network.
        class FakeNtpRunner {
        public:
            FakeNtpRunner() = default;
            explicit FakeNtpRunner(NtpClientPool) {}

            FakeNtpRunner(const FakeNtpRunner&) = delete;
            FakeNtpRunner& operator=(const FakeNtpRunner&) = delete;

            ~FakeNtpRunner() {
                stop();
            }

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

            bool start(int interval_ms, bool measure_immediately = true) {
                return start(std::chrono::milliseconds(interval_ms), measure_immediately);
            }

            void stop() {
                m_is_stop_requested.store(true);
                m_cv.notify_all();
                if (m_thread.joinable()) {
                    m_thread.join();
                }
                m_is_running.store(false);
            }

            bool running() const noexcept { return m_is_running.load(); }

            bool force_measure() {
                if (!m_is_running.load()) {
                    return false;
                }
                m_is_force_requested.store(true);
                m_cv.notify_one();
                return true;
            }

            bool measure_now() {
                return do_measure();
            }

            int64_t offset_us() const noexcept { return m_offset_us.load(); }
            int64_t utc_time_us() const noexcept { return now_realtime_us() + m_offset_us.load(); }
            int64_t utc_time_ms() const noexcept { return utc_time_us() / 1000; }
            int64_t utc_time_sec() const noexcept { return utc_time_us() / 1000000; }

            bool last_measure_ok() const noexcept { return m_last_measure_ok.load(); }
            uint64_t measure_count() const noexcept { return m_measure_count.load(); }
            uint64_t fail_count() const noexcept { return m_fail_count.load(); }
            int64_t last_update_realtime_us() const noexcept { return m_last_update_realtime_us.load(); }
            int64_t last_success_realtime_us() const noexcept { return m_last_success_realtime_us.load(); }

            std::vector<NtpSample> last_samples() const { return {}; }

        private:
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
#endif // TIME_SHIELD_TEST_FAKE_NTP

#ifndef TIME_SHIELD_CPP17
#if defined(TIME_SHIELD_TEST_FAKE_NTP)
        using RunnerAlias = detail::FakeNtpRunner;
#else
        using RunnerAlias = NtpClientPoolRunner;
#endif

        extern NtpTimeServiceT<RunnerAlias> g_ntp_time_service;
#endif // !TIME_SHIELD_CPP17
    } // namespace detail

    template <class RunnerT>
    class NtpTimeServiceT {
    public:
        static NtpTimeServiceT& instance() noexcept {
#ifdef TIME_SHIELD_CPP17
            static NtpTimeServiceT instance_obj;
            return instance_obj;
#else
            return detail::g_ntp_time_service;
#endif
        }

        NtpTimeServiceT(const NtpTimeServiceT&) = delete;
        NtpTimeServiceT& operator=(const NtpTimeServiceT&) = delete;

        bool init() {
            return init(m_interval, m_measure_immediately);
        }

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

        bool running() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            return is_running_locked();
        }

        void ensure_started() noexcept {
            if (running()) {
                return;
            }
            (void)init();
        }

        int64_t offset_us() noexcept {
            ensure_started();
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->offset_us();
        }

        int64_t utc_time_us() noexcept {
            ensure_started();
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return now_realtime_us();
            return m_runner->utc_time_us();
        }

        int64_t utc_time_ms() noexcept {
            return utc_time_us() / 1000;
        }

        int64_t utc_time_sec() noexcept {
            return utc_time_us() / 1000000;
        }

        bool last_measure_ok() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return false;
            return m_runner->last_measure_ok();
        }

        uint64_t measure_count() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->measure_count();
        }

        uint64_t fail_count() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->fail_count();
        }

        int64_t last_update_realtime_us() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->last_update_realtime_us();
        }

        int64_t last_success_realtime_us() const noexcept {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return 0;
            return m_runner->last_success_realtime_us();
        }

        bool stale(std::chrono::milliseconds max_age) const noexcept {
            const int64_t last = last_update_realtime_us();
            if (last == 0) {
                return true;
            }
            const int64_t age = now_realtime_us() - last;
            return age > max_age.count() * 1000;
        }

        bool set_servers(std::vector<NtpServerConfig> servers) {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_servers = true;
            m_servers = std::move(servers);
            return true;
        }

        bool set_default_servers() {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_servers = true;
            m_servers = NtpClientPool::build_default_servers();
            return true;
        }

        bool clear_servers() {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_servers = false;
            m_servers.clear();
            return true;
        }

        bool set_pool_config(NtpPoolConfig cfg) {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (is_running_locked()) {
                return false;
            }
            m_has_custom_pool_cfg = true;
            m_pool_cfg = std::move(cfg);
            return true;
        }

        NtpPoolConfig pool_config() const {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (m_has_custom_pool_cfg) {
                return m_pool_cfg;
            }
            return NtpPoolConfig{};
        }

        std::vector<NtpSample> last_samples() const {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (!m_runner) return {};
            return m_runner->last_samples();
        }

        NtpTimeServiceT() = default;
        ~NtpTimeServiceT() {
            shutdown();
        }

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
        bool is_running_locked() const noexcept {
            return m_runner && m_runner->running();
        }

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

#ifndef TIME_SHIELD_CPP17
namespace detail {
#if defined(TIME_SHIELD_NTP_TIME_SERVICE_DEFINE)
    NtpTimeServiceT<RunnerAlias> g_ntp_time_service;
#endif
} // namespace detail
#endif

#if defined(TIME_SHIELD_TEST_FAKE_NTP)
    using NtpTimeService = NtpTimeServiceT<detail::FakeNtpRunner>;
#else
    using NtpTimeService = NtpTimeServiceT<NtpClientPoolRunner>;
#endif

namespace ntp {

    inline bool init(std::chrono::milliseconds interval = std::chrono::seconds(30),
                     bool measure_immediately = true) {
        return NtpTimeService::instance().init(interval, measure_immediately);
    }

    inline bool init(int interval_ms,
                     bool measure_immediately = true) {
        return NtpTimeService::instance().init(std::chrono::milliseconds(interval_ms), measure_immediately);
    }
    
    inline void shutdown() {
        NtpTimeService::instance().shutdown();
    }
    
    inline int64_t offset_us() noexcept {
        return NtpTimeService::instance().offset_us();
    }
    
    inline int64_t utc_time_us() noexcept {
        return NtpTimeService::instance().utc_time_us();
    }
    
    inline int64_t utc_time_ms() noexcept {
        return NtpTimeService::instance().utc_time_ms();
    }
    
    inline int64_t utc_time_sec() noexcept {
        return NtpTimeService::instance().utc_time_sec();
    }
    
    inline bool last_measure_ok() noexcept {
        return NtpTimeService::instance().last_measure_ok();
    }
    
    inline uint64_t measure_count() noexcept {
        return NtpTimeService::instance().measure_count();
    }
    
    inline uint64_t fail_count() noexcept {
        return NtpTimeService::instance().fail_count();
    }
    
    inline int64_t last_update_realtime_us() noexcept {
        return NtpTimeService::instance().last_update_realtime_us();
    }
    
    inline int64_t last_success_realtime_us() noexcept {
        return NtpTimeService::instance().last_success_realtime_us();
    }
    
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

#endif // TIME_SHIELD_ENABLE_NTP_CLIENT

#endif // TIME_SHIELD_NTP_TIME_SERVICE_HPP_INCLUDED
