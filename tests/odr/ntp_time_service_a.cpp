#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ntp_time_service.hpp>

#include <cassert>
#include <cstdint>

std::uintptr_t ntp_time_service_instance_address_b();

int main() {
    auto& service = time_shield::NtpTimeService::instance();
    service.shutdown();

    const auto instance_address = reinterpret_cast<std::uintptr_t>(&service);
    assert(instance_address == ntp_time_service_instance_address_b());

    (void)service.utc_time_ms();
    assert(service.running());
    service.shutdown();
    return 0;
}
#else
int main() {
    return 0;
}
#endif
