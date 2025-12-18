// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_NTP_CLIENT_HPP_INCLUDED
#define _TIME_SHIELD_NTP_CLIENT_HPP_INCLUDED

/// \file ntp_client.hpp
/// \brief Simple NTP client for querying time offset from NTP servers.
///
/// This module provides a minimal client that can query a remote NTP server over UDP
/// and calculate the offset between local system time and the NTP-reported time.
///
/// The feature is optional and controlled by `TIME_SHIELD_ENABLE_NTP_CLIENT`.
/// \ingroup ntp

#include "config.hpp"

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include "time_utils.hpp"
#include "ntp_client/ntp_client_core.hpp"
#include "ntp_client/ntp_packet.hpp"
#include "ntp_client/udp_transport.hpp"
#include "ntp_client/udp_transport_win.hpp"
#include "ntp_client/udp_transport_posix.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <string>

#if TIME_SHIELD_PLATFORM_WINDOWS

#include "ntp_client/wsa_guard.hpp"

namespace time_shield {

    /// \ingroup ntp
    /// \brief Windows implementation of the NTP client for measuring time offset.
    class NtpClient {
    public:
        /// \brief Constructs NTP client with specified host and port.
        NtpClient(std::string server = "pool.ntp.org", int port = 123)
            : m_host(std::move(server))
            , m_port(port)
            , m_offset_us(TIME_SHIELD_ATOMIC_INIT(0))
            , m_delay_us(TIME_SHIELD_ATOMIC_INIT(0))
            , m_stratum(TIME_SHIELD_ATOMIC_INIT(-1))
            , m_is_success(TIME_SHIELD_ATOMIC_INIT(false)) {
            now_realtime_us();
        }

        /// \brief Queries the NTP server and updates the local offset.
        /// \return true if successful.
        bool query() {
            last_error_code_slot() = 0;
            detail::UdpTransportWin transport;
            detail::NtpClientCore core;

            int error_code = 0;
            int64_t offset = 0;
            int64_t delay = 0;
            int stratum = -1;

            const bool ok = core.query(transport,
                                       m_host,
                                       m_port,
                                       k_default_timeout_ms,
                                       error_code,
                                       offset,
                                       delay,
                                       stratum);

            if (last_error_code_slot() == 0) {
                last_error_code_slot() = error_code;
            }

            if (!ok) {
                m_delay_us = 0;
                m_stratum = -1;
                m_is_success = false;
                return false;
            }

            m_offset_us = offset;
            m_delay_us = delay;
            m_stratum = stratum;
            m_is_success = true;
            return true;
        }

        /// \brief Returns whether the last NTP query was successful.
        /// \return True if the last offset measurement succeeded.
        bool success() const noexcept {
            return m_is_success.load();
        }

        /// \brief Returns the last measured offset in microseconds.
        int64_t offset_us() const noexcept {
            return m_offset_us;
        }

        /// \brief Returns the last measured delay in microseconds.
        int64_t delay_us() const noexcept {
            return m_delay_us;
        }

        /// \brief Returns the last received stratum value.
        int stratum() const noexcept {
            return m_stratum;
        }

        /// \brief Returns current UTC time in microseconds based on last NTP offset.
        /// \return Current UTC time in µs.
        int64_t utc_time_us() const noexcept {
            const int64_t offset = m_offset_us.load();
            return now_realtime_us() + offset;
        }

        /// \brief Returns current UTC time in milliseconds based on last NTP offset.
        /// \return Current UTC time in ms.
        int64_t utc_time_ms() const noexcept {
            return utc_time_us() / 1000;
        }

        /// \brief Returns current UTC time as time_t (seconds since Unix epoch).
        /// \return UTC time in seconds.
        time_t utc_time_sec() const noexcept {
            return static_cast<time_t>(utc_time_us() / 1000000);
        }

        /// \brief Returns last WinSock error code (if any).
        int last_error_code() const noexcept {
            return last_error_code_slot();
        }
        
    private:
        static constexpr int64_t NTP_TIMESTAMP_DELTA = 2208988800ll; ///< Seconds between 1900 and 1970 epochs.

