#pragma once
#ifndef _TIME_SHIELD_TIME_CONVERSIONS_ALIASES_HPP_INCLUDED
#define _TIME_SHIELD_TIME_CONVERSIONS_ALIASES_HPP_INCLUDED

/// \file time_conversion_aliases.hpp
/// \brief Doxygen-only stubs for alias functions defined via macros.
///
/// These declarations are only visible to Doxygen to enable searching and linking
/// to functions like `ts`, `get_ts`, etc., which are defined via macros.
/// This file should be included only at the end of `time_conversion.hpp`.

namespace time_shield {
    
/// \ingroup time_conversions
/// \{
    
    /// \brief Alias for get_unix_year function.
    /// \copydoc get_unix_year
    template<class T = year_t>
    constexpr T unix_year(ts_t ts) noexcept {
        return get_unix_year<T>(ts);
    }

    /// \brief Alias for get_unix_year function.
    /// \copydoc get_unix_year
    template<class T = year_t>
    constexpr T to_unix_year(ts_t ts) noexcept {
        return get_unix_year<T>(ts);
    }

//------------------------------------------------------------------------------


    /// \brief Alias for get_unix_day function.
    /// \copydoc get_unix_day
    template<class T = uday_t>
    constexpr T get_unixday(ts_t ts = time_shield::ts()) noexcept {
        return get_unix_day<T>(ts);
    }


    /// \brief Alias for get_unix_day function.
    /// \copydoc get_unix_day
    template<class T = uday_t>
    constexpr T unix_day(ts_t ts = time_shield::ts()) noexcept {
        return get_unix_day<T>(ts);
    }


    /// \brief Alias for get_unix_day function.
    /// \copydoc get_unix_day
    template<class T = uday_t>
    constexpr T unixday(ts_t ts = time_shield::ts()) noexcept {
        return get_unix_day<T>(ts);
    }


    /// \brief Alias for get_unix_day function.
    /// \copydoc get_unix_day
    template<class T = uday_t>
    constexpr T uday(ts_t ts = time_shield::ts()) noexcept {
        return get_unix_day<T>(ts);
    }
    
//------------------------------------------------------------------------------


    /// \brief Alias for get_unix_day_ms function.
    /// \copydoc get_unix_day_ms
    template<class T = uday_t>
    constexpr T get_unixday_ms(ts_ms_t t_ms = time_shield::ts_ms()) noexcept {
        return get_unix_day_ms<T>(t_ms);
    }


    /// \brief Alias for get_unix_day_ms function.
    /// \copydoc get_unix_day_ms
    template<class T = uday_t>
    constexpr T unix_day_ms(ts_ms_t t_ms = time_shield::ts_ms()) noexcept {
        return get_unix_day_ms<T>(t_ms);
    }


    /// \brief Alias for get_unix_day_ms function.
    /// \copydoc get_unix_day_ms
    template<class T = uday_t>
    constexpr T unixday_ms(ts_ms_t t_ms = time_shield::ts_ms()) noexcept {
        return get_unix_day_ms<T>(t_ms);
    }


