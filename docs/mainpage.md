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

namespace tsh = time_shield;
namespace tshield = time_shield;

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

Additional example files are located in the `examples/` folder:

- `time_utils_example.cpp` — get timestamps and parts
- `time_formatting_example.cpp` — to_string, ISO8601, MQL5
- `time_parser_example.cpp` — parse ISO8601
- `time_conversions_example.cpp` — convert between formats
- `time_zone_conversions_example.cpp` — CET/EET ↔ GMT
- `ntp_client_example.cpp` — NTP sync (Windows-only)

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