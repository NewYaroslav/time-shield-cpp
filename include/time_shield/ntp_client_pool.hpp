// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_NTP_CLIENT_POOL_HPP_INCLUDED
#define TIME_SHIELD_NTP_CLIENT_POOL_HPP_INCLUDED

#include "config.hpp"

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include "ntp_client.hpp"
#include "time_utils.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace time_shield {

    /// \ingroup ntp
    /// \brief NTP measurement sample (one server response).
    struct NtpSample {
        std::string host;
        int         port = 123;

        bool    is_ok = false;
        int     error_code = 0;
        int     stratum = -1;
        int64_t offset_us = 0; ///< UTC - local realtime, microseconds.
        int64_t delay_us  = 0; ///< NTP round-trip delay estimate, microseconds.

        int64_t max_delay_us = 0;
    };

    /// \ingroup ntp
    /// \brief Per-server configuration.
    struct NtpServerConfig {
        std::string host;
        int         port = 123;

        std::chrono::milliseconds min_interval{15000};
        std::chrono::milliseconds max_delay{250};

        std::chrono::milliseconds backoff_initial{15000};
        std::chrono::milliseconds backoff_max{std::chrono::minutes(10)};
    };

    /// \ingroup ntp
    /// \brief Pool configuration.
    struct NtpPoolConfig {
        std::size_t sample_servers = 5;
        std::size_t min_valid_samples = 3;

        enum class Aggregation {
            Median,
            BestDelay,
            MedianMadTrim
        } aggregation = Aggregation::Median;

        double smoothing_alpha = 1.0;
        std::uint64_t rng_seed = 0;
    };

    /// \ingroup ntp
    /// \brief Pool of NTP servers: rate-limited multi-server offset estimation.
    /// \tparam ClientT NTP client type with interface:
    ///         ClientT(const std::string& host, int port);
    ///         bool query(); // may throw
    ///         int last_error_code() const;
    ///         int64_t offset_us() const;
    ///         int64_t delay_us() const;
    ///         int stratum() const;
    template <class ClientT>
    class NtpClientPoolT {
    public:
        explicit NtpClientPoolT(NtpPoolConfig cfg = {})
            : m_cfg(std::move(cfg))
            , m_offset_us(TIME_SHIELD_ATOMIC_INIT(0))
            , m_rng(init_seed(m_cfg.rng_seed)) {}

        NtpClientPoolT(const NtpClientPoolT&) = delete;
        NtpClientPoolT& operator=(const NtpClientPoolT&) = delete;

        NtpClientPoolT(NtpClientPoolT&& other) noexcept
            : m_cfg()
            , m_offset_us(TIME_SHIELD_ATOMIC_INIT(0))
            , m_rng(init_seed(other.m_cfg.rng_seed)) {
            std::lock_guard<std::mutex> lk(other.m_mtx);
            m_cfg = other.m_cfg;
            m_servers = std::move(other.m_servers);
            m_last_samples = std::move(other.m_last_samples);
            m_offset_us.store(other.m_offset_us.load());
            m_rng = std::move(other.m_rng);
        }

        NtpClientPoolT& operator=(NtpClientPoolT&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            std::lock(m_mtx, other.m_mtx);
            std::lock_guard<std::mutex> lk1(m_mtx, std::adopt_lock);
            std::lock_guard<std::mutex> lk2(other.m_mtx, std::adopt_lock);

            m_cfg = other.m_cfg;
            m_servers = std::move(other.m_servers);
            m_last_samples = std::move(other.m_last_samples);
            m_offset_us.store(other.m_offset_us.load());
            m_rng = std::move(other.m_rng);
            return *this;
        }

        /// \brief Replace server list (keeps pool config).
        void set_servers(std::vector<NtpServerConfig> servers) {
            std::lock_guard<std::mutex> lk(m_mtx);
            m_servers.clear();
            m_servers.reserve(servers.size());
            for (auto& server_cfg : servers) {
                ServerState state;
                state.cfg = std::move(server_cfg);
                m_servers.push_back(std::move(state));
            }
        }

        /// \brief Add one server.
        void add_server(NtpServerConfig server_cfg) {
            std::lock_guard<std::mutex> lk(m_mtx);
            ServerState state;
            state.cfg = std::move(server_cfg);
            m_servers.push_back(std::move(state));
        }

        /// \brief Build a conservative default server list.
        static std::vector<NtpServerConfig> build_default_servers() {
            std::vector<NtpServerConfig> servers;
            servers.reserve(160);

            auto add = [&servers](const char* host) {
                NtpServerConfig cfg;
                cfg.host = host;
                cfg.min_interval = std::chrono::milliseconds{60000};
                cfg.max_delay = std::chrono::milliseconds{500};
                cfg.backoff_initial = std::chrono::milliseconds{120000};
                cfg.backoff_max = std::chrono::minutes(10);
                servers.push_back(std::move(cfg));
            };

            add("time.google.com");
            add("time1.google.com");
            add("time2.google.com");
            add("time3.google.com");
            add("time4.google.com");

            add("time.cloudflare.com");

            add("time.facebook.com");
            add("time1.facebook.com");
            add("time2.facebook.com");
            add("time3.facebook.com");
            add("time4.facebook.com");
            add("time5.facebook.com");

            add("time.windows.com");

            add("time.apple.com");
            add("time1.apple.com");
            add("time2.apple.com");
            add("time3.apple.com");
            add("time4.apple.com");
            add("time5.apple.com");
            add("time6.apple.com");
            add("time7.apple.com");
            add("time.euro.apple.com");

            add("time-a-g.nist.gov");
            add("time-b-g.nist.gov");
            add("time-c-g.nist.gov");
            add("time-d-g.nist.gov");
            add("time-a-wwv.nist.gov");
            add("time-b-wwv.nist.gov");
            add("time-c-wwv.nist.gov");
            add("time-d-wwv.nist.gov");
            add("time-a-b.nist.gov");
            add("time-b-b.nist.gov");
            add("time-c-b.nist.gov");
            add("time-d-b.nist.gov");
            add("time.nist.gov");
            add("utcnist.colorado.edu");
            add("utcnist2.colorado.edu");

            add("ntp1.vniiftri.ru");
            add("ntp2.vniiftri.ru");
            add("ntp3.vniiftri.ru");
            add("ntp4.vniiftri.ru");
            add("ntp1.niiftri.irkutsk.ru");
            add("ntp2.niiftri.irkutsk.ru");
            add("vniiftri.khv.ru");
            add("vniiftri2.khv.ru");
            add("ntp21.vniiftri.ru");

            add("ntp.mobatime.ru");

            add("ntp1.stratum1.ru");
            add("ntp2.stratum1.ru");
            add("ntp3.stratum1.ru");
            add("ntp4.stratum1.ru");
            add("ntp5.stratum1.ru");
            add("ntp2.stratum2.ru");
            add("ntp3.stratum2.ru");
            add("ntp4.stratum2.ru");
            add("ntp5.stratum2.ru");

            add("stratum1.net");

            add("ntp.time.in.ua");
            add("ntp2.time.in.ua");
            add("ntp3.time.in.ua");

            add("ntp.ru");

            add("ts1.aco.net");
            add("ts2.aco.net");

            add("ntp1.net.berkeley.edu");
            add("ntp2.net.berkeley.edu");

            add("ntp.gsu.edu");

            add("tick.usask.ca");
            add("tock.usask.ca");

            add("ntp.nsu.ru");
            add("ntp.rsu.edu.ru");

            add("ntp.nict.jp");

            add("x.ns.gin.ntt.net");
            add("y.ns.gin.ntt.net");

            add("clock.nyc.he.net");
            add("clock.sjc.he.net");

            add("ntp.fiord.ru");

            add("gbg1.ntp.se");
            add("gbg2.ntp.se");
            add("mmo1.ntp.se");
            add("mmo2.ntp.se");
            add("sth1.ntp.se");
            add("sth2.ntp.se");
            add("svl1.ntp.se");
            add("svl2.ntp.se");

            add("clock.isc.org");

            add("pool.ntp.org");
            add("0.pool.ntp.org");
            add("1.pool.ntp.org");
            add("2.pool.ntp.org");
            add("3.pool.ntp.org");

            add("europe.pool.ntp.org");
            add("0.europe.pool.ntp.org");
            add("1.europe.pool.ntp.org");
            add("2.europe.pool.ntp.org");
            add("3.europe.pool.ntp.org");

            add("asia.pool.ntp.org");
            add("0.asia.pool.ntp.org");
            add("1.asia.pool.ntp.org");
            add("2.asia.pool.ntp.org");
            add("3.asia.pool.ntp.org");

            add("ru.pool.ntp.org");
            add("0.ru.pool.ntp.org");
            add("1.ru.pool.ntp.org");
            add("2.ru.pool.ntp.org");
            add("3.ru.pool.ntp.org");

            add("0.gentoo.pool.ntp.org");
            add("1.gentoo.pool.ntp.org");
            add("2.gentoo.pool.ntp.org");
            add("3.gentoo.pool.ntp.org");

            add("0.arch.pool.ntp.org");
            add("1.arch.pool.ntp.org");
            add("2.arch.pool.ntp.org");
            add("3.arch.pool.ntp.org");

            add("0.fedora.pool.ntp.org");
            add("1.fedora.pool.ntp.org");
            add("2.fedora.pool.ntp.org");
            add("3.fedora.pool.ntp.org");

            add("0.opensuse.pool.ntp.org");
            add("1.opensuse.pool.ntp.org");
            add("2.opensuse.pool.ntp.org");
            add("3.opensuse.pool.ntp.org");

            add("0.centos.pool.ntp.org");
            add("1.centos.pool.ntp.org");
            add("2.centos.pool.ntp.org");
            add("3.centos.pool.ntp.org");

            add("0.debian.pool.ntp.org");
            add("1.debian.pool.ntp.org");
            add("2.debian.pool.ntp.org");
            add("3.debian.pool.ntp.org");

            add("0.ubuntu.pool.ntp.org");
            add("1.ubuntu.pool.ntp.org");
            add("2.ubuntu.pool.ntp.org");
            add("3.ubuntu.pool.ntp.org");

            add("0.askozia.pool.ntp.org");
            add("1.askozia.pool.ntp.org");
            add("2.askozia.pool.ntp.org");
            add("3.askozia.pool.ntp.org");

            add("0.freebsd.pool.ntp.org");
            add("1.freebsd.pool.ntp.org");
            add("2.freebsd.pool.ntp.org");
            add("3.freebsd.pool.ntp.org");

            add("0.netbsd.pool.ntp.org");
            add("1.netbsd.pool.ntp.org");
            add("2.netbsd.pool.ntp.org");
            add("3.netbsd.pool.ntp.org");

            add("0.openbsd.pool.ntp.org");
            add("1.openbsd.pool.ntp.org");
            add("2.openbsd.pool.ntp.org");
            add("3.openbsd.pool.ntp.org");

            add("0.dragonfly.pool.ntp.org");
            add("1.dragonfly.pool.ntp.org");
            add("2.dragonfly.pool.ntp.org");
            add("3.dragonfly.pool.ntp.org");

            add("0.pfsense.pool.ntp.org");
            add("1.pfsense.pool.ntp.org");
            add("2.pfsense.pool.ntp.org");
            add("3.pfsense.pool.ntp.org");

            add("0.opnsense.pool.ntp.org");
            add("1.opnsense.pool.ntp.org");
            add("2.opnsense.pool.ntp.org");
            add("3.opnsense.pool.ntp.org");

            add("0.smartos.pool.ntp.org");
            add("1.smartos.pool.ntp.org");
            add("2.smartos.pool.ntp.org");
            add("3.smartos.pool.ntp.org");

            add("0.android.pool.ntp.org");
            add("1.android.pool.ntp.org");
            add("2.android.pool.ntp.org");
            add("3.android.pool.ntp.org");

            add("0.amazon.pool.ntp.org");
            add("1.amazon.pool.ntp.org");
            add("2.amazon.pool.ntp.org");
            add("3.amazon.pool.ntp.org");

            return servers;
        }

        /// \brief Replace server list with a conservative default set.
        void set_default_servers() {
            set_servers(build_default_servers());
        }

        /// \brief Clear server list.
        void clear_servers() {
            std::lock_guard<std::mutex> lk(m_mtx);
            m_servers.clear();
        }

        /// \brief Perform measurement using current config (queries up to sample_servers).
        /// \return true if pool offset updated.
        bool measure() {
            const auto cfg = config();
            return measure_n(cfg.sample_servers);
        }

        /// \brief Perform measurement using a custom number of servers.
        bool measure_n(std::size_t servers_to_sample) {
            std::vector<std::size_t> picked;
            NtpPoolConfig cfg;
            {
                std::lock_guard<std::mutex> lk(m_mtx);
                cfg = m_cfg;
                picked = pick_servers_locked(servers_to_sample);
            }

            std::vector<NtpSample> samples;
            samples.reserve(picked.size());

            for (std::size_t idx : picked) {
                samples.push_back(query_one(idx));
            }

            const bool is_updated = update_from_samples(samples, cfg);

            {
                std::lock_guard<std::mutex> lk(m_mtx);
                m_last_samples = std::move(samples);
            }

            return is_updated;
        }

        /// \brief Last estimated pool offset (µs).
        int64_t offset_us() const noexcept { return m_offset_us.load(); }

        /// \brief Current UTC time in microseconds based on pool offset.
        int64_t utc_time_us() const noexcept { return now_realtime_us() + m_offset_us.load(); }

        /// \brief Current UTC time in milliseconds based on pool offset.
        int64_t utc_time_ms() const noexcept { return utc_time_us() / 1000; }

        /// \brief Returns last measurement samples (copy).
        std::vector<NtpSample> last_samples() const {
            std::lock_guard<std::mutex> lk(m_mtx);
            return m_last_samples;
        }

        /// \brief Apply pre-collected samples (testing/offline).
        /// \note Primarily for tests; does not enforce rate limiting or backoff.
        bool apply_samples(const std::vector<NtpSample>& samples) {
            const NtpPoolConfig cfg = config();
            const bool is_updated = update_from_samples(samples, cfg);
            std::lock_guard<std::mutex> lk(m_mtx);
            m_last_samples = samples;
            return is_updated;
        }

        /// \brief Returns median of values.
        static int64_t median(std::vector<int64_t>& values) {
            using diff_t = std::vector<int64_t>::difference_type;
            const diff_t mid_index = static_cast<diff_t>(values.size() / 2);
            std::nth_element(values.begin(), values.begin() + mid_index, values.end());
            const int64_t mid = values[static_cast<std::size_t>(mid_index)];
            if (values.size() % 2 == 1) {
                return mid;
            }

            const auto it = std::max_element(values.begin(), values.begin() + mid_index);
            return (*it + mid) / 2;
        }

        /// \brief Median with MAD trimming.
        static int64_t median_mad_trim(std::vector<int64_t>& offsets) {
            const int64_t med = median(offsets);

            std::vector<int64_t> deviations;
            deviations.reserve(offsets.size());
            for (auto value : offsets) {
                deviations.push_back(value > med ? (value - med) : (med - value));
            }

            const int64_t mad = median(deviations);
            if (mad == 0) {
                return med;
            }

            const int64_t threshold = mad * 3;
            std::vector<int64_t> kept;
            kept.reserve(offsets.size());
            for (auto value : offsets) {
                const int64_t deviation = value > med ? (value - med) : (med - value);
                if (deviation <= threshold) {
                    kept.push_back(value);
                }
            }
            if (kept.empty()) {
                return med;
            }
            return median(kept);
        }

        /// \brief Offset from best (lowest) delay sample.
        static int64_t best_delay_offset(const std::vector<NtpSample>& samples) {
            const NtpSample* best = nullptr;
            for (const auto& sample : samples) {
                if (!sample.is_ok) {
                    continue;
                }
                if (sample.max_delay_us > 0 && sample.delay_us > sample.max_delay_us) {
                    continue;
                }
                if (best == nullptr) {
                    best = &sample;
                    continue;
                }
                if (sample.delay_us > 0 && best->delay_us > 0 && sample.delay_us < best->delay_us) {
                    best = &sample;
                }
            }
            return best ? best->offset_us : 0;
        }

        /// \brief Access config.
        NtpPoolConfig config() const {
            std::lock_guard<std::mutex> lk(m_mtx);
            return m_cfg;
        }
        void set_config(NtpPoolConfig cfg) {
            std::lock_guard<std::mutex> lk(m_mtx);
            m_cfg = std::move(cfg);
        }

        struct ServerState {
            NtpServerConfig cfg;

            std::chrono::steady_clock::time_point next_allowed{};
            std::chrono::milliseconds backoff{0};

            int fail_count = 0;

            int64_t last_offset_us = 0;
            int64_t last_delay_us = 0;
            int     last_error = 0;
            bool    is_last_ok = false;
        };

        NtpPoolConfig m_cfg;

        mutable std::mutex m_mtx;
        std::vector<ServerState> m_servers;
        std::vector<NtpSample>   m_last_samples;

        std::atomic<int64_t> m_offset_us;

        std::mt19937_64 m_rng;

    private:
        static std::uint64_t init_seed(std::uint64_t seed) {
            if (seed != 0) return seed;
            const auto v = static_cast<std::uint64_t>(
                std::chrono::high_resolution_clock::now().time_since_epoch().count());
            return v ^ 0x9E3779B97F4A7C15ULL;
        }

        std::vector<std::size_t> pick_servers_locked(std::size_t servers_to_sample) {
            std::vector<std::size_t> eligible;
            eligible.reserve(m_servers.size());

            const auto now_point = std::chrono::steady_clock::now();
            for (std::size_t i = 0; i < m_servers.size(); ++i) {
                if (now_point >= m_servers[i].next_allowed) {
                    eligible.push_back(i);
                }
            }

            if (eligible.empty()) {
                return {};
            }

            std::shuffle(eligible.begin(), eligible.end(), m_rng);
            if (servers_to_sample < eligible.size()) {
                eligible.resize(servers_to_sample);
            }
            return eligible;
        }

        NtpSample query_one(std::size_t server_index) {
            NtpServerConfig cfg;
            {
                std::lock_guard<std::mutex> lk(m_mtx);
                cfg = m_servers[server_index].cfg;
                m_servers[server_index].next_allowed =
                    std::chrono::steady_clock::now() + cfg.min_interval;
            }

            NtpSample out;
            out.host = cfg.host;
            out.port = cfg.port;
            out.max_delay_us = cfg.max_delay.count() > 0 ? cfg.max_delay.count() * 1000 : 0;

            ClientT client(cfg.host, cfg.port);

            bool is_ok = false;
            try {
                is_ok = client.query();
            } catch (...) {
                out.error_code = client.last_error_code();
            }

            if (out.error_code == 0) {
                out.error_code = client.last_error_code();
            }
            if (out.error_code == 0 && !is_ok) {
                out.error_code = -1;
            }

            out.is_ok = is_ok;
            out.offset_us = client.offset_us();
            out.delay_us = client.delay_us();
            out.stratum = client.stratum();

            update_server_state_after_query(server_index, out);
            return out;
        }

        void update_server_state_after_query(std::size_t index, const NtpSample& sample) {
            std::lock_guard<std::mutex> lk(m_mtx);
            auto& state = m_servers[index];

            state.is_last_ok = sample.is_ok;
            state.last_error = sample.error_code;
            state.last_offset_us = sample.offset_us;
            state.last_delay_us = sample.delay_us;

            if (sample.is_ok) {
                state.fail_count = 0;
                state.backoff = std::chrono::milliseconds(0);
                return;
            }

            state.fail_count++;
            const auto init = state.cfg.backoff_initial;
            const auto max_backoff = state.cfg.backoff_max;

            if (state.backoff.count() == 0) {
                state.backoff = init;
            } else {
                state.backoff = std::min(max_backoff, state.backoff * 2);
            }

            state.next_allowed = std::chrono::steady_clock::now() + state.backoff;
        }

        bool update_from_samples(const std::vector<NtpSample>& samples, const NtpPoolConfig& cfg) {
            std::vector<int64_t> offsets;
            offsets.reserve(samples.size());

            for (const auto& sample : samples) {
                if (!sample.is_ok) {
                    continue;
                }
                if (sample.max_delay_us > 0 && sample.delay_us > sample.max_delay_us) {
                    continue;
                }
                offsets.push_back(sample.offset_us);
            }

            if (offsets.size() < cfg.min_valid_samples) {
                return false;
            }

            int64_t estimate = 0;
            switch (cfg.aggregation) {
            case NtpPoolConfig::Aggregation::BestDelay:
                estimate = best_delay_offset(samples);
                break;
            case NtpPoolConfig::Aggregation::MedianMadTrim:
                estimate = median_mad_trim(offsets);
                break;
            case NtpPoolConfig::Aggregation::Median:
            default:
                estimate = median(offsets);
                break;
            }

            double alpha = cfg.smoothing_alpha;
            if (alpha < 0.0) {
                alpha = 0.0;
            } else if (alpha > 1.0) {
                alpha = 1.0;
            }
            if (alpha >= 1.0) {
                m_offset_us.store(estimate);
            } else if (alpha > 0.0) {
                const int64_t old_value = m_offset_us.load();
                const double new_value =
                    (1.0 - alpha) * static_cast<double>(old_value) + alpha * static_cast<double>(estimate);
                m_offset_us.store(static_cast<int64_t>(new_value));
            }
            return true;
        }

    };

    using NtpClientPool = NtpClientPoolT<NtpClient>;
} // namespace time_shield

#else // TIME_SHIELD_ENABLE_NTP_CLIENT

namespace time_shield {
    class NtpClientPool {
    public:
        NtpClientPool() {
            static_assert(sizeof(void*) == 0, "NtpClientPool is disabled by configuration.");
        }
    };
} // namespace time_shield

#endif // TIME_SHIELD_ENABLE_NTP_CLIENT

#endif // TIME_SHIELD_NTP_CLIENT_POOL_HPP_INCLUDED
