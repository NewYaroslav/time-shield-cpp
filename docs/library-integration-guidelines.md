# Library Integration Guidelines

This document defines the supported integration contract for consuming Time Shield as a header-only dependency.

## Canonical Language

English is the canonical version of this document.

Russian localization:
- `docs/library-integration-guidelines-RU.md`

## Supported Integration Shapes

Time Shield is a header-only library. The normal supported integration paths are:

- CMake `find_package(TimeShield CONFIG REQUIRED)`
- CMake `add_subdirectory(...)`
- manual include-path integration for non-CMake builds

Link the exported target when CMake is available:

```cmake
target_link_libraries(app PRIVATE time_shield::time_shield)
```

## Multiple Static Libraries

It is valid for multiple static libraries in the same final program to depend on `time_shield::time_shield`.

This does not create a source-duplication problem as long as consumers:

- include the public Time Shield headers normally
- do not compile private copies of the library internals as separate `.cpp` files
- do not mix incompatible compile-time configuration for the same final program

## NTP Service Contract

`NtpTimeService` is header-only and supports `C++11`, `C++14`, and `C++17`.

- the service uses an immortal singleton internally to avoid static destruction order problems
- during normal runtime the singleton getters keep their lazy-start behavior
- during process teardown the service stops restarting background work and falls back to realtime plus the last cached offset
- in general, `C++17+` allows a simpler singleton-storage pattern with inline variables, and the public usage contract for `NtpTimeService` is the same in `C++11`/`C++14`/`C++17`

## Windows Linking

When `TIME_SHIELD_ENABLE_NTP_CLIENT=ON` on Windows, the NTP stack requires the platform socket system library.

Supported behavior:

- if you use the exported CMake target `time_shield::time_shield`, the required Windows socket library is linked transitively
- if you integrate manually without the CMake target, you must link the platform socket library yourself (for Windows, `ws2_32`)

## Configuration Consistency

Do not mix incompatible compile-time configuration for the same final program.

In particular:

- all consumers in one program should agree on whether `TIME_SHIELD_ENABLE_NTP_CLIENT` is enabled
- public behavior that depends on compile-time configuration must stay consistent across static libraries and the final executable

## Practical Checklist

Before finalizing integration, verify:

- the program links `time_shield::time_shield` through CMake when possible
- multiple static libraries only include public Time Shield headers and do not build private copies of internals
- `NtpTimeService` is used without ownership macros or a special owning TU
- Windows builds with NTP enabled resolve the socket library transitively, or link it manually in non-CMake builds
- all targets in the final program use compatible Time Shield compile-time configuration
