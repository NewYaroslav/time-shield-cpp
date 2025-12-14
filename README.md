# Time Shield

<img src="docs/logo-1280x640.png" alt="Logo" width="600"/>

![MIT License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20MQL5-blue)
![C++ Standard](https://img.shields.io/badge/C++-11--17-orange)
![CI Windows](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=Windows&logo=windows)
![CI Linux](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=Linux&logo=linux)
![CI macOS](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=macOS&logo=apple)

**Time Shield** is a header-only C++ library for working with time. It provides
functions for converting time values, formatting dates and many utilities for
working with timestamps.
> Named after Homura Akemi's "time shield".

See the [Russian version](README-RU.md) for documentation in Russian.

## Why Time Shield?

**Time Shield** was created as a practical tool for handling time in C++ with a
focus on application and engineering tasks. Unlike the standard `std::chrono` or
more academic solutions like `HowardHinnant/date`, the library:

- uses simple types (`int64_t`, `double`) to represent time (`ts_t`, `fts_t`)
  making them easy to log, serialize and pass through JSON, RPC and databases;
  no overloaded `std::chrono` classes;
- supports **multiple time representations**: Unix time, fractional seconds,
  milli- and microseconds, OLE Automation (Excel), Julian dates;
- includes **utilities for rounding, formatting, ISO8601 parsing**, working with
  parts of a timestamp and calculating period boundaries;
- has an **extensible architecture**—new formats (Julian, OLE, UTC offset) can be
  added as separate types and modules;
- **works even in restricted environments** such as MQL5/MetaTrader—no
  exceptions, STL containers or dynamic memory are required;
- ships as **header-only**—a single include without build steps or external
  dependencies;
- uses only **standard STL headers and system APIs**; modules that depend on
  WinAPI (e.g., `NtpClient`) are isolated and do not hinder cross-platform
  builds.

## Features

- **Date validation**—checks dates for leap years, weekends, and workdays (including ISO8601 strings).
- **Time formatting**—converts timestamps to strings with standard or custom
  templates.
- **Conversions**—translates between second, millisecond and floating time
  representations, `DateTimeStruct` and time zones.
- **Utilities**—fetches current timestamps, computes start/end of periods and
  works with fractions of a second.
- **Time zone conversion**—functions for CET/EET to GMT.
- **NTP client**—obtains accurate time over the network (Windows only).
- **MQL5 support**—adapted headers in the `MQL5` directory allow using the
  library in MetaTrader.
- Compatible with `C++11`–`C++17`.


## Configuration

Compile-time flags in `time_shield/config.hpp` control optional parts of the
library and report platform capabilities:

- `TIME_SHIELD_PLATFORM_WINDOWS` / `TIME_SHIELD_PLATFORM_UNIX` — detected
  target platform.
- `TIME_SHIELD_HAS_WINSOCK` — set when WinSock APIs are available.
- `TIME_SHIELD_ENABLE_NTP_CLIENT` — enables the optional `NtpClient` module
  (defaults to `1` on Windows).

All public headers place their declarations inside the `time_shield` namespace.
Use `time_shield::` or `using namespace time_shield;` to access the API.

> Some functions depend on WinAPI and work only on Windows (for example,
> `NtpClient` or obtaining realtime via `QueryPerformanceCounter`).

## API Invariants

- `ts_t` stores Unix seconds in a signed 64-bit integer and provides
  microsecond precision when converted to other types.
- `fts_t` uses double precision floating seconds. Conversions between
  representations preserve microsecond accuracy.
- ISO8601 utilities assume the proleptic Gregorian calendar and do not account
  for leap seconds.
- Core functions avoid throwing exceptions and do not allocate dynamic memory;
  helpers returning `std::string` rely on the caller to manage allocations.

## Versioning

Time Shield follows [Semantic Versioning](https://semver.org). Patch releases
contain only backward-compatible fixes. Minor versions add
backward-compatible features. Major versions may include breaking changes. The
public API comprises headers under `include/time_shield`.

## Installation

### Install and `find_package`

After installing the library (e.g., with `cmake --install`), consume it in a
project with CMake:

```cmake
cmake_minimum_required(VERSION 3.15)
project(app LANGUAGES CXX)

find_package(TimeShield CONFIG REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE time_shield::time_shield)
```

### Git submodule with `add_subdirectory`

Vendor the library as a submodule:

```sh
git submodule add https://github.com/NewYaroslav/time-shield-cpp external/time-shield-cpp
```

Then include it:

```cmake
add_subdirectory(external/time-shield-cpp)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE time_shield::time_shield)
```

### vcpkg overlay

Install via a local overlay port:

```sh
vcpkg install time-shield-cpp --overlay-ports=./vcpkg-overlay/ports
```

Use the vcpkg toolchain when configuring CMake:

```sh
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

The port is intended to be upstreamed to [microsoft/vcpkg](https://github.com/microsoft/vcpkg).

Examples can be built with the provided scripts:

- `build-examples.bat` for Windows;
- `build_examples.sh` for Linux/macOS;
- `build-cb.bat` to generate a Code::Blocks project.

Use `install_mql5.bat` to install the MQL5 files.

## Tested Platforms

| Platform | Compilers           | C++ Standards |
|----------|---------------------|---------------|
| Windows  | MSVC, ClangCL       | 11, 14, 17    |
| Linux    | GCC, Clang          | 11, 14, 17    |
| macOS    | Apple Clang         | 11, 14, 17    |

## Usage Examples

Below are small examples from different sections of the library.

### Getting and converting time

```cpp
#include <time_shield.hpp>

using namespace time_shield;

ts_t now = ts();                 // seconds since epoch
fts_t now_f = fts();             // time in seconds with fraction
int ms_part = ms_of_sec(now_f);  // millisecond part
```

### Date formatting

```cpp
#include <time_shield.hpp>

std::string iso = to_iso8601(now);          // 2024-06-21T12:00:00
std::string custom = to_string("%Y-%m-%d %H:%M:%S", now);
std::string mql5 = to_mql5_date_time(now);  // 2024.06.21 12:00:00
std::string filename = to_windows_filename(now);
```

### ISO8601 parsing

```cpp
#include <time_shield.hpp>

DateTimeStruct dt;
TimeZoneStruct tz;
if (parse_iso8601("2024-11-25T14:30:00-05:30", dt, tz)) {
    ts_t ts_val = to_timestamp(dt) + to_offset(tz);
}
```

### Checking workdays

```cpp
#include <time_shield/validation.hpp>
#include <time_shield/time_parser.hpp>

using namespace time_shield;

bool monday = is_workday(1710720000);                    // seconds timestamp
bool monday_ms = is_workday_ms("2024-03-18T09:00:00.500Z"); // ISO8601 with milliseconds
bool from_date = is_workday(2024, 3, 18);                 // year, month, day components
bool from_str = is_workday("2024-03-18T09:00:00Z");     // ISO8601 string
```

The string helpers accept the same ISO8601 formats as `str_to_ts` / `str_to_ts_ms` and evaluate to `false` when parsing fails or the date falls on a weekend.

### Locating first and last workdays

```cpp
#include <time_shield/time_conversions.hpp>

using namespace time_shield;

ts_t first_open = start_of_first_workday_month(2024, 6);     // 2024-06-03T00:00:00Z
ts_t first_close = end_of_first_workday_month(2024, Month::JUN);
ts_t last_open = start_of_last_workday_month(2024, 3);        // 2024-03-29T00:00:00Z
ts_ms_t last_close_ms = end_of_last_workday_month_ms(2024, Month::MAR);
```

The helpers reuse the `start_of_day` / `end_of_day` semantics and therefore return UTC timestamps. Invalid months or months without workdays produce `ERROR_TIMESTAMP` to simplify downstream validation.

### Time zone conversion

```cpp
#include <time_shield.hpp>

ts_t cet = to_ts(2024, Month::JUN, 21, 12, 0, 0);
ts_t gmt = cet_to_gmt(cet);
```

### NTP client (Windows)

```cpp
#include <time_shield.hpp>

NtpClient client;
if (client.query()) {
    int64_t offset = client.get_offset_us();
    int64_t utc_ms = client.get_utc_time_ms();
}
```

## Documentation

Full API description and additional examples are available at
<https://newyaroslav.github.io/time-shield-cpp/>

## License

The project is distributed under the [MIT](LICENSE) license.