    /// \brief Alias for get_unix_day_ms function.
    /// \copydoc get_unix_day_ms
    template<class T = uday_t>
    constexpr T uday_ms(ts_ms_t t_ms = time_shield::ts_ms()) noexcept {
        return get_unix_day_ms<T>(t_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for unix_day_to_timestamp function.
    /// \copydoc unix_day_to_timestamp
    template<class T = ts_t>
    constexpr T unix_day_to_ts(uday_t unix_day) noexcept {
        return unix_day_to_timestamp(unix_day);
    }

    /// \brief Alias for unix_day_to_timestamp function.
    /// \copydoc unix_day_to_timestamp
    template<class T = ts_t>
    constexpr T unixday_to_ts(uday_t unix_day) noexcept {
        return unix_day_to_timestamp(unix_day);
    }

    /// \brief Alias for unix_day_to_timestamp function.
    /// \copydoc unix_day_to_timestamp
    template<class T = ts_t>
    constexpr T uday_to_ts(uday_t unix_day) noexcept {
        return unix_day_to_timestamp(unix_day);
    }

    /// \brief Alias for unix_day_to_timestamp function.
    /// \copydoc unix_day_to_timestamp
    template<class T = ts_t>
    constexpr T start_of_day_from_unix_day(uday_t unix_day) noexcept {
        return unix_day_to_timestamp(unix_day);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for unix_day_to_timestamp_ms function.
    /// \copydoc unix_day_to_timestamp_ms
    template<class T = ts_t>
    constexpr T unix_day_to_ts_ms(uday_t unix_day) noexcept {
        return unix_day_to_timestamp_ms(unix_day);
    }

    /// \brief Alias for unix_day_to_timestamp_ms function.
    /// \copydoc unix_day_to_timestamp_ms
    template<class T = ts_t>
    constexpr T unixday_to_ts_ms(uday_t unix_day) noexcept {
        return unix_day_to_timestamp_ms(unix_day);
    }

    /// \brief Alias for unix_day_to_timestamp_ms function.
    /// \copydoc unix_day_to_timestamp_ms
    template<class T = ts_t>
    constexpr T uday_to_ts_ms(uday_t unix_day) noexcept {
        return unix_day_to_timestamp_ms(unix_day);
    }

    /// \brief Alias for unix_day_to_timestamp_ms function.
    /// \copydoc unix_day_to_timestamp_ms
    template<class T = ts_t>
    constexpr T start_of_day_from_unix_day_ms(uday_t unix_day) noexcept {
        return unix_day_to_timestamp_ms(unix_day);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_next_day_from_unix_day function.
    /// \copydoc start_of_next_day_from_unix_day
    template<class T = ts_t>
    constexpr T next_day_from_unix_day(uday_t unix_day) noexcept {
        return start_of_next_day_from_unix_day(unix_day);
    }

    /// \brief Alias for start_of_next_day_from_unix_day function.
    /// \copydoc start_of_next_day_from_unix_day
    template<class T = ts_t>
    constexpr T next_day_unix_day(uday_t unix_day) noexcept {
        return start_of_next_day_from_unix_day(unix_day);
    }

    /// \brief Alias for start_of_next_day_from_unix_day function.
    /// \copydoc start_of_next_day_from_unix_day
    template<class T = ts_t>
    constexpr T next_day_unixday(uday_t unix_day) noexcept {
        return start_of_next_day_from_unix_day(unix_day);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_next_day_from_unix_day_ms function.
    /// \copydoc start_of_next_day_from_unix_day_ms
    template<class T = ts_ms_t>
    constexpr T next_day_from_unix_day_ms(uday_t unix_day) noexcept {
        return start_of_next_day_from_unix_day_ms(unix_day);
    }

    /// \brief Alias for start_of_next_day_from_unix_day_ms function.
    /// \copydoc start_of_next_day_from_unix_day_ms
    template<class T = ts_ms_t>
    constexpr T next_day_unix_day_ms(uday_t unix_day) noexcept {
        return start_of_next_day_from_unix_day_ms(unix_day);
    }

    /// \brief Alias for start_of_next_day_from_unix_day_ms function.
    /// \copydoc start_of_next_day_from_unix_day_ms
    template<class T = ts_ms_t>
    constexpr T next_day_unixday_ms(uday_t unix_day) noexcept {
        return start_of_next_day_from_unix_day_ms(unix_day);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for get_unix_min function.
    /// \copydoc get_unix_min
    template<class T = int64_t>
    constexpr T unix_min(ts_t ts = time_shield::ts()) {
        return get_unix_min(ts);
    }

    /// \brief Alias for get_unix_min function.
    /// \copydoc get_unix_min
    template<class T = int64_t>
    constexpr T to_unix_min(ts_t ts = time_shield::ts()) {
        return get_unix_min(ts);
    }

    /// \brief Alias for get_unix_min function.
    /// \copydoc get_unix_min
    template<class T = int64_t>
    constexpr T umin(ts_t ts = time_shield::ts()) {
        return get_unix_min(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for hour24_to_12 function.
    /// \copydoc hour24_to_12
    template<class T = int>
    TIME_SHIELD_CONSTEXPR inline T h24_to_h12(T hour) noexcept {
        return hour24_to_12(hour);
    }
    
//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Alias for to_date_time function.
    /// \copydoc to_date_time
    template<class T1, class T2 = ts_t>
    T1 to_dt(T2 ts) {
        return to_date_time(ts);
    }
    
    /// \ingroup time_structures
    /// \brief Alias for to_date_time function.
    /// \copydoc to_date_time
    template<class T1, class T2 = ts_t>
    T1 to_dt_struct(T2 ts) {
        return to_date_time(ts);
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Alias for to_date_time_ms function.
    /// \copydoc to_date_time_ms
    template<class T>
    inline T to_dt_ms(ts_ms_t ts) {
        return to_date_time_ms<T>(ts);
    }
    
    /// \ingroup time_structures
    /// \brief Alias for to_date_time_ms function.
    /// \copydoc to_date_time_ms
    template<class T>
    inline T to_dt_struct_ms(ts_ms_t ts) {
        return to_date_time_ms<T>(ts);
    }
    
//------------------------------------------------------------------------------


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t ts(year_t year, int month, int day) {
        return to_timestamp(year, month, day);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t ts(year_t year, int month, int day, int hour) {
        return to_timestamp(year, month, day, hour);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t ts(year_t year, int month, int day, int hour, int min) {
        return to_timestamp(year, month, day, hour, min);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \param sec   The second value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t ts(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp(year, month, day, hour, min, sec);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_ts(year_t year, int month, int day) {
        return to_timestamp(year, month, day);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_ts(year_t year, int month, int day, int hour) {
        return to_timestamp(year, month, day, hour);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_ts(year_t year, int month, int day, int hour, int min) {
        return to_timestamp(year, month, day, hour, min);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \param sec   The second value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_ts(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp(year, month, day, hour, min, sec);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_timestamp(year_t year, int month, int day) {
        return to_timestamp(year, month, day);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_timestamp(year_t year, int month, int day, int hour) {
        return to_timestamp(year, month, day, hour);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_timestamp(year_t year, int month, int day, int hour, int min) {
        return to_timestamp(year, month, day, hour, min);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \param sec   The second value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t get_timestamp(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp(year, month, day, hour, min, sec);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t timestamp(year_t year, int month, int day) {
        return to_timestamp(year, month, day);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t timestamp(year_t year, int month, int day, int hour) {
        return to_timestamp(year, month, day, hour);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t timestamp(year_t year, int month, int day, int hour, int min) {
        return to_timestamp(year, month, day, hour, min);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \param sec   The second value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t timestamp(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp(year, month, day, hour, min, sec);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t to_ts(year_t year, int month, int day) {
        return to_timestamp(year, month, day);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t to_ts(year_t year, int month, int day, int hour) {
        return to_timestamp(year, month, day, hour);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t to_ts(year_t year, int month, int day, int hour, int min) {
        return to_timestamp(year, month, day, hour, min);
    }


    /// \brief Alias for to_timestamp
    ///
    /// This function converts a given date and time to a timestamp, which is the number
    /// of seconds since the Unix epoch (January 1, 1970).
    ///
    /// If the `day` is ≥ 1970 and `year` ≤ 31, parameters are assumed to be in DD-MM-YYYY order
    /// and are automatically reordered.
    ///
    /// \param year The year value.
    /// \param month The month value.
    /// \param day   The day value.
    /// \param hour  The hour value.
    /// \param min   The minute value.
    /// \param sec   The second value.
    /// \return Timestamp representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp
    TIME_SHIELD_CONSTEXPR inline ts_t to_ts(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp(year, month, day, hour, min, sec);
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp function.
    /// \copydoc dt_to_timestamp
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t to_timestamp(
            const T& date_time) {
        return dt_to_timestamp(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp function.
    /// \copydoc dt_to_timestamp
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t to_ts(
            const T& date_time) {
        return dt_to_timestamp(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp function.
    /// \copydoc dt_to_timestamp
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t ts(
            const T& date_time) {
        return dt_to_timestamp(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp function.
    /// \copydoc dt_to_timestamp
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t timestamp(
            const T& date_time) {
        return dt_to_timestamp(date_time);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for tm_to_timestamp
    /// \copydoc tm_to_timestamp
    constexpr ts_t ts(const std::tm* timeinfo) {
        return tm_to_timestamp(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp
    /// \copydoc tm_to_timestamp
    constexpr ts_t get_ts(const std::tm* timeinfo) {
        return tm_to_timestamp(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp
    /// \copydoc tm_to_timestamp
    constexpr ts_t timestamp(const std::tm* timeinfo) {
        return tm_to_timestamp(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp
    /// \copydoc tm_to_timestamp
    constexpr ts_t get_timestamp(const std::tm* timeinfo) {
        return tm_to_timestamp(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp
    /// \copydoc tm_to_timestamp
    constexpr ts_t to_ts(const std::tm* timeinfo) {
        return tm_to_timestamp(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp
    /// \copydoc tm_to_timestamp
    constexpr ts_t ts_from_tm(const std::tm* timeinfo) {
        return tm_to_timestamp(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp
    /// \copydoc tm_to_timestamp
    constexpr ts_t to_timestamp(const std::tm* timeinfo) {
        return tm_to_timestamp(timeinfo);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t ts_ms(year_t year, int month, int day) {
        return to_timestamp_ms(year, month, day);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t ts_ms(year_t year, int month, int day, int hour) {
        return to_timestamp_ms(year, month, day, hour);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t ts_ms(year_t year, int month, int day, int hour, int min) {
        return to_timestamp_ms(year, month, day, hour, min);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t ts_ms(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp_ms(year, month, day, hour, min, sec);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \param ms The millisecond value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t ts_ms(year_t year, int month, int day, int hour, int min, int sec, int ms) {
        return to_timestamp_ms(year, month, day, hour, min, sec, ms);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_ts_ms(year_t year, int month, int day) {
        return to_timestamp_ms(year, month, day);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_ts_ms(year_t year, int month, int day, int hour) {
        return to_timestamp_ms(year, month, day, hour);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_ts_ms(year_t year, int month, int day, int hour, int min) {
        return to_timestamp_ms(year, month, day, hour, min);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_ts_ms(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp_ms(year, month, day, hour, min, sec);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \param ms The millisecond value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_ts_ms(year_t year, int month, int day, int hour, int min, int sec, int ms) {
        return to_timestamp_ms(year, month, day, hour, min, sec, ms);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_timestamp_ms(year_t year, int month, int day) {
        return to_timestamp_ms(year, month, day);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_timestamp_ms(year_t year, int month, int day, int hour) {
        return to_timestamp_ms(year, month, day, hour);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_timestamp_ms(year_t year, int month, int day, int hour, int min) {
        return to_timestamp_ms(year, month, day, hour, min);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_timestamp_ms(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp_ms(year, month, day, hour, min, sec);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \param ms The millisecond value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t get_timestamp_ms(year_t year, int month, int day, int hour, int min, int sec, int ms) {
        return to_timestamp_ms(year, month, day, hour, min, sec, ms);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t timestamp_ms(year_t year, int month, int day) {
        return to_timestamp_ms(year, month, day);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t timestamp_ms(year_t year, int month, int day, int hour) {
        return to_timestamp_ms(year, month, day, hour);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t timestamp_ms(year_t year, int month, int day, int hour, int min) {
        return to_timestamp_ms(year, month, day, hour, min);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t timestamp_ms(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp_ms(year, month, day, hour, min, sec);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \param ms The millisecond value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t timestamp_ms(year_t year, int month, int day, int hour, int min, int sec, int ms) {
        return to_timestamp_ms(year, month, day, hour, min, sec, ms);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t to_ts_ms(year_t year, int month, int day) {
        return to_timestamp_ms(year, month, day);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t to_ts_ms(year_t year, int month, int day, int hour) {
        return to_timestamp_ms(year, month, day, hour);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t to_ts_ms(year_t year, int month, int day, int hour, int min) {
        return to_timestamp_ms(year, month, day, hour, min);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t to_ts_ms(year_t year, int month, int day, int hour, int min, int sec) {
        return to_timestamp_ms(year, month, day, hour, min, sec);
    }

    /// \brief Alias for to_timestamp_ms
    ///
    /// This function converts a given date and time to a timestamp in milliseconds,
    /// which is the number of milliseconds since the Unix epoch (January 1, 1970).
    ///
    /// \tparam T1 The type of the year parameter (default is int64_t).
    /// \tparam T2 The type of the other date and time parameters (default is int).
    /// \param year The year value.
    /// \param month The month value.
    /// \param day The day value.
    /// \param hour The hour value.
    /// \param min The minute value.
    /// \param sec The second value.
    /// \param ms The millisecond value.
    /// \return Timestamp in milliseconds representing the given date and time.
    /// \throws std::invalid_argument if the date-time combination is invalid.
    /// \see to_timestamp_ms
    constexpr ts_ms_t to_ts_ms(year_t year, int month, int day, int hour, int min, int sec, int ms) {
        return to_timestamp_ms(year, month, day, hour, min, sec, ms);
    }

//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp_ms function.
    /// \copydoc dt_to_timestamp_ms
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t to_timestamp_ms(
            const T& date_time) {
        return dt_to_timestamp_ms(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp_ms function.
    /// \copydoc dt_to_timestamp_ms
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t to_ts_ms(
            const T& date_time) {
        return dt_to_timestamp_ms(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp_ms function.
    /// \copydoc dt_to_timestamp_ms
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t ts_ms(
            const T& date_time) {
        return dt_to_timestamp_ms(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_timestamp_ms function.
    /// \copydoc dt_to_timestamp_ms
    template<class T>
    TIME_SHIELD_CONSTEXPR inline ts_t timestamp_ms(
            const T& date_time) {
        return dt_to_timestamp_ms(date_time);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for tm_to_timestamp_ms function.
    /// \copydoc tm_to_timestamp_ms
    TIME_SHIELD_CONSTEXPR inline ts_t to_timestamp_ms(
            const std::tm *timeinfo) {
        return tm_to_timestamp_ms(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp_ms function.
    /// \copydoc tm_to_timestamp_ms
    TIME_SHIELD_CONSTEXPR inline ts_t to_ts_ms(
            const std::tm *timeinfo) {
        return tm_to_timestamp_ms(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp_ms function.
    /// \copydoc tm_to_timestamp_ms
    TIME_SHIELD_CONSTEXPR inline ts_t ts_ms(
            const std::tm *timeinfo) {
        return tm_to_timestamp_ms(timeinfo);
    }

    /// \brief Alias for tm_to_timestamp_ms function.
    /// \copydoc tm_to_timestamp_ms
    TIME_SHIELD_CONSTEXPR inline ts_t timestamp_ms(
            const std::tm *timeinfo) {
        return tm_to_timestamp_ms(timeinfo);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for to_ftimestamp
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
    /// \see to_ftimestamp
    template<class T1 = year_t, class T2 = int, class T3 = int>
    constexpr fts_t to_fts(T1 year, T2 month, T2 day, T2 hour = 0, T2 min = 0, T2 sec = 0, T3 ms = 0) {
        return to_ftimestamp(year, month, day, hour, min, sec, ms);
    }

    /// \brief Alias for to_ftimestamp
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
    /// \see to_ftimestamp
    template<class T1 = year_t, class T2 = int, class T3 = int>
    constexpr fts_t fts(T1 year, T2 month, T2 day, T2 hour = 0, T2 min = 0, T2 sec = 0, T3 ms = 0) {
        return to_ftimestamp(year, month, day, hour, min, sec, ms);
    }

    /// \brief Alias for to_ftimestamp
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
    /// \see to_ftimestamp
    template<class T1 = year_t, class T2 = int, class T3 = int>
    constexpr fts_t ftimestamp(T1 year, T2 month, T2 day, T2 hour = 0, T2 min = 0, T2 sec = 0, T3 ms = 0) {
        return to_ftimestamp(year, month, day, hour, min, sec, ms);
    }
    
//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Alias for dt_to_ftimestamp
    /// \copydoc dt_to_ftimestamp
    template<class T>
    constexpr fts_t to_ftimestamp(const T& date_time) {
        return dt_to_ftimestamp(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_ftimestamp
    /// \copydoc dt_to_ftimestamp
    template<class T>
    constexpr fts_t to_fts(const T& date_time) {
        return dt_to_ftimestamp(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_ftimestamp
    /// \copydoc dt_to_ftimestamp
    template<class T>
    constexpr fts_t fts(const T& date_time) {
        return dt_to_ftimestamp(date_time);
    }

    /// \ingroup time_structures
    /// \brief Alias for dt_to_ftimestamp
    /// \copydoc dt_to_ftimestamp
    template<class T>
    constexpr fts_t ftimestamp(const T& date_time) {
        return dt_to_ftimestamp(date_time);
    }
    
//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Alias for tm_to_ftimestamp
    /// \copydoc tm_to_ftimestamp(const std::tm*)
    constexpr fts_t to_ftimestamp(const std::tm* timeinfo) {
        return tm_to_ftimestamp(timeinfo);
    }

    /// \ingroup time_structures
    /// \brief Alias for tm_to_ftimestamp
    /// \copydoc tm_to_ftimestamp(const std::tm*)
    constexpr fts_t to_fts(const std::tm* timeinfo) {
        return tm_to_ftimestamp(timeinfo);
    }
    
    /// \ingroup time_structures
    /// \brief Alias for tm_to_ftimestamp
    /// \copydoc tm_to_ftimestamp(const std::tm*)
    constexpr fts_t fts(const std::tm* timeinfo) {
        return tm_to_ftimestamp(timeinfo);
    }

    /// \ingroup time_structures
    /// \brief Alias for tm_to_ftimestamp
    /// \copydoc tm_to_ftimestamp(const std::tm*)
    constexpr fts_t ftimestamp(const std::tm* timeinfo) {
        return tm_to_ftimestamp(timeinfo);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for get_days_difference function.
    /// \copydoc get_days_difference
    template<class T = int>
    constexpr T get_days(ts_t start, ts_t stop) noexcept {
        return get_days_difference(start, stop);
    }

    /// \brief Alias for get_days_difference function.
    /// \copydoc get_days_difference
    template<class T = int>
    constexpr T days(ts_t start, ts_t stop) noexcept {
        return get_days_difference(start, stop);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for get_year function.
    /// \copydoc get_year
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T year(ts_t ts = time_shield::ts()) {
        return get_year(ts);
    }

    /// \brief Alias for get_year function.
    /// \copydoc get_year
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T to_year(ts_t ts = time_shield::ts()) {
        return get_year(ts);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for get_year_ms function.
    /// \copydoc get_year_ms
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T year_ms(ts_ms_t ts_ms = time_shield::ts_ms()) {
        return get_year_ms(ts_ms);
    }

    /// \brief Alias for get_year_ms function.
    /// \copydoc get_year_ms
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline T to_year_ms(ts_ms_t ts_ms = time_shield::ts_ms()) {
        return get_year_ms(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_year function.
    /// \copydoc start_of_year
    TIME_SHIELD_CONSTEXPR inline ts_t year_start(ts_t ts = time_shield::ts()) {
        return start_of_year(ts);
    }

    /// \brief Alias for start_of_year function.
    /// \copydoc start_of_year
    TIME_SHIELD_CONSTEXPR inline ts_t year_begin(ts_t ts = time_shield::ts()) {
        return start_of_year(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_year_ms function.
    /// \copydoc start_of_year_ms
    TIME_SHIELD_CONSTEXPR inline ts_t year_start_ms(ts_t ts_ms = time_shield::ts_ms()) {
        return start_of_year_ms(ts_ms);
    }

    /// \brief Alias for start_of_year_ms function.
    /// \copydoc start_of_year_ms
    TIME_SHIELD_CONSTEXPR inline ts_t year_begin_ms(ts_t ts_ms = time_shield::ts_ms()) {
        return start_of_year_ms(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_year_date function.
    /// \copydoc start_of_year_date
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_t year_start_date(T year) {
        return start_of_year_date(year);
    }

    /// \brief Alias for start_of_year_date function.
    /// \copydoc start_of_year_date
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_t year_begin_date(T year) {
        return start_of_year_date(year);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for start_of_year_date_ms function.
    /// \copydoc start_of_year_date_ms
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_ms_t year_start_date_ms(T year) {
        return start_of_year_date_ms(year);
    }

    /// \brief Alias for start_of_year_date_ms function.
    /// \copydoc start_of_year_date_ms
    template<class T = year_t>
    TIME_SHIELD_CONSTEXPR inline ts_ms_t year_begin_date_ms(T year) {
        return start_of_year_date_ms(year);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_year function.
    /// \copydoc end_of_year
    TIME_SHIELD_CONSTEXPR inline ts_t year_end(ts_t ts = time_shield::ts()) {
        return end_of_year(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_year_ms function.
    /// \copydoc end_of_year_ms
    TIME_SHIELD_CONSTEXPR inline ts_ms_t year_end_ms(ts_ms_t ts_ms = time_shield::ts_ms()) {
        return end_of_year_ms(ts_ms);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for num_days_in_month function.
    /// \param year Year as an integer.
    /// \param month Month as an integer.
    /// \return The number of days in the given month and year.
    template<class T1 = int, class T2 = year_t, class T3 = int>
    constexpr T1 days_in_month(T2 year, T3 month) noexcept {
        return num_days_in_month(year, month);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for num_days_in_month_ts function.
    /// \copydoc num_days_in_month_ts
    template<class T1 = int>
    constexpr T1 num_days_in_month(ts_t ts = time_shield::ts()) noexcept {
        return num_days_in_month_ts(ts);
    }

    /// \brief Alias for num_days_in_month_ts function.
    /// \copydoc num_days_in_month_ts
    template<class T1 = int>
    constexpr T1 days_in_month(ts_t ts = time_shield::ts()) noexcept {
        return num_days_in_month_ts(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for num_days_in_year function.
    /// \copydoc num_days_in_year
    template<class T1 = int, class T2 = year_t>
    constexpr T1 days_in_year(T2 year) noexcept {
        return num_days_in_year(year);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for num_days_in_year_ts function.
    /// \copydoc num_days_in_year_ts
    template<class T = int>
    constexpr T days_in_year_ts(ts_t ts = time_shield::ts()) {
        return num_days_in_year_ts(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_day function.
    /// \copydoc start_of_day
    constexpr ts_t day_start(ts_t ts = time_shield::ts()) noexcept {
        return start_of_day(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_prev_day function.
    /// \copydoc start_of_prev_day
    template<class T = int>
    constexpr ts_t previous_day_start(ts_t ts = time_shield::ts(), T days = 1) noexcept {
        return start_of_prev_day(ts, days);
    }
    
//------------------------------------------------------------------------------
    
    /// \brief Alias for start_of_day_sec function.
    /// \copydoc start_of_day_sec
    constexpr ts_t day_start_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_day(ms_to_sec(ts_ms));
    }
    
    /// \brief Alias for start_of_day_sec function.
    /// \copydoc start_of_day_sec
    constexpr ts_t start_day_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_day(ms_to_sec(ts_ms));
    }

//------------------------------------------------------------------------------
    
    /// \brief Alias for start_of_day_ms function.
    /// \copydoc start_of_day_ms
    constexpr ts_ms_t day_start_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_day_ms(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_next_day function.
    /// \copydoc start_of_next_day
    template<class T = int>
    constexpr ts_t next_day_start(ts_t ts, T days = 1) noexcept {
        return start_of_next_day(ts, days);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_next_day_ms function.
    /// \copydoc start_of_next_day_ms
    template<class T = int>
    constexpr ts_ms_t next_day_start_ms(ts_ms_t ts_ms, T days = 1) noexcept {
        return start_of_next_day_ms(ts_ms, days);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_day function.
    /// \copydoc end_of_day
    constexpr ts_t day_end(ts_t ts = time_shield::ts()) noexcept {
        return end_of_day(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_day_sec function.
    /// \copydoc end_of_day_sec
    constexpr ts_t day_end_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_day_sec(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_day_ms function.
    /// \copydoc end_of_day_ms
    constexpr ts_ms_t day_end_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_day_ms(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 day_of_week(year_t year, int month, int day) {
        return day_of_week_date<T1>(year, month, day);
    }

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 day_of_week(year_t year, Month month, int day) {
        return day_of_week_date<T1>(year, static_cast<int>(month), day);
    }

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 get_weekday(year_t year, int month, int day) {
        return day_of_week_date<T1>(year, month, day);
    }

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 get_weekday(year_t year, Month month, int day) {
        return day_of_week_date<T1>(year, static_cast<int>(month), day);
    }

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 weekday(year_t year, int month, int day) {
        return day_of_week_date<T1>(year, month, day);
    }

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 weekday(year_t year, Month month, int day) {
        return day_of_week_date<T1>(year, static_cast<int>(month), day);
    }

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 dow(year_t year, int month, int day) {
        return day_of_week_date<T1>(year, month, day);
    }

    /// \brief Alias for day_of_week_date
    /// \copydoc day_of_week_date
    template<class T1 = Weekday>
    constexpr T1 dow(year_t year, Month month, int day) {
        return day_of_week_date<T1>(year, static_cast<int>(month), day);
    }
    
//------------------------------------------------------------------------------

    /// \ingroup time_structures
    /// \brief Alias for get_weekday_from_date
    /// \copydoc get_weekday_from_date
    template<class T1 = Weekday, class T2, typename std::enable_if<std::is_class<T2>::value, int>::type = 0>
    constexpr T1 get_dow(const T2& date) {
        return get_weekday_from_date<T1>(date);
    }

    /// \ingroup time_structures
    /// \brief Alias for get_weekday_from_date
    /// \copydoc get_weekday_from_date
    template<class T1 = Weekday, class T2, typename std::enable_if<std::is_class<T2>::value, int>::type = 0>
    constexpr T1 dow_from_date(const T2& date) {
        return get_weekday_from_date<T1>(date);
    }

    /// \ingroup time_structures
    /// \brief Alias for get_weekday_from_date
    /// \copydoc get_weekday_from_date
    template<class T1 = Weekday, class T2, typename std::enable_if<std::is_class<T2>::value, int>::type = 0>
    constexpr T1 weekday_of(const T2& date) {
        return get_weekday_from_date<T1>(date);
    }

    /// \ingroup time_structures
    /// \brief Alias for get_weekday_from_date
    /// \copydoc get_weekday_from_date
    template<class T1 = Weekday, class T2, typename std::enable_if<std::is_class<T2>::value, int>::type = 0>
    constexpr T1 day_of_week_dt(const T2& date) {
        return get_weekday_from_date<T1>(date);
    }

    /// \ingroup time_structures
    /// \brief Alias for get_weekday_from_date
    /// \copydoc get_weekday_from_date
    template<class T1 = Weekday, class T2, typename std::enable_if<std::is_class<T2>::value, int>::type = 0>
    constexpr T1 day_of_week(const T2& date) {
        return get_weekday_from_date<T1>(date);
    }

    /// \ingroup time_structures
    /// \brief Alias for get_weekday_from_date
    /// \copydoc get_weekday_from_date
    template<class T1 = Weekday, class T2, typename std::enable_if<std::is_class<T2>::value, int>::type = 0>
    constexpr T1 dow(const T2& date) {
        return get_weekday_from_date<T1>(date);
    }
    
//------------------------------------------------------------------------------


    /// \brief Alias for get_weekday_from_ts
    /// \copydoc get_weekday_from_ts
    template<class T = Weekday, class U, typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
    constexpr T day_of_week(U ts) noexcept {
        return get_weekday_from_ts<T>(ts);
    }

    /// \brief Alias for get_weekday_from_ts
    /// \copydoc get_weekday_from_ts
    template<class T = Weekday, class U, typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
    constexpr T dow_ts(U ts) noexcept {
        return get_weekday_from_ts<T>(ts);
    }

    /// \brief Alias for get_weekday_from_ts
    /// \copydoc get_weekday_from_ts
    template<class T = Weekday, class U, typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
    constexpr T get_dow_from_ts(U ts) noexcept {
        return get_weekday_from_ts<T>(ts);
    }

    /// \brief Alias for get_weekday_from_ts
    /// \copydoc get_weekday_from_ts
    template<class T = Weekday, class U, typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
    constexpr T weekday_of_ts(U ts) noexcept {
        return get_weekday_from_ts<T>(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for get_weekday_from_ts_ms function.
    /// \copydoc get_weekday_from_ts_ms
    template<class T = Weekday>
    constexpr T day_of_week_ms(ts_ms_t ts_ms) {
        return get_weekday_from_ts_ms(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_month function.
    /// \copydoc start_of_month
    TIME_SHIELD_CONSTEXPR inline ts_t month_begin(ts_t ts = time_shield::ts()) {
        return start_of_month(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_month function.
    /// \copydoc end_of_month
    TIME_SHIELD_CONSTEXPR inline ts_t last_day_of_month(ts_t ts = time_shield::ts()) {
        return end_of_month(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for last_sunday_of_month function.
    /// \copydoc last_sunday_of_month
    TIME_SHIELD_CONSTEXPR inline ts_t final_sunday_of_month(ts_t ts = time_shield::ts()) {
        return last_sunday_of_month(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for last_sunday_month_day function.
    /// \copydoc last_sunday_month_day
    template<class T1 = int, class T2 = year_t, class T3 = int>
    TIME_SHIELD_CONSTEXPR inline T1 final_sunday_month_day(T2 year, T3 month) {
        return last_sunday_month_day(year, month);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for start_of_hour function.
    /// \copydoc start_of_hour
    constexpr ts_t hour_begin(ts_t ts = time_shield::ts()) noexcept {
        return start_of_hour(ts);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for start_of_hour_sec function.
    /// \copydoc start_of_hour_sec
    constexpr ts_t hour_begin_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_hour_sec(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_hour_ms function.
    /// \copydoc start_of_hour_ms
    constexpr ts_ms_t hour_begin_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return start_of_hour_ms(ts_ms);
    }

//------------------------------------------------------------------------------

    /// \brief Alias for end_of_hour function.
    /// \copydoc end_of_hour
    constexpr ts_t finish_of_hour(ts_t ts = time_shield::ts()) noexcept {
        return end_of_hour(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_hour_sec function.
    /// \copydoc end_of_hour_sec
    constexpr ts_t finish_of_hour_sec(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_hour_sec(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_hour_ms function.
    /// \copydoc end_of_hour_ms
    constexpr ts_ms_t finish_of_hour_ms(ts_ms_t ts_ms = time_shield::ts_ms()) noexcept {
        return end_of_hour_ms(ts_ms);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for hour_of_day function.
    /// \copydoc hour_of_day
    template<class T = int>
    constexpr T hour_in_day(ts_t ts = time_shield::ts()) noexcept {
        return hour_of_day(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_week function.
    /// \copydoc start_of_week
    constexpr ts_t week_begin(ts_t ts = time_shield::ts()) {
        return start_of_week(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_week function.
    /// \copydoc end_of_week
    constexpr ts_t finish_of_week(ts_t ts = time_shield::ts()) {
        return end_of_week(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_saturday function.
    /// \copydoc start_of_saturday
    constexpr ts_t saturday_begin(ts_t ts = time_shield::ts()) {
        return start_of_saturday(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for start_of_min function.
    /// \copydoc start_of_min
    constexpr ts_t min_begin(ts_t ts = time_shield::ts()) noexcept {
        return start_of_min(ts);
    }
    
//------------------------------------------------------------------------------

    /// \brief Alias for end_of_min function.
    /// \copydoc end_of_min
    constexpr ts_t finish_of_min(ts_t ts = time_shield::ts()) noexcept {
        return end_of_min(ts);
    }
    
/// \}

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_CONVERSIONS_ALIASES_HPP_INCLUDED