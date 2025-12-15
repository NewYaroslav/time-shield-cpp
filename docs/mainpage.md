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

- Validation of dates and times (including weekend and workday predicates)
- Time and date formatting (standard and custom)
- Time zone conversion functions
- ISO8601 string parsing
- Utilities for time manipulation and conversion
- OLE Automation date conversions and astronomy helpers (JD/MJD/JDN, lunar phase)

\section config_sec Configuration

Compile-time macros in `time_shield/config.hpp` allow adapting the library to
the target platform and toggling optional modules:

- `TIME_SHIELD_PLATFORM_WINDOWS` / `TIME_SHIELD_PLATFORM_UNIX` — platform
  detection.
- `TIME_SHIELD_HAS_WINSOCK` — WinSock availability.
- `TIME_SHIELD_ENABLE_NTP_CLIENT` — builds the NTP client when set to `1`.

All public symbols are declared inside the `time_shield` namespace.

\section invariants_sec API Invariants

- `ts_t` represents Unix time in seconds as a signed 64-bit integer with
  microsecond precision available through conversions.
- `fts_t` stores time as double precision seconds; conversions maintain
  microsecond accuracy.
- ISO8601 parsing assumes the proleptic Gregorian calendar without leap seconds.
- Functions avoid throwing exceptions and use no dynamic memory internally;
  utilities returning `std::string` rely on the caller for allocations.

\section examples_sec Examples

Here is a simple demonstration:

\code{.cpp}
#include <time_shield.hpp>

int main() {
    time_shield::ts_t current_ts = time_shield::timestamp();
    time_shield::ts_t future_ts = time_shield::end_of_year(current_ts);
    int days = time_shield::get_days_difference(current_ts, future_ts);
    std::cout << "Days until end of year: " << days << std::endl;

    std::string formatted_time = time_shield::to_iso8601_str(future_ts);
    time_shield::ts_ms_t ts_ms = 0;
    if (!time_shield::str_to_ts_ms(formatted_time, ts_ms)) {
        std::cout << "Error occurred while parsing ISO8601 string" << std::endl;
        return -1;
    }

    formatted_time = time_shield::to_iso8601_utc_ms(ts_ms);
    std::cout << "Future time: " << formatted_time << std::endl;
    return 0;
}
\endcode

Additional example files are located in the `examples/` folder:

- `time_utils_example.cpp` — get timestamps and parts
- `time_formatting_example.cpp` — to_string, ISO8601, MQL5
- `time_parser_example.cpp` — parse ISO8601
- `time_conversions_example.cpp` — convert between formats
- `time_zone_conversions_example.cpp` — CET/EET ↔ GMT
- `ntp_client_example.cpp` — NTP sync (sockets)

\subsection oa_and_astronomy OA date and astronomy helpers

Convert between Unix timestamps and Excel/COM OA dates, or derive basic
astronomical values from calendar inputs:

\code{.cpp}
#include <time_shield/ole_automation_conversions.hpp>
#include <time_shield/astronomy_conversions.hpp>

using namespace time_shield;

oadate_t oa = ts_to_oadate(1714608000);               // OA date for 2024-05-02
ts_t ts_from_oa = oadate_to_ts(oa);

jd_t jd = gregorian_to_jd(2, 5, 2024, 12, 0);         // Julian Date with time
mjd_t mjd = ts_to_mjd(1714608000);                    // Modified Julian Date
double phase = moon_phase(fts());                     // lunar phase [0..1)
double age = moon_age_days(fts());                    // lunar age in days
\endcode

\subsection workday_helpers Workday helpers

Check whether a moment falls on a business day using timestamps, calendar components, or ISO8601 strings:

\code{.cpp}
#include <time_shield/validation.hpp>
#include <time_shield/time_parser.hpp>

using namespace time_shield;

bool monday = is_workday(1710720000);                    // Unix seconds (2024-03-18)
bool monday_ms = is_workday_ms("2024-03-18T09:00:00.250Z"); // ISO8601 with milliseconds
bool from_date = is_workday(2024, 3, 18);                 // year, month, day components

