/// \file ntp_client_example.cpp
/// \brief Demonstrates usage of time_shield::NtpClient.
///
/// This example queries an NTP server, measures the time offset
/// between the local machine and the server, then prints the
/// current time without correction and with the measured offset applied.

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#if defined(_WIN32)
#   include <time_shield.hpp>

int main() {
    using namespace time_shield;
    
    init();

    NtpClient client; // uses pool.ntp.org by default

    std::cout << "Querying NTP server..." << std::endl;
    if (!client.query()) {
        std::cerr << "Failed to query NTP server. Error code: "
                  << client.get_last_error_code() << std::endl;
        return 1;
    }

    const int64_t offset_us = client.get_offset_us();

    // Current local system time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "Local time:    "
              << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%d %H:%M:%S")
              << std::endl;

    // Corrected time using the offset from the NTP server
    auto corrected = std::chrono::system_clock::time_point(
        std::chrono::microseconds(client.get_utc_time_us()));
    auto corrected_time_t = std::chrono::system_clock::to_time_t(corrected);
    std::cout << "Corrected time: "
              << std::put_time(std::gmtime(&corrected_time_t), "%Y-%m-%d %H:%M:%S")
              << std::endl;

    std::cout << "Offset (us): " << offset_us << std::endl;
    
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}

#else
int main() {
    std::cout << "NtpClient is supported only on Windows." << std::endl;
    return 0;
}
#endif

