/// \file time_utils_example.cpp
/// \brief Demonstrates functions from time_shield::time_utils.
///
/// The example prints various timestamps and the CPU time used by
/// the process.

#include <iostream>

#if defined(_WIN32)
#include <time_shield/types.hpp>
#include <time_shield/constants.hpp>
#include <time_shield/time_utils.hpp>

int main() {
    using namespace time_shield;

    // Print different representations of the current time
    std::cout << "Seconds since epoch:      " << ts() << '\n';
    std::cout << "Milliseconds since epoch: " << ts_ms() << '\n';
    std::cout << "Microseconds since epoch: " << ts_us() << '\n';

    // Show fractional parts of the current second
    std::cout << "Nanosecond part: " << ns_of_sec() << '\n';
    std::cout << "Microsecond part: " << us_of_sec() << '\n';
    std::cout << "Millisecond part: " << ms_of_sec() << '\n';

    std::cout << "CPU time used: " << get_cpu_time() << " s" << '\n';
    std::cout << "Realtime (us) via QPC: " << now_realtime_us() << '\n';
    
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
#else
int main() {
    std::cout << "time_utils.hpp requires Windows for now_realtime_us()" << std::endl;
    return 0;
}
#endif
