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
    assert(to_iso8601(ts_t(1), 3 * SEC_PER_HOUR) == "1970-01-01T03:00:01+03:00");
    assert(to_iso8601_ms(ts_ms_t(1500), -2 * SEC_PER_HOUR) == "1969-12-31T22:00:01.500-02:00");
    assert(to_human_readable(ts_t(1718973296)) == "2024-06-21 12:34:56");
    assert(to_human_readable_ms(ts_ms_t(1718973296789)) == "2024-06-21 12:34:56.789");
    assert(to_string("%G-%V-%u", to_timestamp(2025, 12, 16)) == "2025-51-2");
    assert(to_string("%g-W%V", to_timestamp(2025, 12, 16)) == "25-W51");

    return 0;
}
