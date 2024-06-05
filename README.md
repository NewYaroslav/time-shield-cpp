# Time Shield

Time-Shield is ~~not only a combat tool of Homura Akemi, but also~~ a header-only C++ library designed for time manipulation. This powerful library offers a wide range of functionalities, including time conversion, string formatting, and various transformations.

## Features

- **Date Validation:** Validate dates for correctness, including leap years and day-offs.
- **Time Formatting:** Convert timestamps to formatted strings according to standard or custom patterns.
- **Time Conversions:** Convert between different time representations, including timestamps, date-time structures, and time zones.
- **Utility Functions:** Access a collection of utility functions for time-related calculations and conversions.
- Supports standards from C++11 to C++17.

## Installation

To use Time Shield in your C++ projects, simply include the time_shield.hpp header file in your source code:

```cpp
#include "time_shield.hpp"
```

Then, compile your project with the Time Shield library linked.

## Usage

Here's a quick example demonstrating how to use Time Shield for time manipulation and conversion:

```cpp
#include <iostream>
#include "time_shield.hpp"

int main() {
    // Get the current timestamp
    tsh::ts_t current_ts = tsh::timestamp(); // or ts();
	
	// Calculate the timestamp for the end of the current year
	tsh::ts_t future_ts = tsh::end_of_year(current_ts);
	
	// Calculate the number of days between current timestamp and the future timestamp
	int days = tsh::get_days_difference(current_ts, future_ts); // or days(current_ts, future_ts);
	std::cout << "Days until end of year: " << days << std::endl;
	
    // Convert the future timestamp to a formatted string in ISO8601 format
    std::string formatted_time = tsh::to_iso8601_str(future_ts);

    // Output the formatted time
    std::cout << "Future time: " << formatted_time << std::endl;

    return 0;
}
```

## Documentation

Detailed documentation for Time Shield, including API reference and usage examples, can be found here.

## License

Time Shield is licensed under the [MIT License](LICENSE).