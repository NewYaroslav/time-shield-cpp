// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_NTP_CLIENT_CORE_HPP_INCLUDED
#define TIME_SHIELD_NTP_CLIENT_CORE_HPP_INCLUDED

#include "ntp_packet.hpp"
#include "udp_transport.hpp"

#include <cstdint>
#include <string>

namespace time_shield {
namespace detail {

    class NtpClientCore {
    public:
        bool query(IUdpTransport& transport,
                   const std::string& host,
                   int port,
                   int timeout_ms,
                   int& out_error_code,
                   int64_t& out_offset_us,
                   int64_t& out_delay_us,
                   int& out_stratum) noexcept {
            out_error_code = 0;
            out_offset_us = 0;
            out_delay_us = 0;
            out_stratum = -1;

            uint64_t now_us = 0;
            if (!get_now_us(now_us)) {
                out_error_code = -1;
                return false;
            }

            NtpPacket pkt{};
            fill_client_packet(pkt, now_us);

            NtpPacket reply{};
            UdpRequest req;
            req.host = host;
            req.port = port;
            req.send_data = &pkt;
            req.send_size = sizeof(pkt);
            req.recv_data = &reply;
            req.recv_size = sizeof(reply);
            req.timeout_ms = timeout_ms;

            if (!transport.transact(req, out_error_code)) {
                if (out_error_code == 0) {
                    out_error_code = -1;
                }
                return false;
            }

            uint64_t arrival_us = 0;
            if (!get_now_us(arrival_us)) {
                out_error_code = -1;
                return false;
            }

            if (!parse_server_packet(reply, arrival_us, out_offset_us, out_delay_us, out_stratum, out_error_code)) {
                if (out_error_code == 0) {
                    out_error_code = -1;
                }
                return false;
            }

            return true;
        }

    private:
        static bool get_now_us(uint64_t& out) noexcept {
            const int64_t v = time_shield::now_realtime_us();
            if (v < 0) return false;
            out = static_cast<uint64_t>(v);
            return true;
        }
    };

} // namespace detail
} // namespace time_shield

#endif // TIME_SHIELD_NTP_CLIENT_CORE_HPP_INCLUDED
