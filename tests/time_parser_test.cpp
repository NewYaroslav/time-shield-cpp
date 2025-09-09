#include <time_shield/time_parser.hpp>
#include <cassert>

/// \brief Basic checks for time parsing helpers.
int main() {
    using namespace time_shield;

    assert(get_month_number("March") == MAR);

    Month m = JAN;
    assert(try_get_month_number("Aug", m) && m == AUG);

    int sec = 0;
    assert(sec_of_day("01:02:03", sec));
    assert(sec == SEC_PER_HOUR + 2 * SEC_PER_MIN + 3);
    assert(sec_of_day("01:02") == SEC_PER_HOUR + 2 * SEC_PER_MIN);

    assert(ts("1970-01-01T00:00:00Z") == 0);
    assert(ts_ms("1970-01-01T00:00:01.500Z") == 1500);

    return 0;
}
