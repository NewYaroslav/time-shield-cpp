// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_NTP_CLIENT_HPP_INCLUDED
#define _TIME_SHIELD_NTP_CLIENT_HPP_INCLUDED

/// \file ntp_client.hpp
/// \brief Simple NTP client for querying time offset from NTP servers.
///
/// The feature is optional and controlled by `TIME_SHIELD_ENABLE_NTP_CLIENT`.
/// \ingroup ntp

#include "config.hpp"

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include "time_utils.hpp"
#include "ntp_client/ntp_client_core.hpp"
#include "ntp_client/ntp_packet.hpp"
#include "ntp_client/udp_transport.hpp"

#if TIME_SHIELD_PLATFORM_WINDOWS
#   include "ntp_client/wsa_guard.hpp"
#   include "ntp_client/udp_transport_win.hpp"
#elif TIME_SHIELD_PLATFORM_UNIX
#   include "ntp_client/udp_transport_posix.hpp"
#endif

#include <atomic>
#include <cstdint>
#include <string>

namespace time_shield {

#if TIME_SHIELD_PLATFORM_WINDOWS
    namespace detail { using PlatformUdpTransport = UdpTransportWin; }
#elif TIME_SHIELD_PLATFORM_UNIX
    namespace detail { using PlatformUdpTransport = UdpTransportPosix; }
#endif

#if TIME_SHIELD_PLATFORM_WINDOWS || TIME_SHIELD_PLATFORM_UNIX

    /// \ingroup ntp
    /// \brief NTP client for measuring time offset.
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

#if TIME_SHIELD_PLATFORM_WINDOWS
            if (!WsaGuard::instance().success()) {
                last_error_code_slot() = WsaGuard::instance().ret_code();
                m_is_success = false;
                return false;
            }
#endif

            detail::PlatformUdpTransport transport;
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
        bool success() const noexcept { return m_is_success.load(); }

        /// \brief Returns the last measured offset in microseconds.
        int64_t offset_us() const noexcept { return m_offset_us; }

        /// \brief Returns the last measured delay in microseconds.
        int64_t delay_us() const noexcept { return m_delay_us; }

        /// \brief Returns the last received stratum value.
        int stratum() const noexcept { return m_stratum; }

        /// \brief Returns current UTC time in microseconds based on last NTP offset.
        int64_t utc_time_us() const noexcept { return now_realtime_us() + m_offset_us.load(); }

        /// \brief Returns current UTC time in milliseconds based on last NTP offset.
        int64_t utc_time_ms() const noexcept { return utc_time_us() / 1000; }

        /// \brief Returns current UTC time as time_t (seconds since Unix epoch).
        time_t utc_time_sec() const noexcept { return static_cast<time_t>(utc_time_us() / 1000000); }

        /// \brief Returns last socket error code (if any).
        int last_error_code() const noexcept { return last_error_code_slot(); }

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
    };

#else

    class NtpClient {
    public:
        NtpClient() {
            static_assert(sizeof(void*) == 0, "NtpClient is disabled by configuration.");
        }
    };

#endif // platform switch

} // namespace time_shield

#else // TIME_SHIELD_ENABLE_NTP_CLIENT

namespace time_shield {
    class NtpClient {
    public:
        NtpClient() {
            static_assert(sizeof(void*) == 0, "NtpClient is disabled by configuration.");
        }
    };
} // namespace time_shield

#endif // TIME_SHIELD_ENABLE_NTP_CLIENT

#endif // _TIME_SHIELD_NTP_CLIENT_HPP_INCLUDED
