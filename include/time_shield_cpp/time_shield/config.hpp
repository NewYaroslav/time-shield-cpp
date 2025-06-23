#pragma once
#ifndef _TIME_SHIELD_CONFIG_HPP_INCLUDED
#define _TIME_SHIELD_CONFIG_HPP_INCLUDED

/// \file config.hpp
/// \brief Header file with preprocessor definitions for C++ standards and `constexpr` usage.
///
/// This file defines macros to check the C++ standard version being used and configure
/// `constexpr` and `if constexpr` support accordingly.

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

#endif // _TIME_SHIELD_CONFIG_HPP_INCLUDED
