#pragma once
/// \file types.hpp
/// \brief Header file with type definitions.
///
/// This file contains type definitions for various time-related units and formats.
#ifndef _TIME_SHIELD_TYPES_HPP_INCLUDED
#define _TIME_SHIELD_TYPES_HPP_INCLUDED

#include <cstdint>

namespace time_shield {

	typedef int64_t		year_t;					///< Integer year type
	typedef int64_t		uday_t;					///< Integer unix day type
	typedef int64_t		ts_t;					///< Integer timestamp type
	typedef int64_t		ts_ms_t;				///< Integer timestamp milliseconds type
	typedef int64_t		ts_us_t;				///< Integer timestamp microseconds type
	typedef double		fts_t;					///< Floating point timestamp type
	typedef double		oadate_t;				///< Automation date type (OADate) floating point
	typedef double		jd_t;					///< Julian Date type floating point
	typedef double		mjd_t;					///< Modified Julian Date type floating point
	typedef uint64_t	jdn_t;					///< Julian Day Number type
	typedef int			tz_t;

}; // namespace time_shield

#endif // _TIME_SHIELD_TYPES_HPP_INCLUDED