        /// \brief Структура пакета NTP
        /// Total: 384 bits or 48 bytes.
        struct ntp_packet {
            uint8_t li_vn_mode;       // Eight bits. li, vn, and mode.
                                      // li. Two bits. Leap indicator.
                                      // vn. Three bits. Version number of the protocol.
                                      // mode. Three bits. Client will pick mode 3 for client.
            uint8_t stratum;          // Eight bits. Stratum level of the local clock.
            uint8_t poll;             // Eight bits. Maximum interval between successive messages.
            uint8_t precision;        // Eight bits. Precision of the local clock. 
            uint32_t root_delay;      // 32 bits. Total round trip delay time.
            uint32_t root_dispersion; // 32 bits. Max error aloud from primary clock source. 
            uint32_t ref_id;          // 32 bits. Reference clock identifier.
            uint32_t ref_ts_sec;      // 32 bits. Reference time-stamp seconds.
            uint32_t ref_ts_frac;     // 32 bits. Reference time-stamp fraction of a second.
            uint32_t orig_ts_sec;     // 32 bits. Originate time-stamp seconds.
            uint32_t orig_ts_frac;    // 32 bits. Originate time-stamp fraction of a second.
            uint32_t recv_ts_sec;     // 32 bits. Received time-stamp seconds.
            uint32_t recv_ts_frac;    // 32 bits. Received time-stamp fraction of a second.
            uint32_t tx_ts_sec;       // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
            uint32_t tx_ts_frac;      // 32 bits. Transmit time-stamp fraction of a second.
        };

        std::string          m_host;
        int                  m_port;
        std::atomic<int64_t> m_offset_us;
        std::atomic<int64_t> m_delay_us;
        std::atomic<int>     m_stratum;
        std::atomic<bool>    m_is_success;

        static int& last_error_code_slot() noexcept {
            static TIME_SHIELD_THREAD_LOCAL int value = 0;
            return value;
        }
        
        static bool get_now_us_u64(uint64_t& out) noexcept {
            const int64_t v = time_shield::now_realtime_us();
            if (v < 0) return false;
            out = static_cast<uint64_t>(v);
            return true;
        }

        static uint64_t ntp_frac_to_us(uint32_t frac_net) noexcept {
            // NTP frac: unsigned 32-bit fraction of second with denominator 2^32.
            const uint64_t frac = static_cast<uint64_t>(ntohl(frac_net));
            return (frac * 1000000ULL) >> 32; // / 2^32
        }

        static bool ntp_ts_to_unix_us(uint32_t sec_net, uint32_t frac_net, uint64_t& out_us) noexcept {
            const int64_t sec = static_cast<int64_t>(ntohl(sec_net)) - NTP_TIMESTAMP_DELTA;
            if (sec < 0) return false; // время до 1970 — не ожидается, но защитимся
            out_us = static_cast<uint64_t>(sec) * 1000000ULL + ntp_frac_to_us(frac_net);
            return true;
        }

        /// \brief Converts local time to NTP timestamp format.
        void fill_packet(ntp_packet& pkt) const {
            std::memset(&pkt, 0, sizeof(pkt));
            pkt.li_vn_mode = (0 << 6) | (3 << 3) | 3; // LI=0, VN=3, Mode=3 (client)

            uint64_t now_us = 0;
            if (!get_now_us_u64(now_us)) {
                pkt.tx_ts_sec = 0;
                pkt.tx_ts_frac = 0;
                return;
            }
    
            const uint64_t sec = now_us / 1000000 + static_cast<uint64_t>(NTP_TIMESTAMP_DELTA);
            const uint64_t frac = ((now_us % 1000000) * 0x100000000ULL) / 1000000;

            pkt.tx_ts_sec  = htonl(static_cast<uint32_t>(sec));
            pkt.tx_ts_frac = htonl(static_cast<uint32_t>(frac));
        }

