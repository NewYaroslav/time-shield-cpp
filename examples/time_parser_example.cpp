/// \file time_parser_example.cpp
/// \brief Demonstrates functions from time_shield::time_parser.
///
/// This example parses ISO8601 strings, retrieves month numbers, and
/// converts the parsed date and time to different timestamp formats.

#include <iostream>

#if defined(_WIN32)
#include <time_shield/time_parser.hpp>
#include <time_shield/time_zone_struct.hpp>
#include <time_shield/date_time_struct.hpp>

int main() {
    using namespace time_shield;

    const std::string iso = "2024-11-25T14:30:00+01:00";

    DateTimeStruct dt;
    TimeZoneStruct tz;
    if (parse_iso8601(iso, dt, tz)) {
        std::cout << "Parsed date/time: "
                  << dt.year << '-' << dt.mon << '-' << dt.day << ' '
                  << dt.hour << ':' << dt.min << ':' << dt.sec;
        if (dt.ms)
            std::cout << '.' << dt.ms;
        std::cout << ' ' << (tz.is_positive ? '+' : '-')
                  << tz.hour << ':' << tz.min << '\n';
    } else {
        std::cerr << "Failed to parse: " << iso << '\n';
    }

    ts_t s_ts;
    if (str_to_ts(iso, s_ts))
        std::cout << "ts: " << s_ts << '\n';

    ts_ms_t ms_ts;
    if (str_to_ts_ms(iso, ms_ts))
        std::cout << "ts_ms: " << ms_ts << '\n';

    fts_t f_ts;
    if (str_to_fts(iso, f_ts))
        std::cout << "fts: " << f_ts << '\n';

    std::cout << "Using helpers ts():  " << ts(iso) << '\n';
    std::cout << "Using helpers ts_ms(): " << ts_ms(iso) << '\n';
    std::cout << "Using helpers fts():   " << fts(iso) << '\n';

    Month mon = get_month_number<Month>("March");
    std::cout << "Month number for March: " << static_cast<int>(mon) << '\n';

    int sod;
    if (sec_of_day("15:30:10", sod))
        std::cout << "sec_of_day(\"15:30:10\"): " << sod << '\n';
    std::cout << "sec_of_day(\"8:20\"): " << sec_of_day("8:20") << '\n';

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
#else
int main() {
    std::cout << "time_parser.hpp requires Windows for now_realtime_us()" << std::endl;
    return 0;
}
#endif
