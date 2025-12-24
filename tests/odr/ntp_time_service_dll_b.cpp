#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#define TIME_SHIELD_NTP_TIME_SERVICE_USE_DLL_SINGLETON
#define TIME_SHIELD_NTP_TIME_SERVICE_DLL_EXPORTS
#include <time_shield/ntp_time_service.hpp>

int ntp_time_service_dll_helper() {
    return time_shield::NtpTimeService::instance().running() ? 1 : 0;
}
#else
int ntp_time_service_dll_helper() {
    return 0;
}
#endif
