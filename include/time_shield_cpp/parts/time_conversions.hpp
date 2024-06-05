#pragma once
/// \file time_conversions.hpp
/// \brief Header file for time conversion functions.
///
/// This file contains functions for converting between different time representations
/// and performing various time-related calculations.
#ifndef _TIME_SHIELD_TIME_CONVERSIONS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_CONVERSIONS_HPP_INCLUDED

#include "enums.hpp"
#include "validation.hpp"
#include "time_utils.hpp"
#include "time_zone_struct.hpp"
#include <cmath>
#include <ctime>

namespace time_shield {

	/// \brief Get the nanosecond part of the second from a floating-point timestamp.
	/// \tparam T Type of the returned value (default is int).
	/// \param ts Timestamp in floating-point seconds.
	/// \return T Nanosecond part of the second.
	template<class T = int>
	constexpr const T ns_of_sec(const fts_t& ts) noexcept {
		fts_t temp;
		return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(NS_PER_SEC)));
	}

	/// \brief Get the microsecond part of the second from a floating-point timestamp.
	/// \tparam T Type of the returned value (default is int).
	/// \param ts Timestamp in floating-point seconds.
	/// \return T Microsecond part of the second.
	template<class T = int>
	constexpr const T us_of_sec(const fts_t& ts) noexcept {
		fts_t temp;
		return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(US_PER_SEC)));
	}

	/// \brief Get the millisecond part of the second from a floating-point timestamp.
	/// \tparam T Type of the returned value (default is int).
	/// \param ts Timestamp in floating-point seconds.
	/// \return T Millisecond part of the second.
	template<class T = int>
	constexpr const T ms_of_sec(const fts_t& ts) noexcept {
		fts_t temp;
		return static_cast<T>(std::round(std::modf(ts, &temp) * static_cast<fts_t>(MS_PER_SEC)));
	}

	/// \brief Get the millisecond part of the timestamp.
	/// \tparam T Type of the returned value (default is int).
	/// \param ts Timestamp in milliseconds.
	/// \return T Millisecond part of the timestamp.
	template<class T = int>
	constexpr const T ms_of_ts(const ts_ms_t& ts) noexcept {
		return ts % MS_PER_SEC;
	}

#	ifndef TIME_SHIELD_CPP17
	template<class T>
	constexpr const ts_ms_t sec_to_ms_impl(const T& t, std::true_type) noexcept {
		return static_cast<ts_ms_t>(std::round(t * static_cast<T>(MS_PER_SEC)));
	}

	template<class T>
	constexpr const ts_ms_t sec_to_ms_impl(const T& t, std::false_type) noexcept {
		return static_cast<ts_ms_t>(t) * static_cast<ts_ms_t>(MS_PER_SEC);
	}