        /// \brief Parses response and calculates offset and delay.
        bool parse_packet(const ntp_packet& pkt, int64_t& result_offset_us, int64_t& result_delay_us,
                          int& result_stratum) const {
            uint64_t arrival_us = 0;
            if (!get_now_us_u64(arrival_us)) return false;

            uint64_t originate_us = 0, receive_us = 0, transmit_us = 0;

            if (!ntp_ts_to_unix_us(pkt.orig_ts_sec, pkt.orig_ts_frac, originate_us)) return false;
            if (!ntp_ts_to_unix_us(pkt.recv_ts_sec, pkt.recv_ts_frac, receive_us)) return false;
            if (!ntp_ts_to_unix_us(pkt.tx_ts_sec,   pkt.tx_ts_frac,   transmit_us)) return false;

            // RFC 5905: offset = ((t2 - t1) + (t3 - t4)) / 2
            const int64_t t1 = static_cast<int64_t>(originate_us);
            const int64_t t2 = static_cast<int64_t>(receive_us);
            const int64_t t3 = static_cast<int64_t>(transmit_us);
            const int64_t t4 = static_cast<int64_t>(arrival_us);

            result_offset_us = ((t2 - t1) + (t3 - t4)) / 2;
            result_delay_us = (t4 - t1) - (t3 - t2);
            result_stratum = pkt.stratum;
            return true;
        }

    };
    
} // namespace time_shield

#elif TIME_SHIELD_PLATFORM_UNIX

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>

namespace time_shield {

    /// \ingroup ntp
    /// \brief Simple POSIX NTP client for measuring time offset.
    class NtpClient {
    public:
        /// \brief Constructs NTP client with specified host and port.
        NtpClient(std::string server = "pool.ntp.org", int port = 123)
            : m_host(std::move(server))
            , m_port(port)
            , m_offset_us(TIME_SHIELD_ATOMIC_INIT(0))
            , m_delay_us(TIME_SHIELD_ATOMIC_INIT(0))
            , m_stratum(TIME_SHIELD_ATOMIC_INIT(-1))
            , m_is_success(TIME_SHIELD_ATOMIC_INIT(false)) {
            now_realtime_us();
        }

        /// \brief Queries the NTP server and updates the local offset.
        /// \return true if successful.
        bool query() {
            last_error_code_slot() = 0;

            detail::UdpTransportPosix transport;
            detail::NtpClientCore core;

            int error_code = 0;
            int64_t offset = 0;
            int64_t delay = 0;
            int stratum = -1;

            const bool ok = core.query(transport,
                                       m_host,
                                       m_port,
                                       k_default_timeout_ms,
                                       error_code,
                                       offset,
                                       delay,
                                       stratum);

            if (last_error_code_slot() == 0) {
                last_error_code_slot() = error_code;
            }

            if (!ok) {
                m_delay_us = 0;
                m_stratum = -1;
                m_is_success = false;
                return false;
            }

            m_offset_us = offset;
            m_delay_us = delay;
            m_stratum = stratum;
            m_is_success = true;
            return true;
        }

        /// \brief Returns whether the last NTP query was successful.
        /// \return True if the last offset measurement succeeded.
        bool success() const noexcept {
            return m_is_success.load();
        }

        /// \brief Returns the last measured offset in microseconds.
        int64_t offset_us() const noexcept {
            return m_offset_us;
        }

        /// \brief Returns the last measured delay in microseconds.
        int64_t delay_us() const noexcept {
            return m_delay_us;
        }

        /// \brief Returns the last received stratum value.
        int stratum() const noexcept {
            return m_stratum;
        }

        /// \brief Returns current NTP UTC timestamp in microseconds.
        /// \return UTC time in microseconds.
        int64_t utc_time_us() const noexcept {
            uint64_t system_time_us = 0;
            if (!get_now_us_u64(system_time_us)) return 0;
            return static_cast<int64_t>(system_time_us) + m_offset_us.load();
        }

        /// \brief Returns current NTP UTC timestamp in milliseconds.
        /// \return UTC time in milliseconds.
        int64_t utc_time_ms() const noexcept {
            return utc_time_us() / 1000;
        }

        /// \brief Returns current UTC time as time_t (seconds since Unix epoch).
        /// \return UTC time in seconds.
        time_t utc_time_sec() const noexcept {
            return static_cast<time_t>(utc_time_us() / 1000000);
        }

        /// \brief Returns last socket error code (if any).
        int last_error_code() const noexcept {
            return last_error_code_slot();
        }

    private:
        std::string          m_host;
        int                  m_port;
        std::atomic<int64_t> m_offset_us;
        std::atomic<int64_t> m_delay_us;
        std::atomic<int>     m_stratum;
        std::atomic<bool>    m_is_success;
        static const int k_default_timeout_ms = 5000;

        static int& last_error_code_slot() noexcept {
            static TIME_SHIELD_THREAD_LOCAL int value = 0;
            return value;
        }

