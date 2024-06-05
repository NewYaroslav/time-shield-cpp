#pragma once
/// \file date_struct.hpp
/// \brief Header for date structure and related functions.
///
/// This file contains the definition of the DateStruct structure and a function to create DateStruct instances.
#ifndef _TIME_SHIELD_DATE_STRUCT_HPP_INCLUDED
#define _TIME_SHIELD_DATE_STRUCT_HPP_INCLUDED

namespace time_shield {

	/// \brief Structure to represent a date.
	struct DateStruct {
		int64_t year;	///< Year component of the date.
		int		mon;	///< Month component of the date (1-12).
		int		day;	///< Day component of the date (1-31).
	};

	/// \brief Creates a DateStruct instance.
	/// \param year The year component of the date.
	/// \param mon The month component of the date, defaults to 1 (January).
	/// \param day The day component of the date, defaults to 1.
	/// \return A DateStruct instance with the provided date components.
	inline const DateStruct create_date_struct(
			const int64_t& year,
			const int& mon = 1,
			const int& day = 1) {
		DateStruct data{year, mon, day};
		return data;
	}

}; // namespace time_shield

#endif // _TIME_SHIELD_DATE_STRUCT_HPP_INCLUDED
