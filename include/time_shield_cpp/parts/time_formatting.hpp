#pragma once
/// \file time_formatting.hpp
/// \brief Header file for time formatting utilities.
///
/// This file contains functions for converting timestamps to formatted strings.
/// It provides utilities for custom formatting based on user-defined patterns
/// and for standard date-time string representations.
#ifndef _TIME_SHIELD_TIME_FORMATTING_HPP_INCLUDED
#define _TIME_SHIELD_TIME_FORMATTING_HPP_INCLUDED

#include "date_time_struct.hpp"
#include "time_zone_struct.hpp"
#include "time_conversions.hpp"

namespace time_shield {

	inline void process_format_impl(
			const char& last_char,
			const size_t& repeat_count,
			const ts_t& ts,
			const tz_t& utc_offset,
			const DateTimeStruct& dt,
			std::string& result) {
		switch (last_char) {
		case 'a':
			if (repeat_count > 1) break;
			result += to_str(day_of_week(dt.year, dt.mon, dt.day), FormatType::SHORT_NAME);
			break;
		case 'A':
			if (repeat_count > 1) break;
			result += to_str(day_of_week(dt.year, dt.mon, dt.day), FormatType::FULL_NAME);
			break;
		case 'I':
			if (repeat_count == 1) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d", hour24_to_12(dt.hour));
				result += std::string(buffer);
			}
			break;
		case 'H':
			if (repeat_count <= 2) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer),"%.2d", dt.hour);
				result += std::string(buffer);
			}
			break;
		case 'h':
			if (repeat_count == 2) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer),"%.2d", dt.hour);
				result += std::string(buffer);
				break;
			}
			// %h: Equivalent to %b
		case 'b':
			if (repeat_count > 1) break;
			result += to_str(static_cast<Month>(dt.mon), FormatType::SHORT_NAME);
			break;
		case 'B':
			if (repeat_count > 1) break;
			result += to_str(static_cast<Month>(dt.mon), FormatType::FULL_NAME);
			break;
		case 'c':
			// Preferred date and time representation for the current locale.
			// %a %b %e %H:%M:%S %Y
			if (repeat_count <= 1){
				char buffer[16];
				result += to_str(day_of_week(dt.year, dt.mon, dt.day), FormatType::SHORT_NAME);
				result += " ";
				result += to_str(static_cast<Month>(dt.mon), FormatType::SHORT_NAME);
				result += " ";
				// added %e
				std::fill(buffer, buffer + sizeof(buffer), '\0');
				snprintf(buffer, sizeof(buffer),"%2d ", dt.day);
				result += std::string(buffer);
				// added %H:%M:%S
				std::fill(buffer, buffer + sizeof(buffer), '\0');
				snprintf(buffer, sizeof(buffer), "%.2d:%.2d:%.2d ", dt.hour, dt.min, dt.sec);
				result += std::string(buffer);
				// added %Y
				result += std::to_string(dt.year);
			}
			break;
		case 'C':
			if (repeat_count > 1) break;
			result += std::to_string(dt.year/100);
			break;
		case 'd':
			if (repeat_count < 2) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer),"%.2d", dt.day);
				result += std::string(buffer);
			}
			break;
		case 'D':
			if (repeat_count == 1) {
				// %m/%d/%y
				char buffer[16] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d/%.2d/%.2d", dt.mon, dt.day, (int)(dt.year % 100LL));
				result += std::string(buffer);
			} else
			if (repeat_count == 2) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer),"%.2d", dt.day);
				result += std::string(buffer);
			}
			break;
		case 'e':
			if (repeat_count == 1) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer),"%2d", dt.day);
				result += std::string(buffer);
			}
			break;
		case 'E':
			// %E: Modifier for alternative ("era-based") format.
			// https://help.hcltechsw.com/onedb/1.0.0.1/gug/ids_gug_086.html#ids_gug_086
			break;
		case 'F':
			if (repeat_count == 1) {
				// %Y-%m-%d ISO 8601 date format
				char buffer[32] = {0};
				if (dt.year <= 9999 || dt.year >= 0) {
					snprintf(buffer, sizeof(buffer), "%.4d-%.2d-%.2d", (int)dt.year, dt.mon, dt.day);
				} else
				if (dt.year < 0) {
					snprintf(buffer, sizeof(buffer), "-%lld-%.2d-%.2d", dt.year, dt.mon, dt.day);
				} else {
					snprintf(buffer, sizeof(buffer), "+%lld-%.2d-%.2d", dt.year, dt.mon, dt.day);
				}
				result += std::string(buffer);
			}
		case 'g':
			// ISO 8601 week-based year without century (2-digit year).
			break;
		case 'G':
			// ISO 8601 week-based year with century as a decimal number.
			break;
		case 'j':
			if (repeat_count == 1) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%.3d", day_of_year(ts));
				result += std::string(buffer);
			}
			break;
		case 'k':
			if (repeat_count == 1) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%2d", dt.hour);
				result += std::string(buffer);
			}
			break;
		case 'l':
			if (repeat_count == 1) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%2d", hour24_to_12(dt.hour));
				result += std::string(buffer);
			}
			break;
		case 'm':
			if (repeat_count == 1) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d", dt.mon);
				result += std::string(buffer);
			} else
			if (repeat_count == 2) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d", dt.min);
				result += std::string(buffer);
			}
			break;
		case 'M':
			if (repeat_count == 1) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d", dt.min);
				result += std::string(buffer);
			} else
			if (repeat_count == 2) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d", dt.mon);
				result += std::string(buffer);
			} else
			if (repeat_count == 3) {
				result += to_str(static_cast<Month>(dt.mon), FormatType::UPPERCASE_NAME);
			}
			break;
		case 'n':
			result += "\n";
			break;
		case 'O':
			// Modifier for using alternative numeric symbols.
			break;
		case 'p':
			if (dt.hour < 12) result += "AM";
			else result += "PM";
			break;
		case 'P':
			if (dt.hour < 12) result += "am";
			else result += "pm";
			break;
		case 'r':
			if (repeat_count == 1) {
				char buffer[16] = {0};
				if (dt.hour < 12) snprintf(buffer, sizeof(buffer), "%.2d:%.2d:%.2d AM", hour24_to_12(dt.hour), dt.min, dt.sec);
				else snprintf(buffer, sizeof(buffer), "%.2d:%.2d:%.2d PM", hour24_to_12(dt.hour), dt.min, dt.sec);
				result += std::string(buffer);
				break;
			}
			break;
		case 'R':
			// %H:%M
			if (repeat_count == 1) {
				char buffer[8] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d:%.2d", dt.hour, dt.min);
				result += std::string(buffer);
			}
			break;
		case 's':
			if (repeat_count == 1) {
				result += std::to_string(ts);
				break;
			}
			if (repeat_count == 4) {
				result += std::to_string(dt.ms);
				break;
			}
			// to '%ss'
		case 'S':
			if (repeat_count <= 2) {
				char buffer[4] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d", dt.sec);
				result += std::string(buffer);
			}
			break;
		case 't':
			if (repeat_count > 1) break;
			result += "\t";
			break;
		case 'T':
			// %H:%M:%S
			if (repeat_count == 1) {
				char buffer[16] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d:%.2d:%.2d", dt.hour, dt.min, dt.sec);
				result += std::string(buffer);
			}
			break;
		case 'u':
			if (repeat_count == 1) {
				// Day of the week as a decimal number (1 to 7, Monday being 1).
				int dw = day_of_week(dt.year, dt.mon, dt.day);
				if (dw == 0) dw = 7;
				result += std::to_string(dw);
			}
			break;
		case 'U':
			// Week number of the current year (00 to 53, starting with the first Sunday as week 01).
			break;
		case 'V':
			// ISO 8601 week number of the current year (01 to 53, with specific rules).
			break;
		case 'w':
			// Day of the week as a decimal number (0 to 6, Sunday being 0).
			if (repeat_count == 1) {
				result += std::to_string(day_of_week(dt.year, dt.mon, dt.day));
			} else
			if (repeat_count == 3) {
				result += to_str(day_of_week(dt.year, dt.mon, dt.day), FormatType::SHORT_NAME);
			}
			break;
		case 'W':
			// Week number of the current year (00 to 53, starting with the first Monday as week 01).
			if (repeat_count == 3) {
				result += to_str(day_of_week(dt.year, dt.mon, dt.day), FormatType::UPPERCASE_NAME);
			}
			break;
		case 'x':
			// Preferred date representation for the current locale without the time.
			break;
		case 'X':
			// Preferred time representation for the current locale without the date.
			break;
		case 'y':
			if (repeat_count == 1) {
				result += std::to_string(dt.year % 100);
			}
			break;
		case 'Y':
			if (repeat_count == 1) {
				result += std::to_string(dt.year);
			} else
			if (repeat_count == 6) {
				char buffer[32] = {0};
				const int64_t mega_years = dt.year / 1000000;
				const int64_t millennia = (dt.year - mega_years * 1000000) / 1000;
				const int64_t centuries = dt.year - mega_years * 1000000 - millennia * 1000;
				if (mega_years) {
					if (millennia) {
						snprintf(buffer, sizeof(buffer), "%lldM%lldK%.3lld", mega_years, std::abs(millennia), std::abs(centuries));
					} else {
						snprintf(buffer, sizeof(buffer), "%lldM%.3lld", mega_years, std::abs(centuries));
					}
				} else
				if (millennia) {
					snprintf(buffer, sizeof(buffer), "%lldK%.3lld", millennia, std::abs(centuries));
				} else {
					snprintf(buffer, sizeof(buffer), "%.4lld", dt.year);
				}
				result += std::string(buffer);
			} else
			if (repeat_count == 4) {
				char buffer[8] = {0};
				snprintf(buffer, sizeof(buffer), "%.4d", (int)(dt.year % 10000));
				result += std::string(buffer);
			} else
			if (repeat_count == 2) {
				char buffer[8] = {0};
				snprintf(buffer, sizeof(buffer), "%.2d", (int)(dt.year % 100));
				result += std::string(buffer);
			}
			break;
		case 'z':
			// +hhmm or -hhmm numeric timezone offset from UTC.
			if (repeat_count == 1) {
				TimeZoneStruct tz = to_time_zone_struct(utc_offset);
				char buffer[16] = {0};
				if (tz.is_positive) snprintf(buffer, sizeof(buffer), "+%.2d%.2d", tz.hour, tz.min);
				else snprintf(buffer, sizeof(buffer), "-%.2d%.2d", tz.hour, tz.min);
				result += std::string(buffer);
			}
			break;
		case 'Z':
			// Timezone name or abbreviation.

			result += "UTC";
			break;
		case '+':
			// Date and time in date(1) format (not supported in glibc2).
			// Tue Jun 4 04:07:43 UTC 2024
			break;
		};
	}

	/// \brief Convert timestamp to string with custom format.
	///
	/// This function is similar to the strftime function and supports the majority of its specifiers,
	/// as well as additional ones: YY, YYYY, YYYYYY, WWW, www, hh, mm, ss, dd, sss.
	///
	/// The function accepts the following format specifiers as parameters:
	/// - %YYYYYY: Year with reduction in the number of millennia.
	/// - %YYYY: Year represented by 4 digits.
	/// - %YY: Last two digits of the year.
	/// - %MM: Month (01-12).
	/// - %MMM: Abbreviated month name.
	/// - %DD: Day of the month (01-31).
	/// - %hh: Hour of the day in 24-hour format (00-23).
	/// - %mm: Minute of the hour (00-59).
	/// - %ss: Second (00-59).
	/// - %sss: Millisecond (000-999).
	/// - %WWW: Abbreviated day of the week name in uppercase (SUN, MON, TUE, etc.).
	/// - %www: Abbreviated day of the week name (Sun, Mon, Tue, etc.).
	///
	/// For more information, see the strftime specifiers documentation:
	/// \sa https://manpages.debian.org/bullseye/manpages-dev/strftime.3.en.html
	///
	/// \param format_str Format string with custom parameters, e.g., "%H:%M:%S".
	/// \param timestamp Timestamp.
	/// \param utc_offset UTC offset in seconds (default is 0).
	/// \return Returns a string in the format specified by the user.
	template<class T = ts_t>
	const std::string to_string(
			const std::string& format_str,
			const T& timestamp,
			const tz_t& utc_offset = 0) {
		std::string result;
		if (format_str.empty()) return result;
		DateTimeStruct dt = to_date_time<DateTimeStruct>(timestamp);

		char buffer[32];

		bool is_command = false;
		size_t repeat_count = 0;
		char last_char = format_str[0];
		if (last_char != '%') result += last_char;
		for (size_t i = 0; i < format_str.size(); ++i) {
			const char& current_char = format_str[i];
			if (!is_command) {
				if (current_char == '%') {
					++repeat_count;
					if (repeat_count == 2) {
						result += current_char;
						repeat_count = 0;
					}
					continue;
				}
				if (!repeat_count) {
					result += current_char;
					continue;
				}
				last_char = current_char;
				is_command = true;
				continue;
			}
			if (last_char == current_char) {
				++repeat_count;
				continue;
			}
			process_format_impl(last_char, repeat_count, timestamp, utc_offset, dt, result);
			repeat_count = 0;
			is_command = false;
			--i;
		}
		if (is_command) {
			process_format_impl(last_char, repeat_count, timestamp, utc_offset, dt, result);
		}
		return result;
	}

	/// \brief Alias for to_string function.
	/// \copydoc to_string
	template<class T = ts_t>
	inline const std::string to_str(
			const std::string& format_str,
			const T& timestamp,
			const tz_t& utc_offset = 0) {
		return to_string<T>(format_str, timestamp, utc_offset);
	}

	/// \brief Converts a timestamp to an ISO8601 string.
	///
	/// This function converts a timestamp to a string in ISO8601 format.
	///
	/// \tparam T The type of the timestamp (default is ts_t).
	/// \param ts The timestamp to convert.
	/// \return A string representing the timestamp in ISO8601 format.
	template<class T = ts_t>
	inline const std::string to_iso8601_str(const T& ts) {
		DateTimeStruct dt = to_date_time<DateTimeStruct>(ts);
		char buffer[32] = {0};
		if TIME_SHIELD_CONSTEXPR (std::is_floating_point<T>::value) {
			snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms);
		} else {
			snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
		}
		return std::string(buffer);
	}

	/// \brief Converts a timestamp to an ISO8601 string in UTC format.
	///
	/// This function converts a timestamp to a string in ISO8601 format with UTC timezone.
	///
	/// \tparam T The type of the timestamp (default is ts_t).
	/// \param ts The timestamp to convert.
	/// \return A string representing the timestamp in ISO8601 UTC format.
	template<class T = ts_t>
	inline const std::string to_iso8601_utc_str(const T& ts) {
		DateTimeStruct dt = to_date_time<DateTimeStruct>(ts);
		char buffer[32] = {0};
		if TIME_SHIELD_CONSTEXPR (std::is_floating_point<T>::value) {
			snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d.%.3dZ", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms);
		} else {
			snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2dZ", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
		}
		return std::string(buffer);
	}

	/// \brief Converts a timestamp in milliseconds to an ISO8601 string in UTC format.
	///
	/// This function converts a timestamp in milliseconds to a string in ISO8601 format with UTC timezone.
	///
	/// \param ts_ms The timestamp in milliseconds to convert.
	/// \return A string representing the timestamp in ISO8601 UTC format with milliseconds.
	inline const std::string to_iso8601_utc_str_ms(const ts_ms_t& ts_ms) {
		DateTimeStruct dt = to_date_time_ms<DateTimeStruct>(ts_ms);
		char buffer[32] = {0};
		snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d.%.3dZ", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms);
		return std::string(buffer);
	}

	/// \brief Converts a timestamp to an ISO8601 string with timezone offset.
	///
	/// This function converts a timestamp to a string in ISO8601 format with timezone offset.
	///
	/// \tparam T The type of the timestamp (default is ts_t).
	/// \param ts The timestamp to convert.
	/// \param utc_offset The timezone offset in seconds.
	/// \return A string representing the timestamp in ISO8601 format with timezone offset.
	template<class T = ts_t>
	inline const std::string to_iso8601_str(const T& ts, const tz_t &utc_offset) {
		TimeZoneStruct tz = to_time_zone(utc_offset);
		DateTimeStruct dt = to_date_time(ts);
		char buffer[32] = {0};
		if TIME_SHIELD_CONSTEXPR (std::is_floating_point<T>::value) {
			if (tz.is_positive) {
				snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d+%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms, tz.hour, tz.min);
			} else {
				snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d-%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms, tz.hour, tz.min);
			}
		} else {
			if (tz.is_positive) {
				snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d+%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, tz.hour, tz.min);
			} else {
				snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d-%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, tz.hour, tz.min);
			}
		}
		return std::string(buffer);
	}

	/// \brief Converts a timestamp in milliseconds to an ISO8601 string with timezone offset.
	///
	/// This function converts a timestamp in milliseconds to a string in ISO8601 format with timezone offset.
	///
	/// \param ts_ms The timestamp in milliseconds to convert.
	/// \param utc_offset The timezone offset in seconds.
	/// \return A string representing the timestamp in ISO8601 format with timezone offset and milliseconds.
	inline const std::string to_iso8601_str_ms(const ts_ms_t& ts_ms, const tz_t &utc_offset) {
		TimeZoneStruct tz = to_time_zone(utc_offset);
		DateTimeStruct dt = to_date_time_ms<DateTimeStruct>(ts_ms);
		char buffer[32] = {0};
		if (tz.is_positive) {
			snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d+%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms, tz.hour, tz.min);
		} else {
			snprintf(buffer, sizeof(buffer), "%lld-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d-%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.ms, tz.hour, tz.min);
		}
		return std::string(buffer);
	}

	/// \brief Converts a timestamp to a string in MQL5 date and time format.
	///
	/// This function converts a timestamp to a string in MQL5 date and time format (yyyy.mm.dd hh:mi:ss).
	///
	/// \param ts The timestamp to convert.
	/// \return A string representing the timestamp in MQL5 date and time format.
	inline const std::string to_mql5_date_time_str(const ts_t& ts) {
		DateTimeStruct dt = to_date_time<DateTimeStruct>(ts);
		char buffer[32] = {0};
		snprintf(buffer, sizeof(buffer), "%lld.%.2d.%.2d %.2d:%.2d:%.2d", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
		return std::string(buffer);
	}

	/// \brief Alias for to_mql5_date_time_str function.
	/// \copydoc to_mql5_date_time_str
	inline const std::string to_mql5_full_str(const ts_t& ts) {
		return to_mql5_date_time_str(ts);
	}

	/// \brief Converts a timestamp to a string in MQL5 date format.
	///
	/// This function converts a timestamp to a string in MQL5 date format (yyyy.mm.dd).
	///
	/// \param ts The timestamp to convert.
	/// \return A string representing the date part of the timestamp in MQL5 format.
	inline const std::string to_mql5_date_str(const ts_t& ts) {
		DateTimeStruct dt = to_date_time<DateTimeStruct>(ts);
		char buffer[32] = {0};
		snprintf(buffer, sizeof(buffer), "%lld.%.2d.%.2d", dt.year, dt.mon, dt.day);
		return std::string(buffer);
	}

	/// \brief Converts a timestamp to a string in MQL5 time format.
	///
	/// This function converts a timestamp to a string in MQL5 time format (hh:mi:ss).
	///
	/// \param ts The timestamp to convert.
	/// \return A string representing the time part of the timestamp in MQL5 format.
	inline const std::string to_mql5_time_str(const ts_t& ts) {
		DateTimeStruct dt = to_date_time<DateTimeStruct>(ts);
		char buffer[32] = {0};
		snprintf(buffer, sizeof(buffer), "%.2d:%.2d:%.2d", dt.hour, dt.min, dt.sec);
		return std::string(buffer);
	}

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_FORMATTING_HPP_INCLUDED
