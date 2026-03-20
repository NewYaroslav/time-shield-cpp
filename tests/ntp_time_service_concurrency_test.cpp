#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ntp_time_service.hpp>

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>

using namespace time_shield;

int main() {
    auto& service = NtpTimeService::instance();
    service.shutdown();

    std::atomic<bool> is_done{false};

    std::thread init_thread([&]() {
        for (int i = 0; i < 20; ++i) {
            (void)service.init(std::chrono::milliseconds(10), true);
        }
        is_done.store(true);
    });

    std::thread reconfigure_thread([&]() {
        NtpPoolConfig cfg;
        cfg.sample_servers = 3;
        cfg.min_valid_samples = 2;
        while (!is_done.load()) {
            (void)service.apply_config_now();
            service.shutdown();
            (void)service.set_pool_config(cfg);
            (void)service.set_default_servers();
        }
    });

    init_thread.join();
    reconfigure_thread.join();

    (void)service.utc_time_ms();
    assert(service.running());
    service.shutdown();
    assert(!service.running());
    return 0;
}

#else
int main() {
    return 0;
}
#endif
