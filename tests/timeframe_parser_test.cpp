#include <time_shield/time_parser.hpp>

#include <cassert>

#if __cplusplus >= 201703L
#   include <string_view>
#endif

int main() {
    using namespace time_shield;

    ts_t seconds = 0;
    ts_ms_t milliseconds = 0;

    assert(str_to_timeframe_sec("M1", seconds) && seconds == SEC_PER_MIN);
    assert(str_to_timeframe_sec("M15", seconds) && seconds == SEC_PER_15_MIN);
    assert(str_to_timeframe_sec("H1", seconds) && seconds == SEC_PER_HOUR);
    assert(str_to_timeframe_sec("H4", seconds) && seconds == SEC_PER_4_HOUR);
    assert(str_to_timeframe_sec("D1", seconds) && seconds == SEC_PER_DAY);
    assert(str_to_timeframe_sec("W1", seconds) && seconds == 7 * SEC_PER_DAY);
    assert(str_to_timeframe_sec("MN1", seconds) && seconds == 30 * SEC_PER_DAY);
    assert(str_to_timeframe_sec("Q1", seconds) && seconds == 90 * SEC_PER_DAY);
    assert(str_to_timeframe_sec("Y1", seconds) && seconds == SEC_PER_YEAR);

    assert(str_to_timeframe_sec("minute", seconds) && seconds == SEC_PER_MIN);
    assert(str_to_timeframe_sec("15 MIN", seconds) && seconds == SEC_PER_15_MIN);
    assert(str_to_timeframe_sec("hour", seconds) && seconds == SEC_PER_HOUR);
    assert(str_to_timeframe_sec("2 weeks", seconds) && seconds == 14 * SEC_PER_DAY);
    assert(str_to_timeframe_sec(" Month ", seconds) && seconds == 30 * SEC_PER_DAY);
    assert(str_to_timeframe_sec("15MIN", seconds) && seconds == SEC_PER_15_MIN);

    assert(str_to_timeframe_ms("M15", milliseconds) && milliseconds == MS_PER_15_MIN);
    assert(str_to_timeframe_ms("hour", milliseconds) && milliseconds == MS_PER_HOUR);
    assert(timeframe_sec("H1") == SEC_PER_HOUR);
    assert(timeframe_ms("2 weeks") == static_cast<ts_ms_t>(14) * MS_PER_DAY);

    const char* compact = "H4";
    assert(str_to_timeframe_sec(compact, seconds) && seconds == SEC_PER_4_HOUR);
    assert(timeframe_sec(compact) == SEC_PER_4_HOUR);

    assert(!str_to_timeframe_sec("", seconds));
    assert(!str_to_timeframe_sec("   ", seconds));
    assert(!str_to_timeframe_sec("M0", seconds));
    assert(!str_to_timeframe_sec("-H1", seconds));
    assert(!str_to_timeframe_sec("1.5h", seconds));
    assert(!str_to_timeframe_sec("1m", seconds));
    assert(!str_to_timeframe_sec("abc", seconds));
    assert(!str_to_timeframe_sec("S9223372036854775808", seconds));
    assert(!str_to_timeframe_ms("S9223372036854776", milliseconds));
    assert(timeframe_sec("abc") == 0);
    assert(timeframe_ms("abc") == 0);

#if __cplusplus >= 201703L
    const std::string_view compact_view("W1");
    const std::string_view word_view("minute");
    assert(str_to_timeframe_sec(compact_view, seconds) && seconds == 7 * SEC_PER_DAY);
    assert(str_to_timeframe_ms(word_view, milliseconds) && milliseconds == MS_PER_MIN);
    assert(timeframe_sec(compact_view) == 7 * SEC_PER_DAY);
    assert(timeframe_ms(word_view) == MS_PER_MIN);
#endif

    return 0;
}