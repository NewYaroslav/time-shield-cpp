\mainpage Time Shield Library

**Version:** `VERSION_PLACEHOLDER`

\section intro_sec Introduction

The Time Shield Library is a comprehensive C++ library designed for time manipulation, formatting, and conversion.
It was built for practical and engineering tasks, especially in constrained or performance-critical environments.

\section motivation_sec Why Time Shield?

Unlike `std::chrono` or more academic libraries like `HowardHinnant/date`, Time Shield is designed to be simple,
portable, and suitable for scenarios like logging, serialization, MQL5 usage, and date/time formatting.

- Uses simple types (`int64_t`, `double`) like `ts_t`, `fts_t` for timestamps — easy to serialize and store.
- Supports multiple time representations: Unix, floating-point seconds, ms/us precision, OLE Automation, Julian dates.
- Includes utilities for rounding, ISO8601 formatting/parsing, time parts, and boundary calculations.
- Header-only and cross-platform; most modules require only STL and `<cstdint>`.
- MQL5-compatible — does not use exceptions, STL containers, or dynamic memory.
- Modules requiring platform-specific APIs (like `NtpClient`) are isolated and optional.

\section features_sec Features

- Validation of dates and times
- Time and date formatting (standard and custom)
- Time zone conversion functions
- ISO8601 string parsing
- Utilities for time manipulation and conversion

\section usage_sec Example

\code{.cpp}
#include <time_shield.hpp>

int main() {
    tsh::ts_t current_ts = tsh::timestamp();
    tsh::ts_t future_ts = tsh::end_of_year(current_ts);
    int days = tsh::get_days_difference(current_ts, future_ts);
    std::cout << "Days until end of year: " << days << std::endl;

    std::string formatted_time = tsh::to_iso8601_str(future_ts);
    tsh::ts_ms_t ts_ms = 0;
    if (!tsh::str_to_ts_ms(formatted_time, ts_ms)) {
        std::cout << "Error occurred while parsing ISO8601 string" << std::endl;
        return -1;
    }

    formatted_time = tsh::to_iso8601_utc_ms(ts_ms);
    std::cout << "Future time: " << formatted_time << std::endl;
    return 0;
}
\endcode

\section examples_sec Examples

Located in the `examples/` folder:

- `time_utils_example.cpp` — get timestamps and parts
- `time_formatting_example.cpp` — to_string, ISO8601, MQL5
- `time_parser_example.cpp` — parse ISO8601
- `time_conversions_example.cpp` — convert between formats
- `time_zone_conversions_example.cpp` — CET/EET ↔ GMT
- `ntp_client_example.cpp` — NTP sync (Windows-only)

\section install_sec Installation

Time Shield is a header-only library. To use it in your C++ project:

- Add the `include/time_shield_cpp` folder to your project's include paths.
- Include the main header:

\code{.cpp}
#include <time_shield.hpp>
\endcode

No additional build steps or external dependencies are required.

For MQL5/MetaTrader:

- Run `install_mql5.bat` to copy the necessary `.mqh` files to your MQL5 include directory.
- Include the main file in your MQL5 script:

\code{.mq5}
#include <TimeShield.mqh>
\endcode

To build the C++ examples:

- Run `build-examples.bat` to compile example programs with MSVC or your preferred toolchain.

\section repo_sec Repository

[Time Shield Library GitHub repository](https://github.com/NewYaroslav/time-shield-cpp)

\section license_sec License

This library is licensed under the MIT License. See the LICENSE file for more details.