#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT && TIME_SHIELD_PLATFORM_UNIX

#include <time_shield/initialization.hpp>
#include <time_shield/ntp_client.hpp>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

namespace {

    struct ntp_packet {
        uint8_t  li_vn_mode;
        uint8_t  stratum;
        uint8_t  poll;
        uint8_t  precision;
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

    constexpr int64_t NTP_TIMESTAMP_DELTA = 2208988800ll;

    uint64_t system_now_us() {
        const auto now = std::chrono::system_clock::now();
        const auto us  = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        return static_cast<uint64_t>(us.count());
    }

    void pack_ts(uint64_t unix_us, uint32_t& sec, uint32_t& frac) {
        const uint64_t seconds = unix_us / 1000000 + static_cast<uint64_t>(NTP_TIMESTAMP_DELTA);
        const uint64_t fraction = ((unix_us % 1000000) * 0x100000000ULL) / 1000000;
        sec  = htonl(static_cast<uint32_t>(seconds));
        frac = htonl(static_cast<uint32_t>(fraction));
    }

    void run_ntp_server(uint16_t port, std::atomic<bool>& is_ready) {
        const int server_sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (server_sock < 0) {
            is_ready = true;
            return;
        }

        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        server_addr.sin_port = htons(port);

        if (bind(server_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
            ::close(server_sock);
            is_ready = true;
            return;
        }

        is_ready = true;

        ntp_packet request{};
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        const ssize_t received = ::recvfrom(
            server_sock,
            &request,
            sizeof(request),
            0,
            reinterpret_cast<sockaddr*>(&client_addr),
            &client_len);

        if (received < 0) {
            ::close(server_sock);
            return;
        }

        const uint64_t receive_us = system_now_us();

        ntp_packet response{};
        response.li_vn_mode = (0 << 6) | (4 << 3) | 4; // LI=0, VN=4, Mode=4 (server)
        response.stratum = 1;
        response.poll = 4;
        response.precision = 0xFA;
        response.ref_id = htonl(0x4c4f434c); // "LOCL"
        response.orig_ts_sec = request.tx_ts_sec;
        response.orig_ts_frac = request.tx_ts_frac;
        pack_ts(receive_us, response.recv_ts_sec, response.recv_ts_frac);
        pack_ts(system_now_us(), response.tx_ts_sec, response.tx_ts_frac);

        ::sendto(
            server_sock,
            &response,
            sizeof(response),
            0,
            reinterpret_cast<sockaddr*>(&client_addr),
            client_len);

        ::close(server_sock);
    }

} // namespace

int main() {
    using namespace time_shield;

    init();

    constexpr uint16_t port = 12345;
    std::atomic<bool> is_server_ready{false};
    std::thread server_thread(run_ntp_server, port, std::ref(is_server_ready));

    while (!is_server_ready.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    NtpClient client("127.0.0.1", port);
    const bool is_query_successful = client.query();

    server_thread.join();

    if (!is_query_successful || !client.success()) {
        std::cerr << "NTP query failed, error code: " << client.get_last_error_code() << std::endl;
        return 1;
    }

    const int64_t offset_us = client.get_offset_us();
    const int64_t tolerance_us = 50000; // 50 ms
    if (std::llabs(offset_us) > tolerance_us) {
        std::cerr << "Offset is too large: " << offset_us << std::endl;
        return 1;
    }

    const int64_t utc_us = client.get_utc_time_us();
    const int64_t host_us = static_cast<int64_t>(system_now_us());
    const int64_t drift_us = std::llabs(utc_us - host_us);
    if (drift_us > 500000) { // 0.5s tolerance for local mock server
        std::cerr << "UTC drift is too large: " << drift_us << std::endl;
        return 1;
    }

    return 0;
}

#else
int main() {
    return 0;
}
#endif