// Parsing failure or a weekend evaluates to false
bool saturday = is_workday("2024-03-16T10:00:00Z");
bool invalid = is_workday("not-a-date");
\endcode

Locate the first and last trading days for a month and constrain schedules to the opening or closing workdays:

\code{.cpp}
using namespace time_shield;

ts_t june28 = to_timestamp(2024, 6, 28);
bool is_last = is_last_workday_of_month(june28);                 // true (Friday before a weekend)
bool in_last_two = is_within_last_workdays_of_month(june28, 2);  // true for the final two workdays
bool in_first_two = is_within_first_workdays_of_month(june28, 2);// false, trailing end of month

bool first_from_str = is_first_workday_of_month("2024-09-02T09:00:00Z");
bool window_from_str = is_within_first_workdays_of_month_ms(
    "2024-09-03T09:00:00.250Z", 2);
\endcode

The string overloads recognise the same ISO8601 formats handled by \ref time_shield::str_to_ts "str_to_ts" and \ref time_shield::str_to_ts_ms "str_to_ts_ms".

Locate the boundaries of the first and last workdays when preparing trading windows or settlement cutoffs:

\code{.cpp}
#include <time_shield/time_conversions.hpp>

using namespace time_shield;

ts_t first_open = start_of_first_workday_month(2024, Month::JUN);    // 2024-06-03T00:00:00Z
ts_t first_close = end_of_first_workday_month(2024, 6);              // 2024-06-03T23:59:59Z
ts_t last_open = start_of_last_workday_month(2024, 3);               // 2024-03-29T00:00:00Z
ts_ms_t last_close_ms = end_of_last_workday_month_ms(2024, Month::MAR); // 2024-03-29T23:59:59.999Z
\endcode

These helpers follow the same semantics as \ref time_shield::start_of_day "start_of_day" and \ref time_shield::end_of_day "end_of_day", returning UTC timestamps. Invalid month inputs or months without workdays yield \ref time_shield::ERROR_TIMESTAMP "ERROR_TIMESTAMP" to simplify validation.

\section install_sec Installation
\subsection install_pkg Install and `find_package`

After installing the library (e.g., via `cmake --install`), locate it with
`find_package`:

\code{.cmake}
cmake_minimum_required(VERSION 3.15)
project(app LANGUAGES CXX)

find_package(TimeShield CONFIG REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE time_shield::time_shield)
\endcode

\subsection install_submodule Git submodule with `add_subdirectory`

Vendor the library as a submodule:

\code{.sh}
git submodule add https://github.com/NewYaroslav/time-shield-cpp external/time-shield-cpp
\endcode

Then include it:

\code{.cmake}
add_subdirectory(external/time-shield-cpp)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE time_shield::time_shield)
\endcode

\subsection install_vcpkg vcpkg overlay

Install via a local overlay port:

\code{.sh}
vcpkg install time-shield-cpp --overlay-ports=./vcpkg-overlay/ports
\endcode

Use the vcpkg toolchain when configuring CMake:

\code{.sh}
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
\endcode

The port is intended to be upstreamed to
\ref https://github.com/microsoft/vcpkg "microsoft/vcpkg".

For MQL5/MetaTrader, run `install_mql5.bat` to copy the `.mqh` files to your
include directory.

To build the C++ examples use the helper scripts:

- `build-examples.bat` for Windows
- `build_examples.sh` for Linux/macOS
- `build-cb.bat` to generate a Code::Blocks project

\section tested_sec Tested Platforms

| Platform | Compilers   | C++ Standards |
|----------|-------------|---------------|
| Windows  | MSVC, ClangCL | 11, 14, 17 |
| Linux    | GCC, Clang  | 11, 14, 17 |
| macOS    | Apple Clang | 11, 14, 17 |

\section docs_sec Online Documentation

The latest generated API reference is available at
\ref https://newyaroslav.github.io/time-shield-cpp/ "newyaroslav.github.io/time-shield-cpp".

\section repo_sec Repository

[Time Shield Library GitHub repository](https://github.com/NewYaroslav/time-shield-cpp)

\section license_sec License

This library is licensed under the MIT License. See the LICENSE file for more details.