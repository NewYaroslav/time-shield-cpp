#include <time_shield.hpp>
#include <time_shield/config.hpp>
#if TIME_SHIELD_ENABLE_NTP_CLIENT
#include <time_shield/ntp_time_service.hpp>
#endif

/// \brief Verify library usage from an installed location.
int main() {
    const auto current_ts = time_shield::now();
    const auto mono_sec = time_shield::monotonic_sec();
    const auto mono_ms = time_shield::monotonic_ms();
    const auto mono_us = time_shield::monotonic_us();
#if TIME_SHIELD_ENABLE_NTP_CLIENT
    const bool has_started = time_shield::ntp::init(30000, false);
    const bool is_stale = time_shield::ntp::stale(5000);
    const bool is_running = time_shield::NtpTimeService::instance().running();
    time_shield::ntp::shutdown();
    (void)has_started;
    (void)is_stale;
    (void)is_running;
#endif
    return (current_ts > 0 && mono_sec >= 0 && mono_ms >= 0 && mono_us >= 0) ? 0 : 1;
}
