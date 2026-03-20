#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ntp_time_service.hpp>

#include <cstdint>

std::uintptr_t ntp_time_service_instance_address_b() {
    return reinterpret_cast<std::uintptr_t>(&time_shield::NtpTimeService::instance());
}
#else
int ntp_time_service_instance_address_b() {
    return 0;
}
#endif
