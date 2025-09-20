# Changelog

All notable changes to this project will be documented in this file.

## [v1.0.4] - 2025-09-20
- fix ODR violations in headers

## [v1.0.3] - 2025-09-10
- Added pkg-config template and vcpkg overlay port.
- Added optional NTP client install toggle and configuration flags.
- Expanded time parser tests and DST boundary coverage.
- Lowered minimum required CMake version to 3.15.
- Improved installation and consumer build scripts.

## [v1.0.2] - 2025-07-20
- Removed invalid const qualifiers from return types in time utilities.
- Fixed compiler warnings and added dt_to_timestamp example.

## [v1.0.1] - 2025-06-25
- Optimized sec_of_day and other parsing helpers.
- Added WinSock-based NtpClient and timezone conversion utilities.
- Provided usage examples and expanded documentation.

## [v1.0.0] - 2025-06-21
- Initial release with MQL5 Unix year helpers.
