#pragma once
#ifndef _TIME_SHIELD_TIME_UTILS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_UTILS_HPP_INCLUDED

/// \file time_utils.hpp
/// \brief Header file with time-related utility functions.
///
/// This file contains various functions used for time calculations and conversions.

#include "config.hpp"
#include <ctime>

namespace time_shield {

/// \defgroup time_utils Time Utilities
/// \brief A collection of utility functions for working with timestamps and time components.
///
/// This module provides functions for obtaining the current timestamps in various formats
/// (e.g., seconds, milliseconds, microseconds, floating-point seconds) and extracting
/// sub-second components such as nanoseconds, microseconds, and milliseconds.
///
/// ### Key Features:
/// - Obtain current UTC timestamps in seconds, milliseconds, and microseconds.
/// - Support for floating-point and integer timestamp formats.
/// - Extract nanoseconds, microseconds, and milliseconds from the current second.
///
/// ### Usage Examples:
/// - Get the current timestamp in seconds:
///   \code{.cpp}
///   time_shield::ts_t current_time = time_shield::ts();
///   \endcode
///
/// - Get the millisecond part of the current second:
///   \code{.cpp}
///   int ms = time_shield::ms_of_sec();
///   \endcode
///
/// - Get the current timestamp in milliseconds:
///   \code{.cpp}
///   time_shield::ts_ms_t current_time_ms = time_shield::ts_ms();
///   \endcode
///
/// \{

	/// \brief Get the current timespec.
	/// \return struct timespec The current timespec.
	inline const struct timespec get_timespec_impl() noexcept {
		// https://en.cppreference.com/w/c/chrono/timespec_get
		struct timespec ts;
#		if defined(CLOCK_REALTIME)
		clock_gettime(CLOCK_REALTIME, &ts); // Версия для POSIX
#		else
		timespec_get(&ts, TIME_UTC);
#		endif
		return ts;
	}

	/// \brief Get the nanosecond part of the current second.
	/// \tparam T Type of the returned value (default is int).
	/// \return T Nanosecond part of the current second.
	template<class T = int>
	inline const T ns_of_sec() noexcept {
		const struct timespec ts = get_timespec_impl();
		return static_cast<T>(ts.tv_nsec);
	}

	/// \brief Get the microsecond part of the current second.
	/// \tparam T Type of the returned value (default is int).
	/// \return T Microsecond part of the current second.
	template<class T = int>
	const T us_of_sec() noexcept {
		const struct timespec ts = get_timespec_impl();
		return ts.tv_nsec / NS_PER_US;
	}

	/// \brief Get the millisecond part of the current second.
	/// \tparam T Type of the returned value (default is int).
	/// \return T Millisecond part of the current second.
	template<class T = int>
	const T ms_of_sec() noexcept {
		const struct timespec ts = get_timespec_impl();
		return ts.tv_nsec / NS_PER_MS;
	}

	/// \brief Get the current UTC timestamp in seconds.
	/// \return ts_t Current UTC timestamp in seconds.
	inline const ts_t ts() noexcept {
		const struct timespec ts = get_timespec_impl();
		return ts.tv_sec;
	}

	/// \brief Get the current UTC timestamp in seconds.
	/// \return ts_t Current UTC timestamp in seconds.
	inline const ts_t timestamp() noexcept {
		const struct timespec ts = get_timespec_impl();
		return ts.tv_sec;
	}

	/// \brief Get the current UTC timestamp in floating-point seconds.
	/// \return fts_t Current UTC timestamp in floating-point seconds.
	inline const fts_t fts() noexcept {
		const struct timespec ts = get_timespec_impl();
		return ts.tv_sec + static_cast<fts_t>(ts.tv_nsec) / static_cast<fts_t>(NS_PER_SEC);
	}

	/// \brief Get the current UTC timestamp in floating-point seconds.
	/// \return fts_t Current UTC timestamp in floating-point seconds.
	inline const fts_t ftimestamp() noexcept {
		const struct timespec ts = get_timespec_impl();
		return ts.tv_sec + static_cast<fts_t>(ts.tv_nsec) / static_cast<fts_t>(NS_PER_SEC);
	}

	/// \brief Get the current UTC timestamp in milliseconds.
	/// \return ts_ms_t Current UTC timestamp in milliseconds.
	inline const ts_ms_t ts_ms() noexcept {
		const struct timespec ts = get_timespec_impl();
		return MS_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_MS;
	}

	/// \brief Get the current UTC timestamp in milliseconds.
	/// \return ts_ms_t Current UTC timestamp in milliseconds.
	inline const ts_ms_t timestamp_ms() noexcept {
		const struct timespec ts = get_timespec_impl();
		return MS_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_MS;
	}

	/// \brief Get the current UTC timestamp in milliseconds.
	/// \return ts_ms_t Current UTC timestamp in milliseconds.
	inline const ts_ms_t now() noexcept {
        const struct timespec ts = get_timespec_impl();
		return MS_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_MS;
	}

	/// \brief Get the current UTC timestamp in microseconds.
	/// \return ts_us_t Current UTC timestamp in microseconds.
	inline const ts_us_t ts_us() noexcept {
		const struct timespec ts = get_timespec_impl();
		return US_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_US;
	}

	/// \brief Get the current UTC timestamp in microseconds.
	/// \return ts_us_t Current UTC timestamp in microseconds.
	inline const ts_us_t timestamp_us() noexcept {
		const struct timespec ts = get_timespec_impl();
		return US_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_US;
	}

/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_UTILS_HPP_INCLUDED
