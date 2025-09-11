#include <time_shield/time_parser.hpp>
#include <cassert>

/// \brief Tests for leap years, month/year transitions, and time-of-day extremes.
int main() {
    using namespace time_shield;

    ts_t feb29 = ts("2024-02-29T23:59:59Z");
    ts_t march1 = ts("2024-03-01T00:00:00Z");
    assert(feb29 + 1 == march1);

    ts_t feb28 = ts("2023-02-28T23:59:59Z");
    ts_t march1_2023 = ts("2023-03-01T00:00:00Z");
    assert(feb28 + 1 == march1_2023);

    ts_t apr_end = ts("2023-04-30T23:59:59Z");
    ts_t may_start = ts("2023-05-01T00:00:00Z");
    assert(apr_end + 1 == may_start);

    ts_t dec_start = ts("2023-12-31T00:00:00Z");
    ts_t dec_end = ts("2023-12-31T23:59:59Z");
    ts_t jan_start = ts("2024-01-01T00:00:00Z");
    ts_t jan_end = ts("2024-01-01T23:59:59Z");
    assert(dec_end + 1 == jan_start);
    assert(dec_start + SEC_PER_DAY == jan_start);
    assert(dec_end + SEC_PER_DAY == jan_end);

    ts_t start_day = ts("2020-01-01T00:00:00Z");
    ts_t end_day = ts("2020-01-01T23:59:59Z");
    assert(end_day - start_day == SEC_PER_DAY - 1);

    ts_ms_t frac_ms = ts_ms("2020-01-01T00:00:00.123Z");
    ts_ms_t start_ms = ts_ms("2020-01-01T00:00:00Z");
    assert(frac_ms - start_ms == 123);

    return 0;
}
