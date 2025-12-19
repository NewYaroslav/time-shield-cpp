#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include <time_shield/time_utils.hpp>
#include <time_shield/ntp_client/ntp_client_core.hpp>
#include <time_shield/ntp_client/ntp_packet.hpp>
#include <time_shield/ntp_client/udp_transport.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>

using namespace time_shield;

class FakeUdpTransport : public detail::IUdpTransport {
public:
    bool is_ok = true;
    int error_code = 0;
    detail::NtpPacket reply{};

    bool transact(const detail::UdpRequest& req, int& out_error_code) noexcept override {
        assert(req.send_size == sizeof(detail::NtpPacket));
        assert(req.recv_size == sizeof(detail::NtpPacket));
        out_error_code = error_code;
        if (!is_ok) {
            return false;
        }
        if (req.recv_data && req.recv_size == sizeof(detail::NtpPacket)) {
            std::memcpy(req.recv_data, &reply, sizeof(reply));
        }
        out_error_code = 0;
        return true;
    }
};

static uint8_t make_li_vn_mode(uint8_t li, uint8_t vn, uint8_t mode) {
    return static_cast<uint8_t>((li << 6) | (vn << 3) | mode);
}

static void unix_us_to_ntp_ts(uint64_t unix_us, uint32_t& sec_net, uint32_t& frac_net) {
    const uint64_t sec = unix_us / 1000000 + 2208988800ULL;
    const uint64_t frac = ((unix_us % 1000000) * 0x100000000ULL) / 1000000;
    sec_net = htonl(static_cast<uint32_t>(sec));
    frac_net = htonl(static_cast<uint32_t>(frac));
}

static detail::NtpPacket build_base_packet(uint64_t base_us) {
    detail::NtpPacket pkt{};
    pkt.li_vn_mode = make_li_vn_mode(0, 3, 4);
    pkt.stratum = 2;

    uint32_t sec = 0;
    uint32_t frac = 0;
    unix_us_to_ntp_ts(base_us - 4000, sec, frac);
    pkt.orig_ts_sec = sec;
    pkt.orig_ts_frac = frac;

    unix_us_to_ntp_ts(base_us - 2000, sec, frac);
    pkt.recv_ts_sec = sec;
    pkt.recv_ts_frac = frac;

    unix_us_to_ntp_ts(base_us - 1000, sec, frac);
    pkt.tx_ts_sec = sec;
    pkt.tx_ts_frac = frac;

    return pkt;
}

int main() {
    const int64_t now_us = time_shield::now_realtime_us();
    assert(now_us > 0);
    const uint64_t base_us = static_cast<uint64_t>(now_us);

    {
        // Valid server response
        FakeUdpTransport transport;
        transport.reply = build_base_packet(base_us);

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(is_ok);
        assert(error == 0);
        assert(stratum == 2);
        assert(delay >= 0);
        (void)is_ok;
        (void)error;
        (void)offset;
        (void)delay;
        (void)stratum;
    }

    {
        // Bad mode
        FakeUdpTransport transport;
        detail::NtpPacket pkt = build_base_packet(base_us);
        pkt.li_vn_mode = make_li_vn_mode(0, 3, 3);
        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!is_ok);
        assert(error == detail::NTP_E_BAD_MODE);
    }

    {
        // Bad version
        FakeUdpTransport transport;
        detail::NtpPacket pkt = build_base_packet(base_us);
        pkt.li_vn_mode = make_li_vn_mode(0, 0, 4);
        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!is_ok);
        assert(error == detail::NTP_E_BAD_VERSION);
    }

    {
        // LI alarm
        FakeUdpTransport transport;
        detail::NtpPacket pkt = build_base_packet(base_us);
        pkt.li_vn_mode = make_li_vn_mode(3, 3, 4);
        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!is_ok);
        assert(error == detail::NTP_E_BAD_LI);
    }

    {
        // KoD
        FakeUdpTransport transport;
        detail::NtpPacket pkt = build_base_packet(base_us);
        pkt.stratum = 0;
        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!is_ok);
        assert(error == detail::NTP_E_KOD);
    }

    {
        // Unsynchronized stratum
        FakeUdpTransport transport;
        detail::NtpPacket pkt = build_base_packet(base_us);
        pkt.stratum = 16;
        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!is_ok);
        assert(error == detail::NTP_E_BAD_STRATUM);
    }

    {
        // Bad timestamps
        FakeUdpTransport transport;
        detail::NtpPacket pkt = build_base_packet(base_us);
        pkt.orig_ts_sec = 0;
        pkt.orig_ts_frac = 0;
        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!is_ok);
        assert(error == detail::NTP_E_BAD_TS);
    }

    {
        // Negative delay
        FakeUdpTransport transport;
        detail::NtpPacket pkt{};
        pkt.li_vn_mode = make_li_vn_mode(0, 3, 4);
        pkt.stratum = 2;

        uint32_t sec = 0;
        uint32_t frac = 0;
        unix_us_to_ntp_ts(base_us - 1000000, sec, frac);
        pkt.orig_ts_sec = sec;
        pkt.orig_ts_frac = frac;

        unix_us_to_ntp_ts(base_us - 900000, sec, frac);
        pkt.recv_ts_sec = sec;
        pkt.recv_ts_frac = frac;

        unix_us_to_ntp_ts(base_us + 1000000, sec, frac);
        pkt.tx_ts_sec = sec;
        pkt.tx_ts_frac = frac;

        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool is_ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!is_ok);
        assert(error == detail::NTP_E_BAD_TS);
    }

    return 0;
}

#else
int main() {
    return 0;
}
#endif
