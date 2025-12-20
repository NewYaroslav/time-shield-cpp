# Time Shield

<img src="docs/logo-1280x640.png" alt="Logo" width="600"/>

![MIT License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS%20%7C%20MQL5-blue)
![C++ Standard](https://img.shields.io/badge/C++-11--17-orange)
![CI Windows](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=Windows&logo=windows)
![CI Linux](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=Linux&logo=linux)
![CI macOS](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=macOS&logo=apple)

**Time Shield** is a header-only C++ library for working with time. It provides
functions for converting time values, formatting dates and many utilities for
working with timestamps.
> Named after Homura Akemi's "time shield".

See the [Russian version](README-RU.md) for documentation in Russian.

## Quick start

```cpp
#include <time_shield.hpp>

using namespace time_shield;

ts_t now = ts();
std::string iso = to_iso8601(now);
ts_t gmt = cet_to_gmt(now);
bool monday = is_workday(now);
```

Use `#include <time_shield.hpp>` for the full API, or include specific headers
for a minimal build.

## Why Time Shield?

**Time Shield** was created as a practical tool for handling time in C++ with a
focus on application and engineering tasks. Unlike the standard `std::chrono` or
more academic solutions like `HowardHinnant/date`, the library:

- uses simple types (`int64_t`, `double`) to represent time (`ts_t`, `fts_t`)
  making them easy to log, serialize and pass through JSON, RPC and databases;
  no overloaded `std::chrono` classes;
- supports **multiple time representations**: Unix time, fractional seconds,
  milli- and microseconds, OLE Automation (Excel), Julian dates;
- includes **utilities for rounding, formatting, ISO 8601 parsing**, working with
  parts of a timestamp and calculating period boundaries;
- has an **extensible architecture**—new formats (Julian, OLE, UTC offset) can be
  added as separate types and modules;
- **works even in restricted environments** such as MQL5/MetaTrader—no
  exceptions or dynamic memory are required for the core API; optional helpers
  can use `std::string` or throw and can be disabled in restricted builds;
- ships as **header-only**—a single include without build steps or external
  dependencies;
- uses only **standard STL headers and system APIs**; platform-specific modules
  (e.g., the socket-based `NtpClient`) are isolated and do not hinder
  cross-platform builds.

## Features

- **Date & calendar helpers**—validation, weekdays/weekends/workdays (including ISO 8601 strings).
- **Time formatting**—converts timestamps to strings with standard or custom
  templates.
- **Conversions**—translates between second, millisecond and floating time
  representations, `DateTimeStruct`, OLE Automation dates and time zones.
- **Fast date conversions**—some timestamp-to-calendar helpers use a fast
  algorithm inspired by https://www.benjoffe.com/fast-date-64 and implemented from scratch.
- **DateTime value type**—fixed-offset wrapper that stores UTC milliseconds,
  parses/prints ISO 8601, exposes local/UTC components, and provides arithmetic
  helpers.
- **ISO week dates**—conversion helpers, formatting, and parsing for ISO 8601
  week-numbering years.
- **Astronomy utilities**—computes Julian Date/MJD/JDN values and estimates
  lunar phase/age from Unix timestamps.
- **Utilities**—fetches current timestamps, computes start/end of periods and
  works with fractions of a second.
- **Time zone conversion**—functions for CET/EET/ET/CT to GMT.
- **NTP client and pool**—single-client queries plus a configurable pool/runner/service pipeline with offline testing hooks (Windows and Unix).
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
  (defaults to `1` on supported platforms).

All public headers place their declarations inside the `time_shield` namespace.
Use `time_shield::` or `using namespace time_shield;` to access the API.

> Some functions depend on platform APIs and may be limited (for example,
> obtaining realtime via `QueryPerformanceCounter` on Windows).

## API Invariants

- `ts_t` — Unix time in seconds (signed 64-bit). Represents whole seconds.
- `ts_ms_t` / `ts_us_t` — Unix time in milli/microseconds (signed 64-bit).
- `fts_t` — Unix time in seconds as `double`. Fractional precision depends on
  magnitude; near the modern epoch it typically preserves microseconds, while
  very large |ts| values can lose lower bits.
- `year_t` — signed 64-bit year.
- `dse_t` / `unix_day_t` / `unixday_t` — count of days since 1970-01-01. The
  signedness of the type determines correctness for dates before the epoch.
- ISO 8601 utilities use the proleptic Gregorian calendar and do not account
  for leap seconds.
- Core conversions and “hot” functions aim for `noexcept` and no dynamic
  allocations; string/parsing routines and some high-level helpers may allocate
  and/or throw (as documented per function).

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

### ISO 8601 parsing

```cpp
#include <time_shield.hpp>

DateTimeStruct dt;
TimeZoneStruct tz;
if (parse_iso8601("2024-11-25T14:30:00-05:30", dt, tz)) {
    ts_t ts_val = to_timestamp(dt) + to_offset(tz);
}
```

### DateTime value type

`DateTime` stores UTC milliseconds plus an optional fixed offset for local
component access and round-trip formatting.

```cpp
#include <time_shield.hpp>

using namespace time_shield;

DateTime dt = DateTime::parse_iso8601("2025-12-16T10:20:30.123+02:30");
std::string local = dt.to_iso8601();        // preserves +02:30
std::string utc = dt.to_iso8601_utc();      // prints Z
DateTime tomorrow = dt.add_days(1).start_of_day();
int hour_local = dt.hour();                 // local hour (offset applied)
int hour_utc = dt.utc_hour();               // UTC hour
```

### Checking workdays

```cpp
#include <time_shield/validation.hpp>
#include <time_shield/time_parser.hpp>

using namespace time_shield;

bool monday = is_workday(1710720000);                    // seconds timestamp
bool monday_ms = is_workday_ms("2024-03-18T09:00:00.500Z"); // ISO 8601 with milliseconds
bool from_date = is_workday(2024, 3, 18);                 // year, month, day components
bool from_str = is_workday("2024-03-18T09:00:00Z");     // ISO 8601 string
```

The string helpers accept the same ISO 8601 formats as `str_to_ts` / `str_to_ts_ms` and evaluate to `false` when parsing fails or the date falls on a weekend.

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

### OLE Automation (OA) date conversions

OA conversions are Excel/COM compatible (base date 1899-12-30), operate in UTC, and mirror the round-toward-zero semantics used by OA serials.

```cpp
#include <time_shield/ole_automation_conversions.hpp>

using namespace time_shield;

oadate_t oa = ts_to_oadate(1714608000);                     // 2024-05-02 00:00:00Z
ts_t ts_from_oa = oadate_to_ts(oa);                         // round toward zero
oadate_t from_parts = to_oadate(2024, Month::MAY, 2, 12, 0); // 2024-05-02 12:00:00Z
```

### Julian date and lunar helpers

The astronomy helpers provide lightweight analytics-oriented values (JD, MJD, JDN, phase, age) rather than high-precision ephemerides.

```cpp
#include <time_shield/astronomy_conversions.hpp>

using namespace time_shield;

jd_t jd = ts_to_jd(1714608000);               // Julian Date for the timestamp
mjd_t mjd = ts_to_mjd(1714608000);             // Modified Julian Date
jdn_t jdn = gregorian_to_jdn(2, 5, 2024);      // Julian Day Number
double phase = moon_phase(fts());              // lunar phase fraction [0..1)
double age_days = moon_age_days(fts());        // approximate lunar age
MoonPhaseSineCosine signal = moon_phase_sincos(fts()); // sin/cos of the phase angle (continuous)
MoonQuarterInstants quarters = moon_quarters(fts());   // nearest quarter instants (Unix seconds, double)
bool is_near_new = is_new_moon_window(fts());  // inside +/-12h new moon window
```

### Geocentric Moon phase calculator

`MoonPhaseCalculator` (`time_shield::astronomy::MoonPhase`) exposes richer geocentric outputs (illumination, angular diameters, distance, phase angle), sin/cos for a continuous phase signal, quarter instants, and phase event windows. The current math is geocentric (Earth-centered) without topocentric corrections, so phase and illumination are “global” for a given moment. What varies locally:

- local date/time (timezone conversion),
- visual orientation of the lit part (inverted between hemispheres),
- visibility/observability, which depends on atmosphere and altitude above the horizon.

> Current math is geocentric (Sun/Moon positions relative to Earth’s center, without topocentric parallax). This means illumination and the phase angle are primarily “global” at a given instant for the Earth as a whole. Locally, what actually differs is:
> - calendar date/time via timezone shifts,
> - apparent orientation of the illuminated side (flipped between northern/southern hemispheres),
> - visibility (e.g., first crescent) driven by atmosphere/horizon/altitude rather than the geocentric phase itself.

```cpp
#include <time_shield/MoonPhase.hpp>

using namespace time_shield;

MoonPhaseCalculator calculator{};
const double ts = 1704067200.0; // 2024-01-01T00:00:00Z
MoonPhaseResult res = calculator.compute(ts);
MoonPhase::quarters_unix_s_t quarters = calculator.quarter_times_unix(ts); // Unix seconds (double)
MoonQuarterInstants around = moon_quarters(ts);
MoonPhaseSineCosine signal = moon_phase_sincos(ts);
bool is_new = calculator.is_new_moon_window(ts); // +/-12h window by default
```

### Time zone conversion

```cpp
#include <time_shield.hpp>

ts_t cet = to_ts(2024, Month::JUN, 21, 12, 0, 0);
ts_t gmt = cet_to_gmt(cet);
```

### NTP client, pool, and time service

```cpp
#include <time_shield/ntp_client_pool.hpp>
#include <time_shield/ntp_time_service.hpp>

using namespace time_shield;

NtpClientPool pool;
pool.set_default_servers();
pool.measure();
int64_t pool_offset = pool.offset_us();

// Background runner + lazy singleton service via wrapper functions:
ntp::init(std::chrono::seconds(30));
int64_t utc_ms = ntp::utc_time_ms();
int64_t offset_us = ntp::offset_us();
int64_t utc_sec = ntp::utc_time_sec();
bool ok = ntp::last_measure_ok();
uint64_t attempts = ntp::measure_count();
ntp::shutdown();
```

## Documentation

Full API description and additional examples are available at
<https://newyaroslav.github.io/time-shield-cpp/>

Doxygen HTML is published via GitHub Pages.

## License

The project is distributed under the [MIT](LICENSE) license.
