#include <time_shield/time_utils.hpp>
#include <cassert>

/// \brief Basic checks for time utility helpers.
int main() {
    using namespace time_shield;

    int ns = ns_of_sec();
    assert(ns >= 0 && ns < NS_PER_SEC);

    int us = us_of_sec();
    assert(us >= 0 && us < US_PER_SEC);

    int ms = ms_of_sec();
    assert(ms >= 0 && ms < MS_PER_SEC);

    ts_ms_t t1 = ts_ms();
    ts_ms_t t2 = timestamp_ms();
    assert(t2 >= t1 && t2 - t1 < MS_PER_SEC);

    ts_us_t u1 = ts_us();
    ts_us_t u2 = timestamp_us();
    assert(u2 >= u1 && u2 - u1 < US_PER_SEC);

    return 0;
}
