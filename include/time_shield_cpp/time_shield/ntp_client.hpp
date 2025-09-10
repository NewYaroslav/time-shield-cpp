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

#include "ntp_client/wsa_guard.hpp"
#include "time_utils.hpp"

#include <cstdint>
#include <cstring>
#include <string>
#include <mutex>
#include <atomic>
#include <functional>
#include <chrono>

namespace time_shield {

    /// \ingroup ntp
    /// \brief Simple Windows-only NTP client for measuring time offset.
    class NtpClient {
    public:
        /// \brief Constructs NTP client with specified host and port.
        NtpClient(std::string server = "pool.ntp.org", int port = 123)
            : m_host(std::move(server)), m_port(port) {
            now_realtime_us();
        }

        /// \brief Queries the NTP server and updates the local offset.
        /// \return true if successful.
        bool query() {
            s_last_error_code = 0;
            if (!WsaGuard::instance().success()) {
                m_is_success = false;
                throw std::runtime_error("WSAStartup failed with error: " + std::to_string(WsaGuard::instance().ret_code()));
            }

            SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (sock == INVALID_SOCKET) {
                m_is_success = false;
                return false;
            }

            struct sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(static_cast<u_short>(m_port));
            
            addrinfo hints{}, *res = nullptr;
            hints.ai_family = AF_INET; // IPv4
            hints.ai_socktype = SOCK_DGRAM;
            hints.ai_protocol = IPPROTO_UDP;

            if (getaddrinfo(m_host.c_str(), nullptr, &hints, &res) != 0 || !res) {
                s_last_error_code = WSAGetLastError();
                closesocket(sock);
                m_is_success = false;
                return false;
            }

            sockaddr_in* resolved = reinterpret_cast<sockaddr_in*>(res->ai_addr);
            addr.sin_addr = resolved->sin_addr;
            freeaddrinfo(res);

            ntp_packet pkt;
            fill_packet(pkt);

            int timeout_ms = 5000;
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout_ms), sizeof(timeout_ms));
            if (sendto(sock, reinterpret_cast<const char*>(&pkt), sizeof(pkt), 0,
                       reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
                s_last_error_code = WSAGetLastError();
                closesocket(sock);
                m_is_success = false;
                return false;
            }

            sockaddr_in from;
            int from_len = sizeof(from);
            if (recvfrom(sock, reinterpret_cast<char*>(&pkt), sizeof(pkt), 0,
                         reinterpret_cast<sockaddr*>(&from), &from_len) < 0) {
                s_last_error_code = WSAGetLastError();
                closesocket(sock);
                m_is_success = false;
                return false;
            }

            closesocket(sock);

            int64_t result_offset;
            if (parse_packet(pkt, result_offset)) {
                m_offset_us = result_offset;
                m_is_success = true;
                return true;
            }

            m_is_success = false;
            return false;
        }
        
        /// \brief Returns whether the last NTP query was successful.
        /// \return True if the last offset measurement succeeded.
        bool success() const noexcept {
            return m_is_success.load();
        }

        /// \brief Returns the last measured offset in microseconds.
        int64_t get_offset_us() const noexcept {
            return m_offset_us;
        }
        
        /// \brief Returns current UTC time in microseconds based on last NTP offset.
        /// \return Current UTC time in µs.
        int64_t get_utc_time_us() const noexcept {
            const int64_t offset = m_offset_us.load();
            return now_realtime_us() + offset;
        }
        
        /// \brief Returns current UTC time in milliseconds based on last NTP offset.
        /// \return Current UTC time in ms.
        int64_t get_utc_time_ms() const noexcept {
            return get_utc_time_us() / 1000;
        }

        /// \brief Returns current UTC time as time_t (seconds since Unix epoch).
        /// \return UTC time in seconds.
        time_t get_utc_time() const noexcept {
            return static_cast<time_t>(get_utc_time_us() / 1000000);
        }
        
        /// \brief Returns last WinSock error code (if any).
        int get_last_error_code() const noexcept {
            return s_last_error_code;
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

        std::string              m_host;
        int                      m_port = 123;
        std::atomic<int64_t>     m_offset_us{0};
        std::atomic<bool>        m_is_success{false};
        static thread_local int  s_last_error_code;

        /// \brief Converts local time to NTP timestamp format.
        void fill_packet(ntp_packet& pkt) const {
            std::memset(&pkt, 0, sizeof(pkt));
            pkt.li_vn_mode = (0 << 6) | (3 << 3) | 3; // LI=0, VN=3, Mode=3 (client)

            const uint64_t now_us = time_shield::now_realtime_us();
            const uint64_t sec = now_us / 1000000 + NTP_TIMESTAMP_DELTA;
            const uint64_t frac = ((now_us % 1000000) * 0x100000000ULL) / 1000000;

            pkt.tx_ts_sec  = htonl(static_cast<uint32_t>(sec));
            pkt.tx_ts_frac = htonl(static_cast<uint32_t>(frac));
        }

        /// \brief Parses response and calculates offset.
        bool parse_packet(const ntp_packet& pkt, int64_t& result_offset_us) const {
            const uint64_t arrival_us = time_shield::now_realtime_us();

            const uint64_t originate_us = ((static_cast<uint64_t>(ntohl(pkt.orig_ts_sec)) - NTP_TIMESTAMP_DELTA) * 1000000) +
                                           (static_cast<uint64_t>(ntohl(pkt.orig_ts_frac)) * 1000000 / 0xFFFFFFFFull);
            const uint64_t receive_us = ((static_cast<uint64_t>(ntohl(pkt.recv_ts_sec)) - NTP_TIMESTAMP_DELTA) * 1000000) +
                                         (static_cast<uint64_t>(ntohl(pkt.recv_ts_frac)) * 1000000 / 0xFFFFFFFFull);
            const uint64_t transmit_us = ((static_cast<uint64_t>(ntohl(pkt.tx_ts_sec)) - NTP_TIMESTAMP_DELTA) * 1000000) +
                                          (static_cast<uint64_t>(ntohl(pkt.tx_ts_frac)) * 1000000 / 0xFFFFFFFFull);

            // RFC 5905
            result_offset_us = ((static_cast<int64_t>(receive_us) - static_cast<int64_t>(originate_us)) 
                             + (static_cast<int64_t>(transmit_us) - static_cast<int64_t>(arrival_us))) / 2;
            return true;
        }
    };
    
    thread_local int NtpClient::s_last_error_code = 0;

} // namespace time_shield

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
