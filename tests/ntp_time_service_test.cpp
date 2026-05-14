#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ntp_time_service.hpp>

#include "test_assert.hpp"
#include <chrono>
#include <thread>

using namespace time_shield;

int main() {
    auto& service = NtpTimeService::instance();

    service.shutdown();
    TIME_SHIELD_TEST_CHECK(!service.running());
    (void)service.utc_time_ms();
    TIME_SHIELD_TEST_CHECK(service.running());

    service.shutdown();
    TIME_SHIELD_TEST_CHECK(service.init(std::chrono::milliseconds(20), true));

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    while (service.measure_count() < 3 && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    service.shutdown();
    TIME_SHIELD_TEST_CHECK(!service.running());

    TIME_SHIELD_TEST_CHECK(service.set_default_servers());
    NtpPoolConfig cfg;
    cfg.sample_servers = 3;
    cfg.min_valid_samples = 2;
    TIME_SHIELD_TEST_CHECK(service.set_pool_config(cfg));
    TIME_SHIELD_TEST_CHECK(service.init());
    TIME_SHIELD_TEST_CHECK(service.apply_config_now());

    TIME_SHIELD_TEST_CHECK(!service.set_default_servers());
    TIME_SHIELD_TEST_CHECK(!service.set_pool_config(cfg));

    service.shutdown();
    return 0;
}

#else
int main() {
    return 0;
}
#endif
