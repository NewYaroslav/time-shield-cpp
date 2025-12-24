#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#define TIME_SHIELD_NTP_TIME_SERVICE_USE_DLL_SINGLETON
#define TIME_SHIELD_NTP_TIME_SERVICE_DLL_EXPORTS
#define TIME_SHIELD_NTP_TIME_SERVICE_DLL_IMPLEMENTATION
#include <time_shield/ntp_time_service.hpp>

int ntp_time_service_dll_helper();

int main() {
    auto& service = time_shield::NtpTimeService::instance();
    service.shutdown();
    (void)ntp_time_service_dll_helper();
    return 0;
}
#else
int main() {
    return 0;
}
#endif
