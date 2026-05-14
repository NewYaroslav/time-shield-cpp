#include <time_shield/time_parser.hpp>

#include "test_assert.hpp"

#if __cplusplus >= 201703L
#   include <string_view>
#endif

int main() {
    using namespace time_shield;

    ts_t seconds = 0;
    ts_ms_t milliseconds = 0;

    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("M1", seconds) && seconds == SEC_PER_MIN);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("M15", seconds) && seconds == SEC_PER_15_MIN);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("H1", seconds) && seconds == SEC_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("H4", seconds) && seconds == SEC_PER_4_HOUR);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("D1", seconds) && seconds == SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("W1", seconds) && seconds == 7 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("MN1", seconds) && seconds == 30 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("Q1", seconds) && seconds == 90 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("Y1", seconds) && seconds == SEC_PER_YEAR);

    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("minute", seconds) && seconds == SEC_PER_MIN);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("15 MIN", seconds) && seconds == SEC_PER_15_MIN);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("hour", seconds) && seconds == SEC_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("2 weeks", seconds) && seconds == 14 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec(" Month ", seconds) && seconds == 30 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec("15MIN", seconds) && seconds == SEC_PER_15_MIN);

    TIME_SHIELD_TEST_CHECK(str_to_timeframe_ms("M15", milliseconds) && milliseconds == MS_PER_15_MIN);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_ms("hour", milliseconds) && milliseconds == MS_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(timeframe_sec("H1") == SEC_PER_HOUR);
    TIME_SHIELD_TEST_CHECK(timeframe_ms("2 weeks") == static_cast<ts_ms_t>(14) * MS_PER_DAY);

    const char* compact = "H4";
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec(compact, seconds) && seconds == SEC_PER_4_HOUR);
    TIME_SHIELD_TEST_CHECK(timeframe_sec(compact) == SEC_PER_4_HOUR);

    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("   ", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("M0", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("-H1", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("1.5h", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("1m", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("abc", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_sec("S9223372036854775808", seconds));
    TIME_SHIELD_TEST_CHECK(!str_to_timeframe_ms("S9223372036854776", milliseconds));
    TIME_SHIELD_TEST_CHECK(timeframe_sec("abc") == 0);
    TIME_SHIELD_TEST_CHECK(timeframe_ms("abc") == 0);

#if __cplusplus >= 201703L
    const std::string_view compact_view("W1");
    const std::string_view word_view("minute");
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_sec(compact_view, seconds) && seconds == 7 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(str_to_timeframe_ms(word_view, milliseconds) && milliseconds == MS_PER_MIN);
    TIME_SHIELD_TEST_CHECK(timeframe_sec(compact_view) == 7 * SEC_PER_DAY);
    TIME_SHIELD_TEST_CHECK(timeframe_ms(word_view) == MS_PER_MIN);
#endif

    return 0;
}