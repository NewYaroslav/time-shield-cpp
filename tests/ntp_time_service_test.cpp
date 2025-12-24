#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ntp_time_service.hpp>

#include <cassert>
#include <chrono>
#include <thread>

using namespace time_shield;

int main() {
    auto& service = NtpTimeService::instance();

    service.shutdown();
    assert(!service.running());
    assert(service.init());
    assert(service.running());

    service.shutdown();
    assert(service.init(std::chrono::milliseconds(20), true));

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    while (service.measure_count() < 3 && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    service.shutdown();
    assert(!service.running());

    assert(service.set_default_servers());
    NtpPoolConfig cfg;
    cfg.sample_servers = 3;
    cfg.min_valid_samples = 2;
    assert(service.set_pool_config(cfg));
    assert(service.init());

    assert(!service.set_default_servers());
    assert(!service.set_pool_config(cfg));

    service.shutdown();
    return 0;
}

#else
int main() {
    return 0;
}
#endif
