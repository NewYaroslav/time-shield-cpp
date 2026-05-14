#pragma once

#include <cstdlib>
#include <iostream>

namespace time_shield_test {

    inline void check_failed(const char* expression,
                             const char* file,
                             int line) {
        std::cerr << file << ':' << line << ": check failed: " << expression
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

} // namespace time_shield_test

#define TIME_SHIELD_TEST_CHECK(condition)                                      \
    do {                                                                       \
        if (!(condition)) {                                                    \
            ::time_shield_test::check_failed(#condition, __FILE__, __LINE__);  \
        }                                                                      \
    } while (false)
