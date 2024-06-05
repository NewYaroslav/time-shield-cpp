#pragma once
/// \file constants.hpp
/// \brief Header file with time-related constants.
///
/// This file contains various constants used for time calculations and conversions.
#ifndef _TIME_SHIELD_CONSTANTS_HPP_INCLUDED
#define _TIME_SHIELD_CONSTANTS_HPP_INCLUDED

#include <cstdint>
#include <limits>

namespace time_shield {

	// Nanoseconds and microseconds
	const int64_t NS_PER_US			= 1000;			///< Nanoseconds per microsecond
	const int64_t NS_PER_MS			= 1000000;		///< Nanoseconds per millisecond
	const int64_t NS_PER_SEC		= 1000000000;	///< Nanoseconds per second

	// Microseconds and milliseconds
	const int64_t US_PER_SEC	   = 1000000;		///< Microseconds per second
	const int64_t MS_PER_SEC	   = 1000;			///< Milliseconds per second
	const int64_t MS_PER_MIN	   = 60000;			///< Milliseconds per minute
	const int64_t MS_PER_HALF_HOUR = 1800000;		///< Milliseconds per half hour
	const int64_t MS_PER_HOUR	   = 3600000;		///< Milliseconds per hour
	const int64_t MS_PER_DAY	   = 86400000;		///< Milliseconds per day

	// Seconds
	const int64_t SEC_PER_MIN	   = 60;			///< Seconds per minute
	const int64_t SEC_PER_HALF_HOUR= 1800;			///< Seconds per half hour
	const int64_t SEC_PER_HOUR	   = 3600;			///< Seconds per hour
	const int64_t SEC_PER_DAY	   = 86400;			///< Seconds per day
	const int64_t SEC_PER_YEAR	   = 31536000;		///< Seconds per year (365 days)
	const int64_t AVG_SEC_PER_YEAR = 31557600;		///< Average seconds per year (365.25 days)
	const int64_t SEC_PER_LEAP_YEAR= 31622400;		///< Seconds per leap year (366 days)
	const int64_t SEC_PER_4_YEARS  = 126230400;		///< Seconds per 4 years
	const int64_t SEC_PER_FIRST_100_YEARS = 3155760000; ///< Seconds per first 100 years
	const int64_t SEC_PER_100_YEARS = 3155673600;	///< Seconds per 100 years
	const int64_t SEC_PER_400_YEARS = 12622780800;	///< Seconds per 400 years
	const int64_t MAX_SEC_PER_DAY  = 86399;			///< Maximum seconds per day

	// Minutes
	const int64_t MIN_PER_HOUR	   = 60;			///< Minutes per hour
	const int64_t MIN_PER_DAY	   = 1440;			///< Minutes per day
	const int64_t MIN_PER_WEEK	   = 10080;			///< Minutes per week
	const int64_t MIN_PER_MONTH	   = 40320;			///< Minutes per month (28 days)
	const int64_t MAX_MOON_MIN	   = 42523;			///< Maximum lunar minutes

	// Hours and days
	const int64_t HOURS_PER_DAY	   = 24;			///< Hours per day
	const int64_t DAYS_PER_WEEK	   = 7;				///< Days per week
	const int64_t DAYS_PER_LEAP_YEAR = 366;			///< Days per leap year
	const int64_t DAYS_PER_YEAR	   = 365;			///< Days per year
	const int64_t DAYS_PER_4_YEARS = 1461;			///< Days per 4 years

	// Months and years
	const int64_t MONTHS_PER_YEAR  = 12;			///< Months per year
	const int64_t MAX_DAYS_PER_MONTH = 31;			///< Maximum days per month
	const int64_t LEAP_YEAR_PER_100_YEAR = 24;		///< Leap years per 100 years
	const int64_t LEAP_YEAR_PER_400_YEAR = 97;		///< Leap years per 400 years

	// Epoch and maximum values
	const int64_t UNIX_EPOCH	   = 1970;			///< Start year of UNIX time
	const int64_t OLE_EPOCH		   = 25569;			///< OLE automation date since UNIX epoch
	const int64_t MAX_YEAR		   = 292277022000LL;///< Maximum representable year
	const int64_t MIN_YEAR		   = -2967369602200LL; ///< Minimum representable year
	const int64_t ERROR_YEAR	   = 9223372036854770000LL; ///< Error year value
	const int64_t MAX_TIMESTAMP	   = 9223371890843040000LL; ///< Maximum timestamp value
	const int64_t ERROR_TIMESTAMP  = 9223372036854770000LL; ///< Error timestamp value
	const double MAX_OADATE		   = std::numeric_limits<double>::max(); ///< Maximum OLE automation date
	const double AVG_DAYS_PER_YEAR = 365.25;	   ///< Average days per year

}; // namespace time_shield

// Define the macro for milliseconds per second
#define TS_MS_PER_SEC 1000
#define TS_F_MS_PER_SEC 1000.0

#endif // _TIME_SHIELD_CONSTANTS_HPP_INCLUDED
