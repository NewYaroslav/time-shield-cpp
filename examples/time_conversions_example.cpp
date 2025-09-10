/// \file time_conversions_example.cpp
/// \brief Demonstrates functions from time_shield::time_conversions.
///
/// This example showcases various time conversion utilities,
/// including fractional parts of seconds, unit conversions,
/// timestamp creation, and date calculations.

#include <iostream>

#if defined(_WIN32)
#include <time_shield/time_conversions.hpp>

int main() {
    using namespace time_shield;

    double sec = 123.456;
    std::cout << "ns_of_sec: " << ns_of_sec(sec) << '\n';
    std::cout << "us_of_sec: " << us_of_sec(sec) << '\n';
    std::cout << "ms_of_sec: " << ms_of_sec(sec) << '\n';

    auto ms = sec_to_ms(sec);
    std::cout << "sec_to_ms: " << ms << '\n';
    std::cout << "ms_to_sec: " << ms_to_sec(ms) << '\n';
    std::cout << "ms_to_fsec: " << ms_to_fsec(ms) << '\n';

    std::cout << "min_to_sec(2.5): " << min_to_sec(2.5) << '\n';
    std::cout << "sec_to_min(150): " << sec_to_min(150) << '\n';
    std::cout << "hour_to_ms(1.5): " << hour_to_ms(1.5) << '\n';
    std::cout << "hour24_to_12(15): " << hour24_to_12(15) << '\n';

    ts_t ts_val = to_ts(2024, Month::JUN, 21, 14, 30, 0);
    ts_ms_t ts_ms_val = to_ts_ms(2024, Month::JUN, 21, 14, 30, 0, 250);

    DateTimeStruct dt = to_dt_ms<DateTimeStruct>(ts_ms_val);
    std::cout << "to_dt_ms: " << dt.year << '-' << dt.mon << '-' << dt.day << ' '
              << dt.hour << ':' << dt.min << ':' << dt.sec << '.' << dt.ms << '\n';

    ts_t back_to_ts = dt_to_timestamp(dt);
    std::cout << "dt_to_timestamp: " << back_to_ts << '\n';
    
    std::cout << "start_of_day: " << start_of_day(ts_val) << '\n';
    std::cout << "end_of_day_ms: " << end_of_day_ms(ts_ms_val) << '\n';
    std::cout << "next_day_ms: " << next_day_ms(ts_ms_val) << '\n';

    std::cout << "day_of_year: " << day_of_year(ts_val) << '\n';
    std::cout << "month_of_year: " << static_cast<int>(month_of_year(ts_val)) << '\n';
    std::cout << "day_of_month: " << day_of_month(ts_val) << '\n';
    std::cout << "start_of_year_date(2024): " << start_of_year_date(2024) << '\n';
    std::cout << "day_of_week_date(2024, JUN, 21): "
              << static_cast<int>(day_of_week_date(2024, Month::JUN, 21)) << '\n';

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
#else
int main() {
    std::cout << "time_conversions.hpp requires Windows." << std::endl;
    return 0;
}
#endif

