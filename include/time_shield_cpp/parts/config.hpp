#pragma once
/// \file config.hpp
/// \brief Header file with preprocessor definitions.
///
/// This file contains preprocessor definitions related to C++ standards and constexpr usage.
#ifndef _TIME_SHIELD_CONFIG_HPP_INCLUDED
#define _TIME_SHIELD_CONFIG_HPP_INCLUDED

// Check if the C++ version is at least C++17
#if __cplusplus >= 201703L
#   define  TIME_SHIELD_CPP17
#endif

// Check if the C++ version is C++11 or lower
#if __cplusplus <= 201103L
#   define  TIME_SHIELD_CONSTEXPR
#   define  TIME_SHIELD_CPP11
#else
#   define  TIME_SHIELD_CONSTEXPR       constexpr
#endif

#endif // _TIME_SHIELD_CONFIG_HPP_INCLUDED
