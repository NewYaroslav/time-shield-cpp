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

    static_assert(sizeof(NtpPacket) == 48, "NtpPacket must be 48 bytes");

    enum NtpProtoError {
        NTP_EPROTO_BASE   = -10000,
        NTP_E_BAD_MODE    = NTP_EPROTO_BASE - 1,
        NTP_E_BAD_VERSION = NTP_EPROTO_BASE - 2,
        NTP_E_BAD_LI      = NTP_EPROTO_BASE - 3,
        NTP_E_BAD_STRATUM = NTP_EPROTO_BASE - 4,
        NTP_E_KOD         = NTP_EPROTO_BASE - 5,
        NTP_E_BAD_TS      = NTP_EPROTO_BASE - 6
    };

    static inline uint8_t ntp_li(uint8_t li_vn_mode) noexcept {
        return static_cast<uint8_t>((li_vn_mode >> 6) & 0x03);
    }

    static inline uint8_t ntp_vn(uint8_t li_vn_mode) noexcept {
        return static_cast<uint8_t>((li_vn_mode >> 3) & 0x07);
    }

    static inline uint8_t ntp_mode(uint8_t li_vn_mode) noexcept {
        return static_cast<uint8_t>(li_vn_mode & 0x07);
    }

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
                                           int& stratum,
                                           int& out_error_code) noexcept {
        const uint8_t li = ntp_li(pkt.li_vn_mode);
        const uint8_t vn = ntp_vn(pkt.li_vn_mode);
        const uint8_t mode = ntp_mode(pkt.li_vn_mode);

        if (mode != 4) {
            out_error_code = NTP_E_BAD_MODE;
            return false;
        }
        if (vn < 3 || vn > 4) {
            out_error_code = NTP_E_BAD_VERSION;
            return false;
        }
        if (li == 3) {
            out_error_code = NTP_E_BAD_LI;
            return false;
        }
        if (pkt.stratum == 0) {
            out_error_code = NTP_E_KOD;
            return false;
        }
        if (pkt.stratum >= 16) {
            out_error_code = NTP_E_BAD_STRATUM;
            return false;
        }

        uint64_t originate_us = 0;
        uint64_t receive_us = 0;
        uint64_t transmit_us = 0;

        if (!ntp_ts_to_unix_us(pkt.orig_ts_sec, pkt.orig_ts_frac, originate_us)) {
            out_error_code = NTP_E_BAD_TS;
            return false;
        }
        if (!ntp_ts_to_unix_us(pkt.recv_ts_sec, pkt.recv_ts_frac, receive_us)) {
            out_error_code = NTP_E_BAD_TS;
            return false;
        }
        if (!ntp_ts_to_unix_us(pkt.tx_ts_sec, pkt.tx_ts_frac, transmit_us)) {
            out_error_code = NTP_E_BAD_TS;
            return false;
        }

        const int64_t t1 = static_cast<int64_t>(originate_us);
        const int64_t t2 = static_cast<int64_t>(receive_us);
        const int64_t t3 = static_cast<int64_t>(transmit_us);
        const int64_t t4 = static_cast<int64_t>(arrival_us);

        if (t3 < t2) {
            out_error_code = NTP_E_BAD_TS;
            return false;
        }

        offset_us = ((t2 - t1) + (t3 - t4)) / 2;
        delay_us = (t4 - t1) - (t3 - t2);
        if (delay_us < 0) {
            out_error_code = NTP_E_BAD_TS;
            return false;
        }
        stratum = pkt.stratum;
        return true;
    }

} // namespace detail
} // namespace time_shield

#endif // TIME_SHIELD_NTP_PACKET_HPP_INCLUDED
