#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include <time_shield/ntp_client/ntp_client_core.hpp>
#include <time_shield/ntp_client/ntp_packet.hpp>
#include <time_shield/ntp_client/udp_transport.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>

using namespace time_shield;

class FakeUdpTransport : public detail::IUdpTransport {
public:
    bool ok = true;
    int error_code = 0;
    detail::NtpPacket reply{};

    bool transact(const detail::UdpRequest& req, int& out_error_code) noexcept override {
        out_error_code = error_code;
        if (!ok) {
            return false;
        }
        if (req.recv_data && req.recv_size == sizeof(detail::NtpPacket)) {
            std::memcpy(req.recv_data, &reply, sizeof(reply));
        }
        out_error_code = 0;
        return true;
    }
};

static void unix_us_to_ntp_ts(uint64_t unix_us, uint32_t& sec_net, uint32_t& frac_net) {
    const uint64_t sec = unix_us / 1000000 + 2208988800ULL;
    const uint64_t frac = ((unix_us % 1000000) * 0x100000000ULL) / 1000000;
    sec_net = htonl(static_cast<uint32_t>(sec));
    frac_net = htonl(static_cast<uint32_t>(frac));
}

int main() {
    {
        // Successful parse
        FakeUdpTransport transport;
        detail::NtpPacket pkt{};
        uint32_t sec = 0;
        uint32_t frac = 0;
        unix_us_to_ntp_ts(1000000ULL, sec, frac); // t1
        pkt.orig_ts_sec = sec;
        pkt.orig_ts_frac = frac;

        unix_us_to_ntp_ts(1000100ULL, sec, frac); // t2
        pkt.recv_ts_sec = sec;
        pkt.recv_ts_frac = frac;

        unix_us_to_ntp_ts(1000120ULL, sec, frac); // t3
        pkt.tx_ts_sec = sec;
        pkt.tx_ts_frac = frac;

        pkt.stratum = 2;
        transport.reply = pkt;

        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(ok);
        assert(error == 0);
        assert(offset != 0 || delay != 0); // basic sanity without relying on realtime arrival
        assert(stratum == 2);
    }

    {
        // Transport failure
        FakeUdpTransport transport;
        transport.ok = false;
        transport.error_code = 42;
        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!ok);
        assert(error == 42);
    }

    {
        // Parse failure
        FakeUdpTransport transport;
        std::memset(&transport.reply, 0xFF, sizeof(transport.reply));
        detail::NtpClientCore core;
        int error = 0;
        int64_t offset = 0;
        int64_t delay = 0;
        int stratum = -1;
        const bool ok = core.query(transport, "example.com", 123, 5000, error, offset, delay, stratum);
        assert(!ok);
        assert(error == -1);
    }

    {
        // Fraction to microseconds
        assert(detail::ntp_frac_to_us(htonl(0u)) == 0);
        assert(detail::ntp_frac_to_us(htonl(0x80000000u)) >= 499999 && detail::ntp_frac_to_us(htonl(0x80000000u)) <= 500001);
        assert(detail::ntp_frac_to_us(htonl(0xFFFFFFFFu)) >= 999998 && detail::ntp_frac_to_us(htonl(0xFFFFFFFFu)) <= 1000000);
    }

    return 0;
}

#else
int main() {
    return 0;
}
#endif
