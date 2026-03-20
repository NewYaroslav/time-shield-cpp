/// \file ntp_time_service_example.cpp
/// \brief Shows a minimal NTP time service workflow.

#include <cstdint>
#include <iostream>

#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#   include <time_shield/initialization.hpp>
#   include <time_shield/ntp_time_service.hpp>

int main() {
    // Initialize library internals before using time-related helpers.
    time_shield::init();

    // Start the background NTP service with a simple polling interval.
    if (!time_shield::ntp::init(30000)) {
        std::cerr << "Failed to start NTP time service." << std::endl;
        return 1;
    }

    // Read corrected UTC time and the current measured offset.
    const int64_t utc_ms = time_shield::ntp::utc_time_ms();
    const int64_t offset_us = time_shield::ntp::offset_us();
    const bool is_ok = time_shield::ntp::last_measure_ok();

    std::cout << "UTC time (ms): " << utc_ms << std::endl;
    std::cout << "Offset (us): " << offset_us << std::endl;
    std::cout << "Last measurement ok: " << (is_ok ? "true" : "false") << std::endl;

    // Stop the service before exiting the application.
    time_shield::ntp::shutdown();
    return 0;
}

#else

int main() {
    std::cout << "NTP time service is disabled in this build." << std::endl;
    return 0;
}

#endif
