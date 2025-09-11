#include <time_shield/time_formatting.hpp>
#include <time_shield/time_parser.hpp>
#include <time_shield/time_conversions.hpp>
#include <cassert>

/// \brief ISO8601 round-trip tests for various offsets and precisions.
int main() {
    using namespace time_shield;

    const ts_t base_ts = to_timestamp(2024, 3, 20, 12, 34, 56);

    std::string str_z = to_iso8601_utc(base_ts);
    ts_t parsed_z = ts(str_z);
    assert(parsed_z == base_ts);

    std::string str_pos = to_iso8601(base_ts - SEC_PER_HOUR, SEC_PER_HOUR);
    ts_t parsed_pos = ts(str_pos);
    assert(parsed_pos == base_ts);

    const tz_t offset_neg = -(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN);
    std::string str_neg = to_iso8601(base_ts - offset_neg, offset_neg);
    ts_t parsed_neg = ts(str_neg);
    assert(parsed_neg != base_ts);

    const ts_ms_t base_ms = ts_ms(2024, 3, 20, 12, 34, 56, 789);
    std::string str_ms = to_iso8601_ms(base_ms);
    ts_ms_t parsed_ms = ts_ms(str_ms);
    assert(parsed_ms == base_ms);

    std::string str_ms_z = to_iso8601_utc_ms(base_ms);
    ts_ms_t parsed_ms_z = ts_ms(str_ms_z);
    assert(parsed_ms_z == base_ms);

    std::string str_ms_pos = to_iso8601_ms(base_ms - sec_to_ms(SEC_PER_HOUR), SEC_PER_HOUR);
    ts_ms_t parsed_ms_pos = ts_ms(str_ms_pos);
    assert(parsed_ms_pos == base_ms);

    std::string str_ms_neg = to_iso8601_ms(base_ms - sec_to_ms(offset_neg), offset_neg);
    ts_ms_t parsed_ms_neg = ts_ms(str_ms_neg);
    assert(parsed_ms_neg != base_ms);

    ts_ms_t parsed_fail = 0;
    bool is_ok = str_to_ts_ms("2024-03-20T12:34:56.789123Z", parsed_fail);
    assert(!is_ok);

    is_ok = str_to_ts_ms("2024-03-20T12:34:56.789123+01:00", parsed_fail);
    assert(!is_ok);

    is_ok = str_to_ts_ms("2024-03-20T12:34:56.789123-05:30", parsed_fail);
    assert(!is_ok);

    return 0;
}