        static bool get_now_us_u64(uint64_t& out) noexcept {
            const int64_t v = time_shield::now_realtime_us();
            if (v < 0) return false;
            out = static_cast<uint64_t>(v);
            return true;
        }

        static uint64_t ntp_frac_to_us(uint32_t frac_net) noexcept {
            // NTP frac: unsigned 32-bit fraction of second with denominator 2^32.
            const uint64_t frac = static_cast<uint64_t>(ntohl(frac_net));
            return (frac * 1000000ULL) >> 32; // / 2^32
        }

        static bool ntp_ts_to_unix_us(uint32_t sec_net, uint32_t frac_net, uint64_t& out_us) noexcept {
            const int64_t sec = static_cast<int64_t>(ntohl(sec_net)) - NTP_TIMESTAMP_DELTA;
            if (sec < 0) return false;
            out_us = static_cast<uint64_t>(sec) * 1000000ULL + ntp_frac_to_us(frac_net);
            return true;
        }

        /// \brief Converts local time to NTP timestamp format.
        void fill_packet(ntp_packet& pkt) const {
            std::memset(&pkt, 0, sizeof(pkt));
            pkt.li_vn_mode = (0 << 6) | (3 << 3) | 3; // LI=0, VN=3, Mode=3 (client)

            uint64_t now_us = 0;
            if (!get_now_us_u64(now_us)) {
                pkt.tx_ts_sec = 0;
                pkt.tx_ts_frac = 0;
                return;
            }

            const uint64_t sec = now_us / 1000000 + static_cast<uint64_t>(NTP_TIMESTAMP_DELTA);
            const uint64_t frac = ((now_us % 1000000) * 0x100000000ULL) / 1000000;

            pkt.tx_ts_sec  = htonl(static_cast<uint32_t>(sec));
            pkt.tx_ts_frac = htonl(static_cast<uint32_t>(frac));
        }

        /// \brief Parses response and calculates offset and delay.
        bool parse_packet(const ntp_packet& pkt, int64_t& result_offset_us, int64_t& result_delay_us,
                          int& result_stratum) const {
            uint64_t arrival_us = 0;
            if (!get_now_us_u64(arrival_us)) return false;

            uint64_t originate_us = 0, receive_us = 0, transmit_us = 0;

            if (!ntp_ts_to_unix_us(pkt.orig_ts_sec, pkt.orig_ts_frac, originate_us)) return false;
            if (!ntp_ts_to_unix_us(pkt.recv_ts_sec, pkt.recv_ts_frac, receive_us)) return false;
            if (!ntp_ts_to_unix_us(pkt.tx_ts_sec,   pkt.tx_ts_frac,   transmit_us)) return false;

            // RFC 5905: offset = ((t2 - t1) + (t3 - t4)) / 2
            const int64_t t1 = static_cast<int64_t>(originate_us);
            const int64_t t2 = static_cast<int64_t>(receive_us);
            const int64_t t3 = static_cast<int64_t>(transmit_us);
            const int64_t t4 = static_cast<int64_t>(arrival_us);

            result_offset_us = ((t2 - t1) + (t3 - t4)) / 2;
            result_delay_us = (t4 - t1) - (t3 - t2);
            result_stratum = pkt.stratum;
            return true;
        }

    };

} // namespace time_shield

#else // TIME_SHIELD_ENABLE_NTP_CLIENT but unsupported platform

#   warning "NtpClient is unsupported on this platform."

namespace time_shield {

    /// \brief Placeholder used when NTP client platform support is missing.
    class NtpClient {
    public:
        NtpClient() {
            static_assert(sizeof(void*) == 0, "time_shield::NtpClient is unsupported on this platform.");
        }
    };

} // namespace time_shield

#endif // TIME_SHIELD_PLATFORM_WINDOWS/UNIX

#else // !TIME_SHIELD_ENABLE_NTP_CLIENT

#   warning "NtpClient is disabled or unsupported on this platform."

namespace time_shield {

    /// \brief Placeholder used when NTP client is disabled.
    class NtpClient {
    public:
        NtpClient() {
            static_assert(sizeof(void*) == 0, "time_shield::NtpClient is disabled by configuration.");
        }
    };

} // namespace time_shield

#endif // TIME_SHIELD_ENABLE_NTP_CLIENT

#endif // _TIME_SHIELD_NTP_CLIENT_HPP_INCLUDED
