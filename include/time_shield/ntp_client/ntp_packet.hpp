// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_NTP_PACKET_HPP_INCLUDED
#define TIME_SHIELD_NTP_PACKET_HPP_INCLUDED

#include <cstdint>
#include <cstring>

#if TIME_SHIELD_PLATFORM_WINDOWS
#   include <winsock2.h>
#else
#   include <arpa/inet.h>
#endif

namespace time_shield {
namespace detail {

    /// \brief NTP packet layout (48 bytes).
    struct NtpPacket {
        uint8_t li_vn_mode;
        uint8_t stratum;
        uint8_t poll;
        uint8_t precision;
        uint32_t root_delay;
        uint32_t root_dispersion;
        uint32_t ref_id;
        uint32_t ref_ts_sec;
        uint32_t ref_ts_frac;
        uint32_t orig_ts_sec;
        uint32_t orig_ts_frac;
        uint32_t recv_ts_sec;
        uint32_t recv_ts_frac;
        uint32_t tx_ts_sec;
        uint32_t tx_ts_frac;
    };

    static inline uint64_t ntp_frac_to_us(uint32_t frac_net) noexcept {
        const uint64_t frac = static_cast<uint64_t>(ntohl(frac_net));
        return (frac * 1000000ULL) >> 32;
    }

    static inline bool ntp_ts_to_unix_us(uint32_t sec_net, uint32_t frac_net, uint64_t& out_us) noexcept {
        static const int64_t NTP_TIMESTAMP_DELTA = 2208988800ll;
        const int64_t sec = static_cast<int64_t>(ntohl(sec_net)) - NTP_TIMESTAMP_DELTA;
        if (sec < 0) return false;
        out_us = static_cast<uint64_t>(sec) * 1000000ULL + ntp_frac_to_us(frac_net);
        return true;
    }

    static inline void fill_client_packet(NtpPacket& pkt, uint64_t now_us) {
        std::memset(&pkt, 0, sizeof(pkt));
        pkt.li_vn_mode = static_cast<uint8_t>((0 << 6) | (3 << 3) | 3); // LI=0, VN=3, Mode=3

        const uint64_t sec = now_us / 1000000 + 2208988800ULL;
        const uint64_t frac = ((now_us % 1000000) * 0x100000000ULL) / 1000000;

        pkt.tx_ts_sec  = htonl(static_cast<uint32_t>(sec));
        pkt.tx_ts_frac = htonl(static_cast<uint32_t>(frac));
    }

    static inline bool parse_server_packet(const NtpPacket& pkt,
                                           uint64_t arrival_us,
                                           int64_t& offset_us,
                                           int64_t& delay_us,
                                           int& stratum) noexcept {
        uint64_t originate_us = 0;
        uint64_t receive_us = 0;
        uint64_t transmit_us = 0;

        if (!ntp_ts_to_unix_us(pkt.orig_ts_sec, pkt.orig_ts_frac, originate_us)) return false;
        if (!ntp_ts_to_unix_us(pkt.recv_ts_sec, pkt.recv_ts_frac, receive_us)) return false;
        if (!ntp_ts_to_unix_us(pkt.tx_ts_sec, pkt.tx_ts_frac, transmit_us)) return false;

        const int64_t t1 = static_cast<int64_t>(originate_us);
        const int64_t t2 = static_cast<int64_t>(receive_us);
        const int64_t t3 = static_cast<int64_t>(transmit_us);
        const int64_t t4 = static_cast<int64_t>(arrival_us);

        offset_us = ((t2 - t1) + (t3 - t4)) / 2;
        delay_us = (t4 - t1) - (t3 - t2);
        stratum = pkt.stratum;
        return true;
    }

} // namespace detail
} // namespace time_shield

#endif // TIME_SHIELD_NTP_PACKET_HPP_INCLUDED
