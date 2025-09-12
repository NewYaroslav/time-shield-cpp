// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_CONFIG_HPP_INCLUDED
#define _TIME_SHIELD_CONFIG_HPP_INCLUDED

/// \file config.hpp
/// \brief Configuration macros for the library.
///
/// This header provides compile-time options for C++ standard detection,
/// platform capabilities and optional features. The macros can be used to
/// enable or disable parts of the library depending on the target platform or
/// user preferences.

#if defined(_MSVC_LANG)
#   define TIME_SHIELD_CXX_VERSION _MSVC_LANG
#else
#   define TIME_SHIELD_CXX_VERSION __cplusplus
#endif

// Check and define macros based on the C++ standard version
#if TIME_SHIELD_CXX_VERSION >= 201703L
#   define TIME_SHIELD_CPP17
#elif TIME_SHIELD_CXX_VERSION >= 201402L
#   define TIME_SHIELD_CPP14
#elif TIME_SHIELD_CXX_VERSION >= 201103L
#   define TIME_SHIELD_CPP11
#else
#   error "C++11 or newer is required to compile this library."
#endif

// Configure support for `constexpr` and `if constexpr` based on the C++ standard
#ifdef TIME_SHIELD_CPP11
#   define  TIME_SHIELD_IF_CONSTEXPR
#   define  TIME_SHIELD_CONSTEXPR
#else
#ifdef TIME_SHIELD_CPP14
#   define  TIME_SHIELD_IF_CONSTEXPR
#   define  TIME_SHIELD_CONSTEXPR       constexpr
#else
#ifdef TIME_SHIELD_CPP17
#   define  TIME_SHIELD_IF_CONSTEXPR    constexpr
#   define  TIME_SHIELD_CONSTEXPR       constexpr
#endif
#endif
#endif

/// \name Platform detection
///@{
#if defined(_WIN32)
#   define TIME_SHIELD_PLATFORM_WINDOWS 1
#else
#   define TIME_SHIELD_PLATFORM_WINDOWS 0
#endif

#if defined(__unix__) || defined(__unix) || defined(unix) || \
    (defined(__APPLE__) && defined(__MACH__))
#   define TIME_SHIELD_PLATFORM_UNIX 1
#else
#   define TIME_SHIELD_PLATFORM_UNIX 0
#endif
///@}

/// \name Platform capabilities
///@{
#if TIME_SHIELD_PLATFORM_WINDOWS
#   define TIME_SHIELD_HAS_WINSOCK 1
#else
#   define TIME_SHIELD_HAS_WINSOCK 0
#endif
///@}

/// \name Optional features
///@{
#ifndef TIME_SHIELD_ENABLE_NTP_CLIENT
#   if TIME_SHIELD_HAS_WINSOCK
#       define TIME_SHIELD_ENABLE_NTP_CLIENT 1
#   else
#       define TIME_SHIELD_ENABLE_NTP_CLIENT 0
#   endif
#endif
///@}

#endif // _TIME_SHIELD_CONFIG_HPP_INCLUDED
