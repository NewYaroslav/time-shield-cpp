#include <time_shield/time_parser.hpp>
#include <cassert>

/// \brief Negative parsing tests for malformed ISO8601 strings.
int main() {
    using namespace time_shield;

    ts_t parsed = 0;
    bool is_valid = str_to_ts("2023-13-01T00:00:00Z", parsed);
    assert(!is_valid);

    is_valid = str_to_ts("2023-02-30T00:00:00Z", parsed);
    assert(!is_valid);

    is_valid = str_to_ts("2023-01-01T24:00:00Z", parsed);
    assert(!is_valid);

    is_valid = str_to_ts("2023-01-01T00:00:00+25:00", parsed);
    assert(!is_valid);

    is_valid = str_to_ts("not a date", parsed);
    assert(!is_valid);

    ts_ms_t ms_parsed = 0;
    is_valid = str_to_ts_ms("2024-03-20T12:34:56.789123Z", ms_parsed);
    assert(!is_valid);

    is_valid = str_to_ts_ms("2024-3-20T12:34:56Z", ms_parsed);
    assert(!is_valid);

    is_valid = str_to_ts_ms("2024-03-2T12:34:56Z", ms_parsed);
    assert(!is_valid);

    return 0;
}
