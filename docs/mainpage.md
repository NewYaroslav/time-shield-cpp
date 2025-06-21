\mainpage Time Shield Library

**Version:** `VERSION_PLACEHOLDER`

\section intro_sec Introduction

The Time Shield Library is a comprehensive C++ library designed for time manipulation, formatting, and conversion. This library aims to provide a robust and flexible set of tools for handling various time-related tasks with ease.

\section features_sec Features

- Validation of dates and times
- Time and date formatting
- Time zone calculations
- Conversion between different time representations
- Utilities for time manipulation

\section usage_sec Usage

\code{.cpp}
#include <time_shield.hpp>

int main() {
    tsh::ts_t current_ts = tsh::timestamp();
    tsh::ts_t future_ts = tsh::end_of_year(current_ts);
    int days = tsh::get_days_difference(current_ts, future_ts);
    std::cout << "Days until end of year: " << days << std::endl;

    std::string formatted_time = tsh::to_iso8601_str(future_ts);
    tsh::ts_ms_t ts_ms = 0;
    if (!tsh::str_to_ts_ms(formatted_time, ts_ms)) {
        std::cout << "Error occurred while parsing ISO8601 string" << std::endl;
        return -1;
    }

    formatted_time = tsh::to_iso8601_utc_ms(ts_ms);
    std::cout << "Future time: " << formatted_time << std::endl;
    return 0;
}
\endcode

\section install_sec Installation

To use the Time Shield Library in your project, include the header file `time_shield.hpp` in your source code. Ensure that your project is set up to compile with standards from C++11 to C++17.

\section repo_sec Repository

[Time Shield Library GitHub repository](https://github.com/NewYaroslav/time-shield-cpp)

\section license_sec License

This library is licensed under the MIT License. See the LICENSE file for more details.