#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT && TIME_SHIELD_PLATFORM_UNIX

#include <time_shield/initialization.hpp>
#include <time_shield/ntp_client_pool.hpp>

#include <chrono>
#include <future>
#include <iostream>
#include <vector>

int main() {
    using namespace time_shield;

    init();

    NtpPoolConfig pool_cfg;
    pool_cfg.sample_servers = 3;
    pool_cfg.min_valid_samples = 1;

    NtpClientPool pool(pool_cfg);
    pool.set_servers(NtpClientPool::build_default_servers());

    auto measurement_future = std::async(std::launch::async, [&pool]() { return pool.measure(); });

    const auto status = measurement_future.wait_for(std::chrono::minutes(5));
    if (status != std::future_status::ready) {
        std::cerr << "NtpClientPool measurement timed out" << std::endl;
        return 1;
    }

    const bool is_updated = measurement_future.get();
    if (!is_updated) {
        std::cerr << "NtpClientPool measurement failed" << std::endl;
        return 1;
    }

    const std::vector<NtpSample> samples = pool.last_samples();
    if (samples.empty()) {
        std::cerr << "No NTP samples collected" << std::endl;
        return 1;
    }

    bool is_any_ok = false;
    for (const auto& sample : samples) {
        if (sample.is_ok) {
            is_any_ok = true;
            break;
        }
    }

    if (!is_any_ok) {
        std::cerr << "All NTP samples failed" << std::endl;
        return 1;
    }

    return 0;
}

#else
int main() {
    return 0;
}
#endif