#	endif

	/// \brief Converts a timestamp from seconds to milliseconds.
	/// \tparam T1 The type of the output timestamp (default is ts_ms_t).
	/// \tparam T2 The type of the input timestamp.
	/// \param ts Timestamp in seconds.
	/// \return T1 Timestamp in milliseconds.
	template<class T1 = ts_ms_t, class T2>
	constexpr const T1 sec_to_ms(const T2& ts) noexcept {
#		ifdef TIME_SHIELD_CPP17
		if constexpr(std::is_same_v<T2, double>) {
			return static_cast<T1>(std::round(ts * static_cast<T2>(MS_PER_SEC)));
		} else
		if constexpr(std::is_same_v<T2, float>) {
			return static_cast<T1>(std::round(ts * static_cast<T2>(MS_PER_SEC)));
		} else {
			return static_cast<T1>(ts) * static_cast<T1>(MS_PER_SEC);
		}
#		else
		return sec_to_ms_impl(ts, std::is_same<T2, double>());
#		endif
	}

	/// \brief Converts a floating-point timestamp from seconds to milliseconds.
	/// \param ts Timestamp in floating-point seconds.
	/// \return ts_ms_t Timestamp in milliseconds.
	constexpr const ts_ms_t fsec_to_ms(const fts_t& ts) noexcept {
		return static_cast<ts_ms_t>(std::round(ts * static_cast<fts_t>(MS_PER_SEC)));
	}

	/// \brief Converts a timestamp from milliseconds to seconds.
	/// \tparam T1 The type of the output timestamp (default is ts_t).
	/// \tparam T2 The type of the input timestamp (default is ts_ms_t).
	/// \param ts_ms Timestamp in milliseconds.
	/// \return T1 Timestamp in seconds.
	template<class T1 = ts_t, class T2 = ts_ms_t>
	constexpr const T1 ms_to_sec(const T2& ts_ms) noexcept {
		return static_cast<T1>(ts_ms) / static_cast<T1>(MS_PER_SEC);
	}

	/// \brief Converts a timestamp from milliseconds to floating-point seconds.
	/// \tparam T The type of the input timestamp (default is ts_ms_t).
	/// \param ts_ms Timestamp in milliseconds.
	/// \return fts_t Timestamp in floating-point seconds.
	template<class T = ts_ms_t>
	constexpr const fts_t ms_to_fsec(const T& ts_ms) noexcept {
		return static_cast<fts_t>(ts_ms) / static_cast<fts_t>(MS_PER_SEC);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a UNIX timestamp to a year.
	/// \tparam T The type of the year (default is year_t).
	/// \param ts UNIX timestamp.
	/// \return T Year corresponding to the given timestamp.
	template<class T = year_t>
	constexpr const T get_unix_year(const ts_t& ts) noexcept {
		// 9223372029693630000 - значение на момент 292277024400 от 2000 года
		// Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
		// Поэтому пришлось снизить до 9223371890843040000
		const int64_t BIAS_292277022000 = 9223371890843040000LL;
		const int64_t BIAS_2000 = 946684800LL;

		int64_t y = MAX_YEAR;
		int64_t secs = -((ts - BIAS_2000) - BIAS_292277022000);

		const int64_t n_400_years = secs / SEC_PER_400_YEARS;
		secs -= n_400_years * SEC_PER_400_YEARS;
		y -= n_400_years * 400;

		const int64_t n_100_years = secs / SEC_PER_100_YEARS;
		secs -= n_100_years * SEC_PER_100_YEARS;
		y -= n_100_years * 100;

		const int64_t n_4_years = secs / SEC_PER_4_YEARS;
		secs -= n_4_years * SEC_PER_4_YEARS;
		y -= n_4_years * 4;

		const int64_t n_1_years = secs / SEC_PER_YEAR;
		secs -= n_1_years * SEC_PER_YEAR;
		y -= n_1_years;

		y = secs == 0 ? y : y - 1;
		return y - UNIX_EPOCH;
	}

	/// \brief Alias for get_unix_year function.
	/// \copydoc get_unix_year
	template<class T = year_t>
	constexpr const T unix_year(const ts_t& ts) noexcept {
		return get_unix_year(ts);
	}

	/// \brief Alias for get_unix_year function.
	/// \copydoc get_unix_year
	template<class T = year_t>
	constexpr const T to_unix_year(const ts_t& ts) noexcept {
		return get_unix_year(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a 24-hour format hour to a 12-hour format.
	/// \tparam T Numeric type of the hour (default is int).
	/// \param hour The hour in 24-hour format to convert.
	/// \return The hour in 12-hour format.
	template<class T = int>
	TIME_SHIELD_CONSTEXPR inline const T hour24_to_12(const T& hour) noexcept {
		if (hour == 0 || hour > 12) return 12;
		return hour;
	}

	///	\brief Alias for hour24_to_12 function.
	/// \copydoc hour24_to_12
	template<class T = int>
	TIME_SHIELD_CONSTEXPR inline const T h24_to_h12(const T& hour) noexcept {
		return hour24_to_12(hour);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a timestamp to a date-time structure.
	///
	/// This function converts a timestamp (usually an integer representing seconds since epoch)
	/// to a custom date-time structure. The default type for the timestamp is int64_t.
	///
	/// \tparam T1 The date-time structure type to be returned.
	/// \tparam T2 The type of the timestamp (default is int64_t).
	/// \param ts The timestamp to be converted.
	/// \return A date-time structure of type T1.
	template<class T1, class T2 = ts_t>
	T1 to_date_time(const T2& ts) {
		// 9223372029693630000 - значение на момент 292277024400 от 2000 года
		// Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
		// Поэтому пришлось снизить до 9223371890843040000
		const int64_t BIAS_292277022000 = 9223371890843040000LL;
		const int64_t BIAS_2000 = 946684800LL;

		int64_t y = MAX_YEAR;
		uint64_t secs = -((ts - BIAS_2000) - BIAS_292277022000);

		const uint64_t n_400_years = secs / SEC_PER_400_YEARS;
		secs -= n_400_years * SEC_PER_400_YEARS;
		y -= n_400_years * 400;

		const uint64_t n_100_years = secs / SEC_PER_100_YEARS;
		secs -= n_100_years * SEC_PER_100_YEARS;
		y -= n_100_years * 100;

		const uint64_t n_4_years = secs / SEC_PER_4_YEARS;
		secs -= n_4_years * SEC_PER_4_YEARS;
		y -= n_4_years * 4;

		const uint64_t n_1_years = secs / SEC_PER_YEAR;
		secs -= n_1_years * SEC_PER_YEAR;
		y -= n_1_years;

		T1 date_time;

		if (secs == 0) {
			date_time.year = y;
			date_time.mon = 1;
			date_time.day = 1;
			return date_time;
		}

		date_time.year = y - 1;
		const bool is_leap_year = is_leap_year_date(date_time.year);
		secs = is_leap_year ? SEC_PER_LEAP_YEAR - secs : SEC_PER_YEAR - secs;
		const int days = secs / SEC_PER_DAY;

		static const int JAN_AND_FEB_DAY_LEAP_YEAR = 60 - 1;
		static const int TABLE_MONTH_OF_YEAR[] = {
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 31 январь
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,		// 28 февраль
			3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,	// 31 март
			4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,	// 30 апрель
			5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
			6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
			7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
			8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
			9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
			10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
			11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
			12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
		};
		static const int TABLE_DAY_OF_YEAR[] = {
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	// 31 январь
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,				// 28 февраль
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	// 31 март
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,		// 30 апрель
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
		};

		if (is_leap_year) {
			const int prev_days = days - 1;
			date_time.day = days == JAN_AND_FEB_DAY_LEAP_YEAR ? (TABLE_DAY_OF_YEAR[prev_days] + 1) :
				(days > JAN_AND_FEB_DAY_LEAP_YEAR ? TABLE_DAY_OF_YEAR[prev_days] : TABLE_DAY_OF_YEAR[days]);
			date_time.mon = days >= JAN_AND_FEB_DAY_LEAP_YEAR ? TABLE_MONTH_OF_YEAR[prev_days] : TABLE_MONTH_OF_YEAR[days];
		} else {
			date_time.day = TABLE_DAY_OF_YEAR[days];
			date_time.mon = TABLE_MONTH_OF_YEAR[days];
		}

		ts_t day_secs = secs % SEC_PER_DAY;
		date_time.hour = day_secs / SEC_PER_HOUR;
		ts_t min_secs = day_secs - date_time.hour * SEC_PER_HOUR;
		date_time.min = min_secs / SEC_PER_MIN;
		date_time.sec = min_secs - date_time.min * SEC_PER_MIN;
#		ifdef TIME_SHIELD_CPP17
		if constexpr (std::is_floating_point<T2>::value) {
			date_time.ms = static_cast<int>(std::round(std::fmod(static_cast<double>(ts), static_cast<double>(MS_PER_SEC))));
		} else date_time.ms = 0;
#		else
		if (std::is_floating_point<T2>::value) {
			date_time.ms = static_cast<int>(std::round(std::fmod(static_cast<double>(ts), static_cast<double>(MS_PER_SEC))));
		} else date_time.ms = 0;
#		endif
		return date_time;
	}

	///	\brief Alias for to_date_time function.
	/// \copydoc to_date_time
	template<class T1, class T2 = ts_t>
	T1 to_dt(const T2& ts) {
		return to_date_time(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a timestamp in milliseconds to a date-time structure with milliseconds.
	/// \tparam T The type of the date-time structure to return.
	/// \param ts The timestamp in milliseconds to convert.
	/// \return T A date-time structure with the corresponding date and time components.
	template<class T>
	inline T to_date_time_ms(const ts_ms_t& ts) {
		T date_time = to_date_time<T, ts_ms_t>(ms_to_sec<ts_ms_t, ts_ms_t>(ts));
		date_time.ms = ms_of_ts(ts); // Extract and set the ms component
		return date_time;
	}

	///	\brief Alias for to_date_time_ms function.
	/// \copydoc to_date_time_ms
	template<class T>
	inline T to_dt_ms(const ts_ms_t& ts) {
		return to_date_time_ms<T>(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a date and time to a timestamp.
	///
	/// This function converts a given date and time to a timestamp, which is the number
	/// of seconds since the Unix epoch (January 1, 1970).
	///
	/// \tparam T1 The type of the year parameter (default is int64_t).
	/// \tparam T2 The type of the other date and time parameters (default is int).
	/// \param year	The year value.
	/// \param month The month value.
	/// \param day	 The day value.
	/// \param hour	 The hour value (default is 0).
	/// \param min	 The minute value (default is 0).
	/// \param sec	 The second value (default is 0).
	/// \return Timestamp representing the given date and time.
	///	\throws std::invalid_argument if the date-time combination is invalid.
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_t to_timestamp(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0) {

		if (day >= UNIX_EPOCH && year <= 31) {
			return to_timestamp(day, month, year, hour, min, sec);
		}
		if (!is_valid_date_time(year, month, day, hour, min, sec)) {
			throw std::invalid_argument("Invalid date-time combination");
		}

		int64_t secs = 0;
		uint64_t years = (static_cast<int64_t>(MAX_YEAR) - year);

		const int64_t n_400_years = years / 400;
		secs += n_400_years * SEC_PER_400_YEARS;
		years -= n_400_years * 400;

		const int64_t n_100_years = years / 100;
		secs += n_100_years * SEC_PER_100_YEARS;
		years -= n_100_years * 100;

		const int64_t n_4_years = years / 4;
		secs += n_4_years * SEC_PER_4_YEARS;
		years -= n_4_years * 4;

		secs += years * SEC_PER_YEAR;

		// 9223372029693630000 - значение на момент 292277024400 от 2000 года
		// Такое значение приводит к неправильному вычислению умножения n_400_years * SEC_PER_400_YEARS
		// Поэтому пришлось снизить до 9223371890843040000
		const int64_t BIAS_292277022000 = 9223371890843040000LL;
		const int64_t BIAS_2000 = 946684800LL;

		secs = BIAS_292277022000 - secs;
		secs += BIAS_2000;

		if (month == 1 && day == 1 &&
			hour == 0 && min == 0 &&
			sec == 0) {
			return secs;
		}

		const int lmos[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
		const int mos[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

		secs += (is_leap_year_date(year) ? (lmos[month - 1] + day - 1) : (mos[month - 1] + day - 1)) * SEC_PER_DAY;
		secs += SEC_PER_HOUR * hour + SEC_PER_MIN * min + sec;
		return secs;
	}

	///	\brief Alias for to_timestamp function.
	/// \copydoc to_timestamp
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_t to_ts(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0) {
		return to_timestamp<T1, T2>(year, month, day, hour, min, sec);
	}

	///	\brief Alias for to_timestamp function.
	/// \copydoc to_timestamp
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_t get_ts(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0) {
		return to_timestamp<T1, T2>(year, month, day, hour, min, sec);
	}

	///	\brief Alias for to_timestamp function.
	/// \copydoc to_timestamp
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_t ts(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0) {
		return to_timestamp<T1, T2>(year, month, day, hour, min, sec);
	}

	///	\brief Alias for to_timestamp function.
	/// \copydoc to_timestamp
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_t timestamp(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0) {
		return to_timestamp<T1, T2>(year, month, day, hour, min, sec);
	}

	///	\brief Alias for to_timestamp function.
	/// \copydoc to_timestamp
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_t get_timestamp(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0) {
		return to_timestamp<T1, T2>(year, month, day, hour, min, sec);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a date-time structure to a timestamp.
	///
	/// This function converts a given date and time to a timestamp, which is the number
	/// of seconds since the Unix epoch (January 1, 1970).
	///
	/// \tparam T The type of the date-time structure.
	/// \param date_time The date-time structure.
	/// \return Timestamp representing the given date and time.
	///	\throws std::invalid_argument if the date-time combination is invalid.
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t dt_to_timestamp(
			const T& date_time) {
		return to_timestamp(
			date_time.year,
			date_time.mon,
			date_time.day,
			date_time.hour,
			date_time.min,
			date_time.sec);
	}

	///	\brief Alias for dt_to_timestamp function.
	/// \copydoc dt_to_timestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t to_timestamp(
			const T& date_time) {
		return dt_to_timestamp(date_time);
	}

	///	\brief Alias for dt_to_timestamp function.
	/// \copydoc dt_to_timestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t to_ts(
			const T& date_time) {
		return dt_to_timestamp(date_time);
	}

	///	\brief Alias for dt_to_timestamp function.
	/// \copydoc dt_to_timestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t ts(
			const T& date_time) {
		return dt_to_timestamp(date_time);
	}

	///	\brief Alias for dt_to_timestamp function.
	/// \copydoc dt_to_timestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t timestamp(
			const T& date_time) {
		return dt_to_timestamp(date_time);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a std::tm structure to a timestamp.
	///
	/// This function converts a standard C++ std::tm structure to a timestamp, which is the number
	/// of seconds since the Unix epoch (January 1, 1970).
	///
	/// \param timeinfo Pointer to a std::tm structure containing the date and time information.
	/// \return Timestamp representing the given date and time.
	///	\throws std::invalid_argument if the date-time combination is invalid.
	TIME_SHIELD_CONSTEXPR inline const ts_t tm_to_timestamp(
			const std::tm *timeinfo) {
		return to_timestamp(
			timeinfo->tm_year + 1900,
			timeinfo->tm_mon + 1,
			timeinfo->tm_mday,
			timeinfo->tm_hour,
			timeinfo->tm_min,
			timeinfo->tm_sec);
	}

	/*
	///	\brief Alias for tm_to_timestamp function.
	/// \copydoc tm_to_timestamp
	TIME_SHIELD_CONSTEXPR inline const ts_t to_timestamp(
			const std::tm *timeinfo) {
		return tm_to_timestamp(timeinfo);
	}

	///	\brief Alias for tm_to_timestamp function.
	/// \copydoc tm_to_timestamp
	TIME_SHIELD_CONSTEXPR inline const ts_t to_ts(
			const std::tm *timeinfo) {
		return tm_to_timestamp(timeinfo);
	}

	///	\brief Alias for tm_to_timestamp function.
	/// \copydoc tm_to_timestamp
	TIME_SHIELD_CONSTEXPR inline const ts_t ts(
			const std::tm *timeinfo) {
		return tm_to_timestamp(timeinfo);
	}

	///	\brief Alias for tm_to_timestamp function.
	/// \copydoc tm_to_timestamp
	TIME_SHIELD_CONSTEXPR inline const ts_t timestamp(
			const std::tm *timeinfo) {
		return tm_to_timestamp(timeinfo);
	}

	///	\brief Alias for tm_to_timestamp function.
	/// \copydoc tm_to_timestamp
	TIME_SHIELD_CONSTEXPR inline const ts_t ts_from_tm(
			const std::tm *timeinfo) {
		return tm_to_timestamp(timeinfo);
	}

	*/

//------------------------------------------------------------------------------

	/// \brief Converts a date and time to a timestamp in milliseconds.
	///
	/// This function converts a given date and time to a timestamp in milliseconds,
	/// which is the number of milliseconds since the Unix epoch (January 1, 1970).
	///
	/// \tparam T1 The type of the year parameter (default is int64_t).
	/// \tparam T2 The type of the other date and time parameters (default is int).
	/// \param year The year value.
	/// \param month The month value.
	/// \param day The day value.
	/// \param hour The hour value (default is 0).
	/// \param min The minute value (default is 0).
	/// \param sec The second value (default is 0).
	/// \param ms The millisecond value (default is 0).
	/// \return Timestamp in milliseconds representing the given date and time.
	/// \throws std::invalid_argument if the date-time combination is invalid.
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t to_timestamp_ms(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T2& ms	= 0) {
		return sec_to_ms(to_timestamp<T1, T2>(year, month, day, hour, min, sec)) + ms;
	}

	///	\brief Alias for to_timestamp_ms function.
	/// \copydoc to_timestamp_ms
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t to_ts_ms(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T2& ms	= 0) {
		return to_timestamp_ms(year, month, day, hour, min, sec, ms);
	}

	///	\brief Alias for to_timestamp_ms function.
	/// \copydoc to_timestamp_ms
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t ts_ms(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T2& ms	= 0) {
		return to_timestamp_ms(year, month, day, hour, min, sec, ms);
	}

	///	\brief Alias for to_timestamp_ms function.
	/// \copydoc to_timestamp_ms
	template<class T1 = year_t, class T2 = int>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t timestamp_ms(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T2& ms	= 0) {
		return to_timestamp_ms(year, month, day, hour, min, sec, ms);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a date-time structure to a timestamp in milliseconds.
	///
	/// This function converts a given date-time structure to a timestamp in milliseconds,
	/// which is the number of milliseconds since the Unix epoch (January 1, 1970).
	///
	/// \tparam T The type of the date-time structure.
	/// \param date_time The date-time structure.
	/// \return Timestamp in milliseconds representing the given date and time.
	///	\throws std::invalid_argument if the date-time combination is invalid.
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t dt_to_timestamp_ms(
			const T& date_time) {
			/*
		return sec_to_ms(to_timestamp(
			date_time.year,
			date_time.month,
			date_time.day,
			date_time.hour,
			date_time.min,
			date_time.sec)) + date_time.ms;
			*/
		return sec_to_ms(dt_to_timestamp(date_time)) + date_time.ms;
	}

	///	\brief Alias for dt_to_timestamp_ms function.
	/// \copydoc dt_to_timestamp_ms
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t to_timestamp_ms(
			const T& date_time) {
		return dt_to_timestamp_ms(date_time);
	}

	///	\brief Alias for dt_to_timestamp_ms function.
	/// \copydoc dt_to_timestamp_ms
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t to_ts_ms(
			const T& date_time) {
		return dt_to_timestamp_ms(date_time);
	}

	///	\brief Alias for dt_to_timestamp_ms function.
	/// \copydoc dt_to_timestamp_ms
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t ts_ms(
			const T& date_time) {
		return dt_to_timestamp_ms(date_time);
	}

	///	\brief Alias for dt_to_timestamp_ms function.
	/// \copydoc dt_to_timestamp_ms
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const ts_t timestamp_ms(
			const T& date_time) {
		return dt_to_timestamp_ms(date_time);
	}

//------------------------------------------------------------------------------

	/// \brief Converts a std::tm structure to a timestamp in milliseconds.
	///
	/// This function converts a given std::tm structure to a timestamp in milliseconds,
	/// which is the number of milliseconds since the Unix epoch (January 1, 1970).
	///
	/// \param timeinfo Pointer to a std::tm structure containing the date and time information.
	/// \return Timestamp in milliseconds representing the given date and time.
	TIME_SHIELD_CONSTEXPR inline const ts_t tm_to_timestamp_ms(
			const std::tm *timeinfo) {
		return sec_to_ms(tm_to_timestamp(timeinfo));
	}

	///	\brief Alias for tm_to_timestamp_ms function.
	/// \copydoc tm_to_timestamp_ms
	TIME_SHIELD_CONSTEXPR inline const ts_t to_timestamp_ms(
			const std::tm *timeinfo) {
		return tm_to_timestamp_ms(timeinfo);
	}

	///	\brief Alias for tm_to_timestamp_ms function.
	/// \copydoc tm_to_timestamp_ms
	TIME_SHIELD_CONSTEXPR inline const ts_t to_ts_ms(
			const std::tm *timeinfo) {
		return tm_to_timestamp_ms(timeinfo);
	}

	///	\brief Alias for tm_to_timestamp_ms function.
	/// \copydoc tm_to_timestamp_ms
	TIME_SHIELD_CONSTEXPR inline const ts_t ts_ms(
			const std::tm *timeinfo) {
		return tm_to_timestamp_ms(timeinfo);
	}

	///	\brief Alias for tm_to_timestamp_ms function.
	/// \copydoc tm_to_timestamp_ms
	TIME_SHIELD_CONSTEXPR inline const ts_t timestamp_ms(
			const std::tm *timeinfo) {
		return tm_to_timestamp_ms(timeinfo);
	}

//------------------------------------------------------------------------------

	///	\brief Converts a date and time to a floating-point timestamp.
	///
	/// This function converts a given date and time to a floating-point timestamp,
	/// which is the number of seconds (with fractional milliseconds) since the Unix epoch
	/// (January 1, 1970).
	///
	/// \tparam T1 The type of the year parameter (default is year_t).
	/// \tparam T2 The type of the month, day, hour, minute, and second parameters (default is int).
	/// \tparam T3 The type of the millisecond parameter (default is int).
	/// \param year The year value.
	/// \param month The month value.
	/// \param day The day value.
	/// \param hour The hour value (default is 0).
	/// \param min The minute value (default is 0).
	/// \param sec The second value (default is 0).
	/// \param ms The millisecond value (default is 0).
	/// \return Floating-point timestamp representing the given date and time.
	/// \throws std::invalid_argument if the date-time combination is invalid.
	template<class T1 = year_t, class T2 = int, class T3 = int>
	TIME_SHIELD_CONSTEXPR inline const fts_t to_ftimestamp(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T3& ms	= 0) {
		return static_cast<fts_t>(to_timestamp(year, month, day, hour, min, sec)) +
			static_cast<fts_t>(ms)/static_cast<fts_t>(MS_PER_SEC);
	}

	///	\brief Alias for to_ftimestamp function.
	/// \copydoc to_ftimestamp
	template<class T1 = year_t, class T2 = int, class T3 = int>
	TIME_SHIELD_CONSTEXPR inline const fts_t to_fts(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T3& ms	= 0) {
		return to_ftimestamp(year, month, day, hour, min, sec, ms);
	}

	///	\brief Alias for to_ftimestamp function.
	/// \copydoc to_ftimestamp
	template<class T1 = year_t, class T2 = int, class T3 = int>
	TIME_SHIELD_CONSTEXPR inline const fts_t fts(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T3& ms	= 0) {
		return to_ftimestamp(year, month, day, hour, min, sec, ms);
	}

	///	\brief Alias for to_ftimestamp function.
	/// \copydoc to_ftimestamp
	template<class T1 = year_t, class T2 = int, class T3 = int>
	TIME_SHIELD_CONSTEXPR inline const fts_t ftimestamp(
			const T1& year,
			const T2& month,
			const T2& day,
			const T2& hour	= 0,
			const T2& min	= 0,
			const T2& sec	= 0,
			const T3& ms	= 0) {
		return to_ftimestamp(year, month, day, hour, min, sec, ms);
	}

//------------------------------------------------------------------------------

	///	\brief Converts a date-time structure to a floating-point timestamp.
	///
	///	This function converts a given date and time structure to a floating-point timestamp,
	///	which is the number of seconds (with fractional milliseconds) since the Unix epoch
	///	(January 1, 1970).
	///
	///	\tparam T The type of the date-time structure.
	///	\param date_time The date-time structure containing year, month, day, hour, minute, second, and millisecond fields.
	///	\return Floating-point timestamp representing the given date and time.
	/// \throws std::invalid_argument if the date-time combination is invalid.
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const fts_t dt_to_ftimestamp(
			const T& date_time) {
		return static_cast<fts_t>(to_timestamp(date_time)) +
			static_cast<fts_t>(date_time.ms)/static_cast<fts_t>(MS_PER_SEC);
	}

	///	\brief Alias for dt_to_ftimestamp function.
	/// \copydoc dt_to_ftimestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const fts_t to_ftimestamp(
			const T& date_time) {
		return dt_to_ftimestamp(date_time);
	}

	///	\brief Alias for dt_to_ftimestamp function.
	/// \copydoc dt_to_ftimestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const fts_t to_fts(
			const T& date_time) {
		return dt_to_ftimestamp(date_time);
	}

	///	\brief Alias for dt_to_ftimestamp function.
	/// \copydoc dt_to_ftimestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const fts_t fts(
			const T& date_time) {
		return dt_to_ftimestamp(date_time);
	}

	///	\brief Alias for dt_to_ftimestamp function.
	/// \copydoc dt_to_ftimestamp
	template<class T>
	TIME_SHIELD_CONSTEXPR inline const fts_t ftimestamp(
			const T& date_time) {
		return dt_to_ftimestamp(date_time);
	}

//------------------------------------------------------------------------------

	///	\brief Converts a std::tm structure to a floating-point timestamp.
	///
	///	This function converts a given std::tm structure to a floating-point timestamp,
	///	which is the number of seconds (with fractional milliseconds) since the Unix epoch
	///	(January 1, 1970).
	///
	///	\param timeinfo Pointer to the std::tm structure containing the date and time.
	///	\return Floating-point timestamp representing the given date and time.
	///	\throws std::invalid_argument if the date-time combination is invalid.
	TIME_SHIELD_CONSTEXPR inline const fts_t tm_to_ftimestamp(
			const std::tm* timeinfo) {
		return static_cast<fts_t>(tm_to_timestamp(timeinfo));
	}

	///	\brief Alias for tm_to_ftimestamp function.
	/// \copydoc tm_to_ftimestamp
	TIME_SHIELD_CONSTEXPR inline const fts_t to_ftimestamp(
			const std::tm* timeinfo) {
		return tm_to_ftimestamp(timeinfo);
	}

	///	\brief Alias for tm_to_ftimestamp function.
	/// \copydoc tm_to_ftimestamp
	TIME_SHIELD_CONSTEXPR inline const fts_t to_fts(
			const std::tm* timeinfo) {
		return tm_to_ftimestamp(timeinfo);
	}

	///	\brief Alias for tm_to_ftimestamp function.
	/// \copydoc tm_to_ftimestamp
	TIME_SHIELD_CONSTEXPR inline const fts_t fts(
			const std::tm* timeinfo) {
		return tm_to_ftimestamp(timeinfo);
	}

	///	\brief Alias for tm_to_ftimestamp function.
	/// \copydoc tm_to_ftimestamp
	TIME_SHIELD_CONSTEXPR inline const fts_t ftimestamp(
			const std::tm* timeinfo) {
		return tm_to_ftimestamp(timeinfo);
	}

//------------------------------------------------------------------------------

	///	\brief Get UNIX day.
	///
	///	This function returns the number of days elapsed since the UNIX epoch.
	///
	///	\tparam T The return type of the function (default is unix_day_t).
	///	\param ts Timestamp in seconds (default is current timestamp).
	///	\return Number of days since the UNIX epoch.
	template<class T = uday_t>
	constexpr const T get_unix_day(const ts_t& ts = ts()) noexcept {
		return ts / SEC_PER_DAY;
	}

	///	\brief Alias for get_unix_day function.
	/// \copydoc get_unix_day
	template<class T = uday_t>
	constexpr const T unix_day(const ts_t& ts = ts()) noexcept {
		return get_unix_day(ts);
	}

	///	\brief Alias for get_unix_day function.
	/// \copydoc get_unix_day
	template<class T = uday_t>
	constexpr const T uday(const ts_t& ts = ts()) noexcept {
		return get_unix_day(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the number of days between two timestamps.
    ///
    /// This function calculates the number of days between two timestamps.
    ///
    /// \tparam T The type of the return value, defaults to int.
    /// \param start The timestamp of the start of the period.
    /// \param stop The timestamp of the end of the period.
    /// \return The number of days between start and stop.
	template<class T = int>
	constexpr const T get_days_difference(const ts_t& start, const ts_t& stop) noexcept {
		return (stop - start) / SEC_PER_DAY;
	}

	///	\brief Alias for get_days_difference function.
	/// \copydoc get_days_difference
	template<class T = int>
	constexpr const T get_days(const ts_t& start, const ts_t& stop) noexcept {
		return get_days_difference(start, stop);
	}

	///	\brief Alias for get_days_difference function.
	/// \copydoc get_days_difference
	template<class T = int>
	constexpr const T days(const ts_t& start, const ts_t& stop) noexcept {
		return get_days_difference(start, stop);
	}

//------------------------------------------------------------------------------

	///	\brief Get UNIX day from milliseconds timestamp.
	///
	///	This function returns the number of days elapsed since the UNIX epoch, given a timestamp in milliseconds.
	///
	///	\tparam T The return type of the function (default is unix_day_t).
	///	\param t_ms Timestamp in milliseconds (default is current timestamp in milliseconds).
	///	\return Number of days since the UNIX epoch.
	template<class T = uday_t>
	constexpr const T get_unix_day_ms(const ts_ms_t& t_ms = ts_ms()) noexcept {
		return get_unix_day(ms_to_sec(t_ms));
	}

	///	\brief Alias for get_unix_day_ms function.
	/// \copydoc get_unix_day_ms
	template<class T = uday_t>
	constexpr const T unix_day_ms(const ts_ms_t& t_ms = ts_ms()) noexcept {
		return get_unix_day_ms(t_ms);
	}

	///	\brief Alias for get_unix_day_ms function.
	/// \copydoc get_unix_day_ms
	template<class T = uday_t>
	constexpr const T uday_ms(const ts_ms_t& t_ms = ts_ms()) noexcept {
		return get_unix_day_ms(t_ms);
	}

//------------------------------------------------------------------------------

	///	\brief Convert a UNIX day to a timestamp.
	///
	///	This function converts a number of days since the UNIX epoch to a timestamp in seconds.
	///
	///	\tparam T The return type of the function (default is ts_t).
	///	\param unix_day Number of days since the beginning of UNIX time.
	///	\return Timestamp at the beginning of the given day.
	template<class T = ts_t>
	constexpr const T unix_day_to_timestamp(const uday_t& unix_day) noexcept {
		return unix_day * SEC_PER_DAY;
	}

	///	\brief Alias for unix_day_to_timestamp function.
	/// \copydoc unix_day_to_timestamp
	template<class T = ts_t>
	constexpr const T unix_day_to_ts(const uday_t& unix_day) noexcept {
		return unix_day_to_timestamp(unix_day);
	}

	///	\brief Alias for unix_day_to_timestamp function.
	/// \copydoc unix_day_to_timestamp
	template<class T = ts_t>
	constexpr const T uday_to_ts(const uday_t& unix_day) noexcept {
		return unix_day_to_timestamp(unix_day);
	}

//------------------------------------------------------------------------------

	///	\brief Get UNIX minute.
	///
	///	This function returns the number of minutes elapsed since the UNIX epoch.
	///
	///	\tparam T The return type of the function (default is int64_t).
	///	\param ts Timestamp in seconds (default is current timestamp).
	///	\return Number of minutes since the UNIX epoch.
	template<class T = int64_t>
	constexpr const T get_unix_min(const ts_t& ts = ts()) {
		return ts / SEC_PER_MIN;
	}

	///	\brief Alias for get_unix_min function.
	/// \copydoc get_unix_min
	template<class T = int64_t>
	constexpr const T unix_min(const ts_t& ts = ts()) {
		return get_unix_min(ts);
	}

	///	\brief Alias for get_unix_min function.
	/// \copydoc get_unix_min
	template<class T = int64_t>
	constexpr const T to_unix_min(const ts_t& ts = ts()) {
		return get_unix_min(ts);
	}

	///	\brief Alias for get_unix_min function.
	/// \copydoc get_unix_min
	template<class T = int64_t>
	constexpr const T umin(const ts_t& ts = ts()) {
		return get_unix_min(ts);
	}

//------------------------------------------------------------------------------

	///	\brief Get the second of the day.
	///
	///	This function returns a value from 0 to MAX_SEC_PER_DAY representing the second of the day.
	///
	///	\tparam T The return type of the function (default is int).
	///	\param ts Timestamp in seconds (default is current timestamp).
	///	\return Second of the day.
	template<class T = int>
	constexpr const T sec_of_day(const ts_t& ts = ts()) {
		return ts % SEC_PER_DAY;
	}

	///	\brief Get the second of the day from milliseconds timestamp.
	///
	///	This function returns a value from 0 to MAX_SEC_PER_DAY representing the second of the day, given a timestamp in milliseconds.
	///
	///	\tparam T The return type of the function (default is int).
	///	\param ts_ms Timestamp in milliseconds.
	///	\return Second of the day.
	template<class T = int>
	constexpr const T sec_of_day_ms(const ts_ms_t& ts_ms) noexcept {
		return sec_of_day(ms_to_sec(ts_ms));
	}

	///	\brief Get the second of the day.
	///
	///	This function returns a value between 0 and MAX_SEC_PER_DAY representing the second of the day, given the hour, minute, and second.
	///
	///	\tparam T1 The return type of the function (default is int).
	///	\tparam T2 The type of the hour, minute, and second parameters (default is int).
	///	\param hour Hour of the day.
	///	\param min Minute of the hour.
	///	\param sec Second of the minute.
	///	\return Second of the day.
	template<class T1 = int, class T2 = int>
	constexpr const T1 sec_of_day(
			const T2& hour,
			const T2& min,
			const T2& sec) noexcept {
		return hour * SEC_PER_HOUR + min * SEC_PER_MIN + sec;
	}

	///	\brief Get the second of the minute.
	///
	///	This function returns a value between 0 and 59 representing the second of the minute.
	///
	///	\tparam T The return type of the function (default is int).
	///	\param ts Timestamp in seconds (default is current timestamp).
	///	\return Second of the minute.
	template<class T = int>
	constexpr const T sec_of_min(const ts_t& ts = ts()) {
		return (ts % SEC_PER_MIN);
	}

	///	\brief Get the second of the hour.
	///
	///	This function returns a value between 0 and 3599 representing the second of the hour.
	///
	///	\tparam T The return type of the function (default is int).
	///	\param ts Timestamp in seconds (default is current timestamp).
	///	\return Second of the hour.
	template<class T = int>
	constexpr const T sec_of_hour(const ts_t& ts = ts()) {
		return (ts % SEC_PER_HOUR);
	}

//------------------------------------------------------------------------------

	///	\brief Get the year from the timestamp.
	///
	///	This function returns the year of the specified timestamp in seconds since the Unix epoch.
	///
	///	\tparam T The return type of the function (default is year_t).
	///	\param ts Timestamp in seconds (default is current timestamp).
	///	\return Year of the specified timestamp.
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const T get_year(const ts_t& ts = ts()) {
		return get_unix_year(ts) + UNIX_EPOCH;
	}

	///	\brief Alias for get_year function.
	/// \copydoc get_year
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const T year(const ts_t& ts = ts()) {
		return get_year(ts);
	}

	///	\brief Alias for get_year function.
	/// \copydoc get_year
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const T to_year(const ts_t& ts = ts()) {
		return get_year(ts);
	}

//------------------------------------------------------------------------------

	///	\brief Get the year from the timestamp in milliseconds.
	///
	///	This function returns the year of the specified timestamp in milliseconds since the Unix epoch.
	///
	///	\tparam T The return type of the function (default is year_t).
	///	\param t_ms Timestamp in milliseconds (default is current timestamp).
	///	\return Year of the specified timestamp.
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const T get_year_ms(const ts_ms_t& t_ms = ts_ms()) {
		return get_year(ms_to_sec(t_ms));
	}

	///	\brief Alias for get_year_ms function.
	/// \copydoc get_year_ms
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const T year_ms(const ts_t& ts = ts()) {
		return get_year_ms(ts);
	}

	///	\brief Alias for get_year_ms function.
	/// \copydoc get_year_ms
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const T to_year_ms(const ts_t& ts = ts()) {
		return get_year_ms(ts);
	}

//------------------------------------------------------------------------------

	///	\brief Get the start of the year timestamp.
	///
	///	This function resets the days, months, hours, minutes, and seconds of the given timestamp
	///	to the beginning of the year.
	///
	///	\param ts Timestamp.
	///	\return Start of the year timestamp.
	TIME_SHIELD_CONSTEXPR inline const ts_t start_of_year(const ts_t& ts) noexcept {
		const ts_t BIAS_2100 = 4102444800;
		if (ts < BIAS_2100) {
			const ts_t SEC_PER_YEAR_X2 = SEC_PER_YEAR * 2;
			ts_t year_start_ts = ts % SEC_PER_4_YEARS;
			if (year_start_ts < SEC_PER_YEAR) {
				return ts - year_start_ts;
			} else
			if (year_start_ts < SEC_PER_YEAR_X2) {
				return ts + SEC_PER_YEAR - year_start_ts;
			} else
			if (year_start_ts < (SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR)) {
				return ts + SEC_PER_YEAR_X2 - year_start_ts;
			}
			return ts + (SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR) - year_start_ts;
		}

		const ts_t BIAS_2000 = 946684800;
		ts_t secs = ts - BIAS_2000;

		ts_t offset_y400 = secs % SEC_PER_400_YEARS;
		ts_t start_ts = secs - offset_y400 + BIAS_2000;
		secs = offset_y400;

		if (secs >= SEC_PER_FIRST_100_YEARS) {
			secs -= SEC_PER_FIRST_100_YEARS;
			start_ts += SEC_PER_FIRST_100_YEARS;
			while (secs >= SEC_PER_100_YEARS) {
				secs -= SEC_PER_100_YEARS;
				start_ts += SEC_PER_100_YEARS;
			}

			const ts_t SEC_PER_4_YEARS_V2 = 4 * SEC_PER_YEAR;
			if (secs >= SEC_PER_4_YEARS_V2) {
				secs -= SEC_PER_4_YEARS_V2;
				start_ts += SEC_PER_4_YEARS_V2;
			} else {
				start_ts += secs - secs % SEC_PER_YEAR;
				return start_ts;
			}
		}

		ts_t offset_4y = secs % SEC_PER_4_YEARS;
		start_ts += secs - offset_4y;
		secs = offset_4y;

		if (secs >= SEC_PER_LEAP_YEAR) {
			secs -= SEC_PER_LEAP_YEAR;
			start_ts += SEC_PER_LEAP_YEAR;
			start_ts += secs - secs % SEC_PER_YEAR;
		}
		return start_ts;
	}

	///	\brief Alias for start_of_year function.
	/// \copydoc start_of_year
	TIME_SHIELD_CONSTEXPR inline const ts_t year_start(const ts_t& ts = ts()) {
		return start_of_year(ts);
	}

	///	\brief Alias for start_of_year function.
	/// \copydoc start_of_year
	TIME_SHIELD_CONSTEXPR inline const ts_t year_begin(const ts_t& ts = ts()) {
		return start_of_year(ts);
	}

//------------------------------------------------------------------------------

	///	\brief Get the start of the year timestamp in milliseconds.
	///
	///	This function resets the days, months, hours, minutes, and seconds of the given timestamp
	///	to the beginning of the year.
	///
	///	\param ts_ms Timestamp in milliseconds.
	///	\return Start of year timestamp in milliseconds.
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t start_of_year_ms(const ts_ms_t& ts_ms) noexcept {
		return sec_to_ms(start_of_year(ms_to_sec(ts_ms)));
	}

	///	\brief Alias for start_of_year_ms function.
	/// \copydoc start_of_year_ms
	TIME_SHIELD_CONSTEXPR inline const ts_t year_start_ms(const ts_t& ts = ts()) {
		return start_of_year_ms(ts);
	}

	///	\brief Alias for start_of_year_ms function.
	/// \copydoc start_of_year_ms
	TIME_SHIELD_CONSTEXPR inline const ts_t year_begin_ms(const ts_t& ts = ts()) {
		return start_of_year_ms(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the start of the year.
	///
	/// This function returns the timestamp at the start of the specified year.
	///
	/// \param year Year.
	/// \return Timestamp of the start of the year.
	///	\throws std::invalid_argument if the date-time combination is invalid.
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const ts_t start_of_year_date(const T& year) {
		if (year < 2100) {
			const ts_t year_diff = year >= UNIX_EPOCH ? year - UNIX_EPOCH : UNIX_EPOCH - year;
			const ts_t year_start_ts = (year_diff / 4) * SEC_PER_4_YEARS;
			const ts_t year_remainder = year_diff % 4;
			const ts_t SEC_PER_YEAR_X2 = 2 * SEC_PER_YEAR;
			const ts_t SEC_PER_YEAR_V2 = SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR;
			switch (year_remainder) {
				case 0: return year_start_ts;
				case 1: return year_start_ts + SEC_PER_YEAR;
				case 2: return year_start_ts + SEC_PER_YEAR_X2;
				default: return year_start_ts + SEC_PER_YEAR_V2;
			};
			return year_start_ts + SEC_PER_YEAR_V2;
		}
		return to_timestamp(year, 1, 1);
	}

	///	\brief Alias for start_of_year_date function.
	/// \copydoc start_of_year_date
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const ts_t year_start_date(const T& year) {
		return start_of_year_date(year);
	}

	///	\brief Alias for start_of_year_date function.
	/// \copydoc start_of_year_date
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const ts_t year_begin_date(const T& year) {
		return start_of_year_date(year);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp in milliseconds of the start of the year.
	///
	/// This function returns the timestamp at the start of the specified year in milliseconds.
	///
	/// \param year Year.
	/// \return Timestamp of the start of the year in milliseconds.
	///	\throws std::invalid_argument if the date-time combination is invalid.
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t start_of_year_date_ms(const T& year) {
		return sec_to_ms(start_of_year_date(year));
	}

	///	\brief Alias for start_of_year_date_ms function.
	/// \copydoc start_of_year_date_ms
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t year_start_date_ms(const T& year) {
		return start_of_year_date_ms(year);
	}

	///	\brief Alias for start_of_year_date_ms function.
	/// \copydoc start_of_year_date_ms
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t year_begin_date_ms(const T& year) {
		return start_of_year_date_ms(year);
	}

//------------------------------------------------------------------------------

	///	\brief Get the end-of-year timestamp.
	///
	///	This function finds the last timestamp of the current year.
	///
	///	\param ts Timestamp.
	///	\return End-of-year timestamp.
	TIME_SHIELD_CONSTEXPR inline ts_t end_of_year(const ts_t& ts = ts()) {
		const ts_t BIAS_2100 = 4102444800;
		if (ts < BIAS_2100) {
			const ts_t SEC_PER_YEAR_X2 = SEC_PER_YEAR * 2;
			const ts_t SEC_PER_YEAR_X3 = SEC_PER_YEAR * 3;
			const ts_t SEC_PER_YEAR_X3_V2 = SEC_PER_YEAR_X2 + SEC_PER_LEAP_YEAR;
			ts_t year_end_ts = ts % SEC_PER_4_YEARS;
			if (year_end_ts < SEC_PER_YEAR) {
				return ts + SEC_PER_YEAR - year_end_ts - 1;
			} else
			if (year_end_ts < SEC_PER_YEAR_X2) {
				return ts + SEC_PER_YEAR_X2 - year_end_ts - 1;
			} else
			if (year_end_ts < SEC_PER_YEAR_X3_V2) {
				return ts + SEC_PER_YEAR_X3_V2 - year_end_ts - 1;
			}
			return ts + (SEC_PER_YEAR_X3 + SEC_PER_LEAP_YEAR) - year_end_ts - 1;
		}

		const ts_t BIAS_2000 = 946684800;
		ts_t secs = ts - BIAS_2000;

		ts_t offset_y400 = secs % SEC_PER_400_YEARS;
		ts_t end_ts = secs - offset_y400 + BIAS_2000;
		secs = offset_y400;

		if (secs >= SEC_PER_FIRST_100_YEARS) {
			secs -= SEC_PER_FIRST_100_YEARS;
			end_ts += SEC_PER_FIRST_100_YEARS;
			while (secs >= SEC_PER_100_YEARS) {
				secs -= SEC_PER_100_YEARS;
				end_ts += SEC_PER_100_YEARS;
			}

			const ts_t SEC_PER_4_YEARS_V2 = 4 * SEC_PER_YEAR;
			if (secs >= SEC_PER_4_YEARS_V2) {
				secs -= SEC_PER_4_YEARS_V2;
				end_ts += SEC_PER_4_YEARS_V2;
			} else {
				end_ts += secs - secs % SEC_PER_YEAR;
				return end_ts + SEC_PER_YEAR - 1;
			}
		}

		ts_t offset_4y = secs % SEC_PER_4_YEARS;
		end_ts += secs - offset_4y;
		secs = offset_4y;

		if (secs >= SEC_PER_LEAP_YEAR) {
			secs -= SEC_PER_LEAP_YEAR;
			end_ts += SEC_PER_LEAP_YEAR;
			end_ts += secs - secs % SEC_PER_YEAR;
			end_ts += SEC_PER_YEAR;
		} else {
			end_ts += SEC_PER_LEAP_YEAR;
		}
		return end_ts - 1;
	}

	///	\brief Alias for end_of_year function.
	/// \copydoc end_of_year
	TIME_SHIELD_CONSTEXPR inline const ts_t year_end(const ts_t& ts = ts()) {
		return end_of_year(ts);
	}

//------------------------------------------------------------------------------

	///	\brief Get the timestamp in milliseconds of the end of the year.
	///
	///	This function finds the last timestamp of the current year in milliseconds.
	///
	///	\param ts_ms Timestamp in milliseconds.
	///	\return End-of-year timestamp in milliseconds.
	template<class T = year_t>
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t end_of_year_ms(const ts_ms_t& ts_ms = ts_ms()) {
		return sec_to_ms(end_of_year(ms_to_sec(ts_ms)));
	}

	///	\brief Alias for end_of_year_ms function.
	/// \copydoc end_of_year_ms
	TIME_SHIELD_CONSTEXPR inline const ts_ms_t year_end_ms(const ts_ms_t& ts_ms = ts_ms()) {
		return end_of_year_ms(ts_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the day of the year.
	///
	/// This function returns the day of the year for the specified timestamp.
	///
	/// \param ts Timestamp.
	/// \return Day of the year.
	template<class T = int>
	inline const T day_of_year(const ts_t& ts = ts()) {
		return ((ts - start_of_year(ts)) / SEC_PER_DAY) + 1;
	}

//------------------------------------------------------------------------------

	/// \brief Get the month of the year.
	///
	/// This function returns the month of the year for the specified timestamp.
	///
	/// \param ts Timestamp.
	/// \return Month of the year.
	template<class T = Month>
	TIME_SHIELD_CONSTEXPR inline const T month_of_year(const ts_t& ts) noexcept {
		const int JAN_AND_FEB_DAY_LEAP_YEAR = 60;
		const int TABLE_MONTH_OF_YEAR[] = {
			0,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 31 январь
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,		// 28 февраль
			3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,	// 31 март
			4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,	// 30 апрель
			5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
			6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
			7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
			8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
			9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
			10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
			11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
			12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
		};
		const size_t dy = day_of_year(ts);
		return static_cast<T>((is_leap_year(ts) && dy >= JAN_AND_FEB_DAY_LEAP_YEAR) ? TABLE_MONTH_OF_YEAR[dy - 1] : TABLE_MONTH_OF_YEAR[dy]);
	}

//------------------------------------------------------------------------------

	/// \brief Get the day of the month.
	///
	/// This function returns the day of the month for the specified timestamp.
	///
	/// \param ts Timestamp.
	/// \return Day of the month.
	template<class T = int>
	TIME_SHIELD_CONSTEXPR inline const T day_of_month(const ts_t& ts) {
		const int JAN_AND_FEB_DAY_LEAP_YEAR = 60;
		// таблица для обычного года, не високосного
		const int TABLE_DAY_OF_YEAR[] = {
			0,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	// 31 январь
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,				// 28 февраль
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	// 31 март
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,		// 30 апрель
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
		};
		const size_t dy = day_of_year(ts);
		if(is_leap_year(ts)) {
			if(dy == JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1] + 1;
			if(dy > JAN_AND_FEB_DAY_LEAP_YEAR) return TABLE_DAY_OF_YEAR[dy - 1];
		}
		return TABLE_DAY_OF_YEAR[dy];
	}

//------------------------------------------------------------------------------

	/// \brief Get the number of days in a month.
	///
	/// This function calculates and returns the number of days in the specified month and year.
	///
	/// \param year Year as an integer.
	/// \param month Month as an integer.
	/// \return The number of days in the given month and year.
	template<class T1 = int, class T2 = year_t, class T3 = int>
	constexpr const T1 num_days_in_month(const T2& year, const T3& month) noexcept {
		if (month > MONTHS_PER_YEAR || month < 0) return 0;
		const T1 num_days[13] = {0,31,30,31,30,31,30,31,31,30,31,30,31};
		if (month == FEB) {
			if(is_leap_year_date(year)) return 29;
			return 28;
		}
		return num_days[month];
	}

	///	\brief Alias for num_days_in_month function.
	/// \copydoc num_days_in_month
	template<class T1 = int, class T2 = year_t, class T3 = int>
	constexpr const T1 days_in_month(const T2& year, const T3& month) noexcept {
		return num_days_in_month(year, month);
	}

//------------------------------------------------------------------------------

	///	\brief Get the number of days in the month of the given timestamp.
	///
	///	This function calculates and returns the number of days in the month of the specified timestamp.
	///
	///	\param ts The timestamp to extract month and year from.
	///	\return The number of days in the month of the given timestamp.
	template<class T1 = int>
	TIME_SHIELD_CONSTEXPR const T1 num_days_in_month_ts(const ts_t& ts = ts()) noexcept {
		const T1 num_days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
		const int month = month_of_year(ts);
		if (month == FEB) {
			return is_leap_year(ts) ? 29 : 28;
		}
		return num_days[month];
	}

	///	\brief Alias for num_days_in_month_ts function.
	/// \copydoc num_days_in_month_ts
	template<class T1 = int>
	constexpr const T1 num_days_in_month(const ts_t& ts = ts()) noexcept {
		return num_days_in_month_ts(ts);
	}

	///	\brief Alias for num_days_in_month_ts function.
	/// \copydoc num_days_in_month_ts
	template<class T1 = int>
	constexpr const T1 days_in_month(const ts_t& ts = ts()) noexcept {
		return num_days_in_month_ts(ts);
	}

//------------------------------------------------------------------------------

	///	\brief Get the number of days in a given year.
	///
	/// This function calculates and returns the number of days in the specified year.
	///
	///	\param year Year.
	///	\return Number of days in the given year.
	template<class T1 = int, class T2 = year_t>
	constexpr const T1 num_days_in_year(const T2& year) noexcept {
		if (is_leap_year_date(year)) return DAYS_PER_LEAP_YEAR;
		return DAYS_PER_YEAR;
	}

	///	\brief Alias for num_days_in_year function.
	/// \copydoc num_days_in_year
	template<class T1 = int, class T2 = year_t>
	constexpr const T1 days_in_year(const T2& year) noexcept {
		return num_days_in_year(year);
	}

//------------------------------------------------------------------------------

	///	\brief Get the number of days in the current year.
	///
	/// This function calculates and returns the number of days in the current year based on the provided timestamp.
	///
	///	\param ts Timestamp.
	///	\return Number of days in the current year.
	template<class T = int>
	constexpr const T num_days_in_year_ts(const ts_t& ts = ts()) {
		if (is_leap_year_ts(ts)) return DAYS_PER_LEAP_YEAR;
		return DAYS_PER_YEAR;
	}

	///	\brief Alias for num_days_in_year_ts function.
	/// \copydoc num_days_in_year_ts
	template<class T = int>
	constexpr const T days_in_year_ts(const ts_t& t = ts()) {
		return num_days_in_year_ts(t);
	}

//------------------------------------------------------------------------------

	/// \brief Get the start of the day timestamp.
	///
	/// This function returns the timestamp at the start of the day.
	/// The function sets the hours, minutes, and seconds to zero.
	///
	/// \param ts Timestamp.
	/// \return Start of the day timestamp.
	constexpr const ts_t start_of_day(const ts_t& ts = ts()) noexcept {
		return ts - (ts % SEC_PER_DAY);
	}

	///	\brief Alias for start_of_day function.
	/// \copydoc start_of_day
	constexpr const ts_t day_start(const ts_t& ts = ts()) noexcept {
		return start_of_day(ts);
	}

//------------------------------------------------------------------------------

	///	\brief Get timestamp of the start of the previous day.
	///
	///	This function returns the timestamp at the start of the previous day.
	///
	///	\param ts Timestamp of the current day.
	///	\param days Number of days to go back (default is 1).
	///	\return Timestamp of the start of the previous day.
	template<class T = int>
	constexpr const ts_t start_of_prev_day(const ts_t& ts = ts(), const T& days = 1) {
		return ts - (ts % SEC_PER_DAY) - SEC_PER_DAY * days;
	}

	///	\brief Alias for start_of_prev_day function.
	/// \copydoc start_of_prev_day
	template<class T = int>
	constexpr const ts_t previous_day_start(const ts_t& ts = ts(), const T& days = 1) {
		return start_of_prev_day(ts, days);
	}

//------------------------------------------------------------------------------

	/// \brief Get the start of the day timestamp in seconds.
	///
	/// This function returns the timestamp at the start of the day in seconds.
	/// The function sets the hours, minutes, and seconds to zero.
	///
	/// \param ts_ms Timestamp in milliseconds.
	/// \return Start of the day timestamp in seconds.
	constexpr const ts_t start_of_day_sec(const ts_ms_t& ts_ms = ts_ms()) {
		return start_of_day(ms_to_sec(ts_ms));
	}

	///	\brief Alias for start_of_day_sec function.
	/// \copydoc start_of_day_sec
	constexpr const ts_t day_start_sec(const ts_ms_t& t_ms = ts_ms()) {
		return start_of_day_sec(t_ms);
	}

//------------------------------------------------------------------------------

	///	\brief Get the start of the day timestamp in milliseconds.
	///
	///	This function returns the timestamp at the start of the day in milliseconds.
	///	The function sets the hours, minutes, seconds, and milliseconds to zero.
	///
	///	\param ts_ms Timestamp in milliseconds.
	///	\return Start of the day timestamp in milliseconds.
	constexpr const ts_ms_t start_of_day_ms(const ts_ms_t& ts_ms = ts_ms()) {
		return ts_ms - (ts_ms % MS_PER_DAY);
	}

	///	\brief Alias for start_of_day_ms function.
	///	\copydoc start_of_day_ms
	constexpr const ts_ms_t day_start_ms(const ts_ms_t& t_ms = ts_ms()) {
		return start_of_day_ms(t_ms);
	}

//------------------------------------------------------------------------------

	///	\brief Get timestamp of the first day after a specified number of days.
	///
	///	This function returns the timestamp of the start of the day after a specified number of days.
	///
	///	\param ts Current timestamp.
	///	\param days Number of days after the current day (default is 1).
	///	\return Timestamp of the first day after the specified number of days.
	template<class T = int>
	constexpr const ts_t start_of_next_day(const ts_t& ts, const T& days = 1) {
		return start_of_day(ts) + days * SEC_PER_DAY;
	}

	///	\brief Alias for start_of_next_day function.
	///	\copydoc start_of_next_day
	template<class T = int>
	constexpr const ts_t next_day_start(const ts_t& ts, const T& days = 1) {
		return start_of_next_day(ts, days);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the end of the day.
	///
	/// This function sets the hour to 23, minute to 59, and second to 59.
	///
	/// \param ts Timestamp.
	/// \return Timestamp at the end of the day.
	constexpr const ts_t end_of_day(const ts_t& ts = ts()) noexcept {
		return ts - (ts % SEC_PER_DAY) + SEC_PER_DAY - 1;
	}

	/// \brief Alias for end_of_day function.
	/// \copydoc end_of_day
	constexpr const ts_t day_end(const ts_t& ts = ts()) noexcept {
		return end_of_day(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the end of the day in seconds.
	///
	/// This function sets the hour to 23, minute to 59, and second to 59.
	///
	/// \param ts_ms Timestamp in milliseconds.
	/// \return Timestamp at the end of the day in seconds.
	constexpr const ts_t end_of_day_sec(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return end_of_day(ms_to_sec(ts_ms));
	}

	/// \brief Alias for end_of_day_sec function.
	/// \copydoc end_of_day_sec
	constexpr const ts_t day_end_sec(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return end_of_day_sec(ts_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the end of the day in milliseconds.
	///
	/// This function sets the hour to 23, minute to 59, second to 59, and millisecond to 999.
	///
	/// \param ts_ms Timestamp in milliseconds.
	/// \return Timestamp at the end of the day in milliseconds.
	constexpr const ts_ms_t end_of_day_ms(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return ts_ms - (ts_ms % MS_PER_DAY) + MS_PER_DAY - 1;
	}

	/// \brief Alias for end_of_day_ms function.
	/// \copydoc end_of_day_ms
	constexpr const ts_ms_t day_end_ms(const ts_ms_t& t_ms = ts_ms()) noexcept {
		return end_of_day_ms(t_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the day of the week.
	/// \param year Year.
	/// \param month Month of the year.
	/// \param day Day of the month.
	/// \return Day of the week (SUN = 0, MON = 1, ... SAT = 6).
	template<class T1 = Weekday, class T2 = year_t, class T3 = int>
	constexpr const T1 day_of_week_date(const T2& year, const T3& month, const T3& day) {
		year_t a, y, m, R;
		a = (14 - month) / MONTHS_PER_YEAR;
		y = year - a;
		m = month + MONTHS_PER_YEAR * a - 2;
		R = 7000 + ( day + y + (y / 4) - (y / 100) + (y / 400) + (31 * m) / MONTHS_PER_YEAR);
		return static_cast<T1>(R % DAYS_PER_WEEK);
	}

	/// \brief Alias for day_of_week_date function.
	/// \copydoc day_of_week_date
	template<class T1 = Weekday, class T2 = year_t, class T3 = int>
	constexpr const T1 get_weekday(const T2& year, const T3& month, const T3& day) {
		return day_of_week_date(year, month, day);
	}

	/// \brief Alias for day_of_week_date function.
	/// \copydoc day_of_week_date
	template<class T1 = Weekday, class T2 = year_t, class T3 = int>
	constexpr const T1 day_of_week(const T2& year, const T3& month, const T3& day) {
		return day_of_week_date(year, month, day);
	}

//------------------------------------------------------------------------------

	/// \brief Get the day of the week from a date structure.
	///
	/// This function takes a date structure with fields 'year', 'mon', and 'day',
	/// and returns the day of the week (SUN = 0, MON = 1, ... SAT = 6).
	///
	/// \param date Structure containing year, month, and day.
	/// \return Day of the week (SUN = 0, MON = 1, ... SAT = 6).
	template<class T1 = Weekday, class T2>
	constexpr const T1 get_weekday_from_date(const T2& date) {
		return day_of_week_date(date.year, date.mon, date.day);
	}

	/// \brief Alias for get_weekday_from_date function that accepts a date structure.
	/// \copydoc get_weekday_from_date
	template<class T1 = int, class T2>
	constexpr const T1 day_of_week_dt(const T2& date) {
		return get_weekday_from_date(date);
	}

	/// \brief Alias for get_weekday_from_date function that accepts a date structure.
	/// \copydoc get_weekday_from_date
	template<class T1 = int, class T2>
	constexpr const T1 day_of_week(const T2& date) {
		return get_weekday_from_date(date);
	}

//------------------------------------------------------------------------------

	/// \brief Get the weekday from a timestamp.
	/// \param ts Timestamp.
	/// \return Weekday (SUN = 0, MON = 1, ... SAT = 6).
	template<class T = Weekday>
	constexpr const T get_weekday_from_ts(const ts_t& ts) noexcept {
		return static_cast<T>((ts / SEC_PER_DAY + THU) % DAYS_PER_WEEK);
	}

	/// \brief Alias for get_weekday_from_ts function.
	/// \copydoc get_weekday_from_ts
	template<class T = Weekday>
	constexpr const T day_of_week(const ts_t& ts) noexcept {
		return get_weekday_from_ts(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the weekday from a timestamp in milliseconds.
	/// \param ts_ms Timestamp in milliseconds.
	/// \return Weekday (SUN = 0, MON = 1, ... SAT = 6).
	template<class T = Weekday>
	constexpr const T get_weekday_from_ts_ms(const ts_ms_t& ts_ms) {
		return get_weekday_from_ts(ms_to_sec(ts_ms));
	}

	/// \brief Alias for get_weekday_from_ts_ms function.
	/// \copydoc get_weekday_from_ts_ms
	template<class T = Weekday>
	constexpr const T day_of_week_ms(const ts_ms_t& ts_ms) {
		return get_weekday_from_ts_ms(ts_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the start of the current month.
	///
	/// This function returns the timestamp at the start of the current month,
	/// setting the day to the first day of the month and the time to 00:00:00.
	///
	/// \param ts Timestamp (default is current timestamp)
	/// \return Timestamp at the start of the current month
	TIME_SHIELD_CONSTEXPR inline const ts_t start_of_month(const ts_t& ts = ts()) {
		return start_of_day(ts) - (day_of_month(ts) - 1) * SEC_PER_DAY;
	}

	/// \brief Alias for start_of_month function.
	/// \copydoc start_of_month
	TIME_SHIELD_CONSTEXPR inline const ts_t month_begin(const ts_t& ts = ts()) {
		return start_of_month(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the last timestamp of the current month.
	///
	/// This function returns the last timestamp of the current month,
	/// setting the day to the last day of the month and the time to 23:59:59.
	///
	/// \param ts Timestamp (default is current timestamp)
	/// \return Last timestamp of the current month
	TIME_SHIELD_CONSTEXPR inline const ts_t end_of_month(const ts_t& ts = ts()) {
		return end_of_day(ts) + (num_days_in_month(ts) - day_of_month(ts)) * SEC_PER_DAY;
	}

	/// \brief Alias for end_of_month function.
	/// \copydoc end_of_month
	TIME_SHIELD_CONSTEXPR inline const ts_t last_day_of_month(const ts_t& ts = ts()) {
		return end_of_month(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the last Sunday of the current month.
	///
	/// This function returns the timestamp of the last Sunday of the current month,
	/// setting the day to the last Sunday and the time to 00:00:00.
	///
	/// \param ts Timestamp (default is current timestamp)
	/// \return Timestamp of the last Sunday of the current month
	TIME_SHIELD_CONSTEXPR inline const ts_t last_sunday_of_month(const ts_t& ts = ts()) {
		return end_of_month(ts) - day_of_week(ts) * SEC_PER_DAY;
	}

	/// \brief Alias for last_sunday_of_month function.
	/// \copydoc last_sunday_of_month
	TIME_SHIELD_CONSTEXPR inline const ts_t final_sunday_of_month(const ts_t& ts = ts()) {
		return last_sunday_of_month(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the day of the last Sunday of the given month and year.
	///
	/// This function returns the day of the last Sunday of the specified month and year.
	///
	/// \param year Year
	/// \param month Month (1 = January, 12 = December)
	/// \return Day of the last Sunday of the given month and year
	template<class T1 = int, class T2 = year_t, class T3 = int>
	TIME_SHIELD_CONSTEXPR inline const T1 last_sunday_month_day(const T2& year, const T3& month) {
		const T1 days = num_days_in_month(year, month);
		return days - day_of_week_date(year, month, days);
	}

	/// \brief Alias for last_sunday_month_day function.
	/// \copydoc last_sunday_month_day
	template<class T1 = int, class T2 = year_t, class T3 = int>
	TIME_SHIELD_CONSTEXPR inline const T1 final_sunday_month_day(const T2& year, const T3& month) {
		return last_sunday_month_day(year, month);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the start of the hour.
	///
	/// This function sets the minute and second to zero.
	///
	/// \param ts Timestamp (default: current timestamp).
	/// \return Timestamp at the start of the hour.
	constexpr const ts_t start_of_hour(const ts_t& ts = ts()) noexcept {
		return ts - (ts % SEC_PER_HOUR);
	}

	/// \brief Alias for start_of_hour function.
	/// \copydoc start_of_hour
	constexpr const ts_t hour_begin(const ts_t& ts = ts()) noexcept {
		return start_of_hour(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the start of the hour.
	///
	/// This function sets the minute and second to zero.
	///
	/// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
	/// \return Timestamp at the start of the hour in seconds.
	constexpr const ts_t start_of_hour_sec(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return start_of_hour(ms_to_sec(ts_ms));
	}

	/// \brief Alias for start_of_hour_sec function.
	/// \copydoc start_of_hour_sec
	constexpr const ts_t hour_begin_sec(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return start_of_hour_sec(ts_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the start of the hour.
	/// This function sets the minute and second to zero.
	/// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
	/// \return Timestamp at the start of the hour in milliseconds.
	constexpr const ts_ms_t start_of_hour_ms(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return ts_ms - (ts_ms % MS_PER_HOUR);
	}

	/// \brief Alias for start_of_hour_ms function.
	/// \copydoc start_of_hour_ms
	constexpr const ts_ms_t hour_begin_ms(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return start_of_hour_ms(ts_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the end of the hour.
	/// This function sets the minute and second to 59.
	/// \param ts Timestamp (default: current timestamp).
	/// \return Timestamp at the end of the hour.
	constexpr const ts_t end_of_hour(const ts_t& ts = ts()) noexcept {
		return ts - (ts % SEC_PER_HOUR) + SEC_PER_HOUR - 1;
	}

	/// \brief Alias for end_of_hour function.
	/// \copydoc end_of_hour
	constexpr const ts_t finish_of_hour(const ts_t& ts = ts()) noexcept {
		return end_of_hour(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the end of the hour.
	///
	/// This function sets the minute and second to 59.
	///
	/// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
	/// \return Timestamp at the end of the hour in seconds.
	constexpr const ts_t end_of_hour_sec(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return end_of_hour(ms_to_sec(ts_ms));
	}

	/// \brief Alias for end_of_hour_sec function.
	/// \copydoc end_of_hour_sec
	constexpr const ts_t finish_of_hour_sec(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return end_of_hour_sec(ts_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp at the end of the hour.
	///
	/// This function sets the minute and second to 59.
	///
	/// \param ts_ms Timestamp in milliseconds (default: current timestamp in milliseconds).
	/// \return Timestamp at the end of the hour in milliseconds.
	constexpr const ts_ms_t end_of_hour_ms(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return ts_ms - (ts_ms % MS_PER_HOUR) + MS_PER_HOUR - 1;
	}

	/// \brief Alias for end_of_hour_ms function.
	/// \copydoc end_of_hour_ms
	constexpr const ts_ms_t finish_of_hour_ms(const ts_ms_t& ts_ms = ts_ms()) noexcept {
		return end_of_hour_ms(ts_ms);
	}

//------------------------------------------------------------------------------

	/// \brief Get the hour of the day.
	///
	/// This function returns a value between 0 to 23 representing the hour of the day.
	///
	/// \param ts Timestamp (default: current timestamp).
	/// \return Hour of the day.
	template<class T = int>
	constexpr const T hour_of_day(const ts_t& ts = ts()) noexcept {
		return ((ts / SEC_PER_HOUR) % HOURS_PER_DAY);
	}

	/// \brief Alias for hour_of_day function.
	/// \copydoc hour_of_day
	template<class T = int>
	constexpr const T hour_in_day(const ts_t& ts = ts()) noexcept {
		return hour_of_day(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the beginning of the week.
	///
	/// This function finds the timestamp of the beginning of the week,
	/// which corresponds to the start of Sunday.
	///
	/// \param ts Timestamp (default: current timestamp).
	/// \return Returns the timestamp of the beginning of the week.
	constexpr const ts_t start_of_week(const ts_t& ts = ts()) {
		return start_of_day(ts) - day_of_week(ts) * SEC_PER_DAY;
	}

	/// \brief Alias for start_of_week function.
	/// \copydoc start_of_week
	constexpr const ts_t week_begin(const ts_t& t = ts()) {
		return start_of_week(t);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the end of the week.
	///
	/// This function finds the timestamp of the end of the week,
	/// which corresponds to the end of Saturday.
	///
	/// \param ts Timestamp (default: current timestamp).
	/// \return Returns the timestamp of the end of the week.
	constexpr const ts_t end_of_week(const ts_t& ts = ts()) {
		return start_of_day(ts) + (DAYS_PER_WEEK - day_of_week(ts)) * SEC_PER_DAY - 1;
	}

	/// \brief Alias for end_of_week function.
	/// \copydoc end_of_week
	constexpr const ts_t finish_of_week(const ts_t& ts = ts()) {
		return end_of_week(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the start of Saturday.
	///
	/// This function finds the timestamp of the beginning of the day on Saturday,
	/// which corresponds to the start of Saturday.
	///
	/// \param ts Timestamp (default: current timestamp).
	/// \return Returns the timestamp of the start of Saturday.
	constexpr const ts_t start_of_saturday(const ts_t& ts = ts()) {
		return start_of_day(ts) + (SAT - day_of_week(ts)) * SEC_PER_DAY;
	}

	/// \brief Alias for start_of_saturday function.
	/// \copydoc start_of_saturday
	constexpr const ts_t saturday_begin(const ts_t& ts = ts()) {
		return start_of_saturday(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the beginning of the minute.
	/// \param ts Timestamp (default: current timestamp).
	/// \return Returns the timestamp of the beginning of the minute.
	constexpr const ts_t start_of_min(const ts_t& ts = ts()) noexcept {
		return ts - (ts % SEC_PER_MIN);
	}

	/// \brief Alias for start_of_min function.
	/// \copydoc start_of_min
	constexpr const ts_t min_begin(const ts_t& ts = ts()) noexcept {
		return start_of_min(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the end of the minute.
	/// \param ts Timestamp (default: current timestamp).
	/// \return Returns the timestamp of the end of the minute.
	constexpr const ts_t end_of_min(const ts_t& ts = ts()) noexcept {
		return ts - (ts % SEC_PER_MIN) + SEC_PER_MIN - 1;
	}

	/// \brief Alias for end_of_min function.
	/// \copydoc end_of_min
	constexpr const ts_t finish_of_min(const ts_t& ts = ts()) noexcept {
		return end_of_min(ts);
	}

//------------------------------------------------------------------------------

	/// \brief Get minute of day.
	/// This function returns a value between 0 to 1439 (minute of day).
	/// \param ts Timestamp in seconds (default: current timestamp).
	/// \return Minute of day.
	template<class T = int>
	constexpr const T min_of_day(const ts_t& ts = ts()) noexcept {
		return ((ts / SEC_PER_MIN) % MIN_PER_DAY);
	}

//------------------------------------------------------------------------------

	/// \brief Get minute of hour.
	/// This function returns a value between 0 to 59.
	/// \param ts Timestamp in seconds (default: current timestamp).
	/// \return Minute of hour.
	template<class T = int>
	constexpr const T min_of_hour(const ts_t& ts = ts()) noexcept {
		return ((ts / SEC_PER_MIN) % MIN_PER_HOUR);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the start of the period.
	/// \param p Period duration in seconds.
	/// \param ts Timestamp (default: current timestamp).
	/// \return Returns the timestamp of the start of the period.
	template<class T = int>
	constexpr const ts_t start_of_period(const T& p, const ts_t& ts = ts()) {
		return ts - (ts % p);
	}

//------------------------------------------------------------------------------

	/// \brief Get the timestamp of the end of the period.
	/// \param p Period duration in seconds.
	/// \param ts Timestamp (default: current timestamp).
	/// \return Returns the timestamp of the end of the period.
	template<class T = int>
	constexpr const ts_t end_of_period(const T& p, const ts_t& ts = ts()) {
		return ts - (ts % p) + p - 1;
	}

//------------------------------------------------------------------------------

	/// \brief Converts an integer to a time zone structure.
	/// \tparam T The type of the time zone structure (default is TimeZoneStruct).
	/// \param offset The integer to convert.
	/// \return A time zone structure of type T represented by the given integer.
	/// \details The function assumes that the type T has members `hour`, `min`, and `is_positive`.
	template<class T = TimeZoneStruct>
	inline const TimeZoneStruct to_time_zone(
			const tz_t& offset) {
		T tz;
		int abs_val = std::abs(offset);
		tz.hour = abs_val / SEC_PER_HOUR;
		tz.min = abs_val % SEC_PER_MIN;
		tz.is_positive = (offset >= 0);
		return tz;
	}

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_CONVERSIONS_HPP_INCLUDED
