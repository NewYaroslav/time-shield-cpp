#include <time_shield/time_conversions.hpp>
#include <cassert>

/// \brief Basic checks for time conversion helpers.
int main() {
    using namespace time_shield;

    assert(get_unix_day_ms(86400000) == 1);
    assert(unix_day_to_timestamp(2) == 2 * SEC_PER_DAY);
    assert(unix_day_to_timestamp_ms(2) == 2 * MS_PER_DAY);
    assert(end_of_day_from_unix_day(0) == SEC_PER_DAY - 1);
    assert(start_of_next_day_from_unix_day(0) == SEC_PER_DAY);

    assert(start_of_min(61) == 60);
    assert(end_of_min(60) == 119);

    return 0;
}
