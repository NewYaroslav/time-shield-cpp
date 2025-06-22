#pragma once
#ifndef _TIME_SHIELD_TIME_UTILS_HPP_INCLUDED
#define _TIME_SHIELD_TIME_UTILS_HPP_INCLUDED

/// \file time_utils.hpp
/// \brief Header file with time-related utility functions.
///
/// This file contains various functions used for time calculations and conversions.

#include "config.hpp"
#include <ctime>
#include <limits>

#if defined(_WIN32)
    #include <Windows.h>
#elif defined(__unix__) || defined(__APPLE__)
    #include <unistd.h>
    #include <sys/resource.h>
    #include <sys/times.h>
    #include <time.h>
#else
    #error "Unsupported platform for get_cpu_time()"
#endif

namespace time_shield {

    /// \ingroup time_utils
    /// \brief Get the current timespec.
    /// \return struct timespec The current timespec.
    inline const struct timespec get_timespec_impl() noexcept {
        // https://en.cppreference.com/w/c/chrono/timespec_get
        struct timespec ts;
#       if defined(CLOCK_REALTIME)
        clock_gettime(CLOCK_REALTIME, &ts); // Версия для POSIX
#       else
        timespec_get(&ts, TIME_UTC);
#       endif
        return ts;
    }

    /// \ingroup time_utils
    /// \brief Get the nanosecond part of the current second.
    /// \tparam T Type of the returned value (default is int).
    /// \return T Nanosecond part of the current second.
    template<class T = int>
    inline const T ns_of_sec() noexcept {
        const struct timespec ts = get_timespec_impl();
        return static_cast<T>(ts.tv_nsec);
    }

    /// \ingroup time_utils
    /// \brief Get the microsecond part of the current second.
    /// \tparam T Type of the returned value (default is int).
    /// \return T Microsecond part of the current second.
    template<class T = int>
    const T us_of_sec() noexcept {
        const struct timespec ts = get_timespec_impl();
        return ts.tv_nsec / NS_PER_US;
    }

    /// \ingroup time_utils
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

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in seconds.
    /// \return ts_t Current UTC timestamp in seconds.
    inline const ts_t timestamp() noexcept {
        const struct timespec ts = get_timespec_impl();
        return ts.tv_sec;
    }

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in floating-point seconds.
    /// \return fts_t Current UTC timestamp in floating-point seconds.
    inline const fts_t fts() noexcept {
        const struct timespec ts = get_timespec_impl();
        return ts.tv_sec + static_cast<fts_t>(ts.tv_nsec) / static_cast<fts_t>(NS_PER_SEC);
    }

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in floating-point seconds.
    /// \return fts_t Current UTC timestamp in floating-point seconds.
    inline const fts_t ftimestamp() noexcept {
        const struct timespec ts = get_timespec_impl();
        return ts.tv_sec + static_cast<fts_t>(ts.tv_nsec) / static_cast<fts_t>(NS_PER_SEC);
    }

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in milliseconds.
    /// \return ts_ms_t Current UTC timestamp in milliseconds.
    inline const ts_ms_t ts_ms() noexcept {
        const struct timespec ts = get_timespec_impl();
        return MS_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_MS;
    }

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in milliseconds.
    /// \return ts_ms_t Current UTC timestamp in milliseconds.
    inline const ts_ms_t timestamp_ms() noexcept {
        const struct timespec ts = get_timespec_impl();
        return MS_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_MS;
    }

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in milliseconds.
    /// \return ts_ms_t Current UTC timestamp in milliseconds.
    inline const ts_ms_t now() noexcept {
        const struct timespec ts = get_timespec_impl();
        return MS_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_MS;
    }

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in microseconds.
    /// \return ts_us_t Current UTC timestamp in microseconds.
    inline const ts_us_t ts_us() noexcept {
        const struct timespec ts = get_timespec_impl();
        return US_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_US;
    }

    /// \ingroup time_utils
    /// \brief Get the current UTC timestamp in microseconds.
    /// \return ts_us_t Current UTC timestamp in microseconds.
    inline const ts_us_t timestamp_us() noexcept {
        const struct timespec ts = get_timespec_impl();
        return US_PER_SEC * ts.tv_sec + ts.tv_nsec / NS_PER_US;
    }

    /// \ingroup time_utils
    /// \brief Get the CPU time used by the current process.
    /// \return CPU time in seconds, or NaN if not available.
    /// \note This function attempts multiple fallback methods depending on platform capabilities.
    /// \see https://habr.com/ru/articles/282301/ — original implementation idea
    inline double get_cpu_time() noexcept {
#   if defined(_WIN32)
        FILETIME create_time{}, exit_time{}, kernel_time{}, user_time{};
        if (GetProcessTimes(GetCurrentProcess(), &create_time, &exit_time, &kernel_time, &user_time)) {
            ULARGE_INTEGER li{};
            li.LowPart = user_time.dwLowDateTime;
            li.HighPart = user_time.dwHighDateTime;
            return static_cast<double>(li.QuadPart) / 10000000.0;
        }
#   elif   defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
        // AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris
#       if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
            clockid_t id = (clockid_t)-1;
#           if defined(_POSIX_CPUTIME) && (_POSIX_CPUTIME > 0)
                if (clock_getcpuclockid(0, &id) != 0) {
#                   if defined(CLOCK_PROCESS_CPUTIME_ID)
                        id = CLOCK_PROCESS_CPUTIME_ID;
#                   elif defined(CLOCK_VIRTUAL)
                        id = CLOCK_VIRTUAL;
#                   endif
                }
#           elif defined(CLOCK_PROCESS_CPUTIME_ID)
                id = CLOCK_PROCESS_CPUTIME_ID;
#           elif defined(CLOCK_VIRTUAL)
                id = CLOCK_VIRTUAL;
#           endif
            if (id != (clockid_t)-1) {
                struct timespec ts;
                if (clock_gettime(id, &ts) == 0) {
                    return static_cast<double>(ts.tv_sec) + static_cast<double>(ts.tv_nsec) / 1e9;
                }
            }
#       endif

#       if defined(RUSAGE_SELF)
            struct rusage usage{};
            if (getrusage(RUSAGE_SELF, &usage) == 0) {
                return static_cast<double>(usage.ru_utime.tv_sec) + static_cast<double>(usage.ru_utime.tv_usec) / 1e6;
            }
#       endif

#       if defined(_SC_CLK_TCK)
            struct tms t{};
            if (times(&t) != (clock_t)-1) {
                return static_cast<double>(t.tms_utime) / static_cast<double>(sysconf(_SC_CLK_TCK));
            }
#       endif

#       if defined(CLOCKS_PER_SEC)
            clock_t cl = clock();
            if (cl != (clock_t)-1) {
                return static_cast<double>(cl) / static_cast<double>(CLOCKS_PER_SEC);
            }
#       endif
#   else
#       warning "get_cpu_time() may not work correctly: unsupported platform"
#   endif
        return std::numeric_limits<double>::quiet_NaN();
    }

}; // namespace time_shield

#endif // _TIME_SHIELD_TIME_UTILS_HPP_INCLUDED
