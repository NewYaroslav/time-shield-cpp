#include <time_shield/time_formatting.hpp>
#include <cassert>

/// \brief Basic checks for time formatting helpers.
int main() {
    using namespace time_shield;

    assert(to_iso8601(ts_t(1)) == "1970-01-01T00:00:01");
    assert(to_iso8601_date(ts_t(0)) == "1970-01-01");
    assert(to_iso8601_time_utc(ts_t(1)) == "00:00:01Z");
    assert(to_iso8601_utc(ts_t(1)) == "1970-01-01T00:00:01Z");
    assert(to_iso8601_ms(ts_ms_t(1500)) == "1970-01-01T00:00:01.500");
    assert(to_iso8601(ts_t(1), 3 * SEC_PER_HOUR) == "1970-01-01T00:00:01+03:00");
    assert(to_iso8601_ms(ts_ms_t(1500), -2 * SEC_PER_HOUR) == "1970-01-01T00:00:01.500-02:00");

    return 0;
}
