#include <time_shield/validation.hpp>
#include <time_shield/time_parser.hpp>
#include <cassert>
#include <string>

/// \brief Validates the is_workday overload set.
int main() {
    using namespace time_shield;

    const ts_t weekday_ts = 1710720000; // 2024-03-18 (Monday)
    const ts_t weekend_ts = 1710547200; // 2024-03-16 (Saturday)

    assert(is_workday(weekday_ts));
    assert(!is_workday(weekend_ts));

    const ts_ms_t weekday_ms = weekday_ts * MS_PER_SEC;
    const ts_ms_t weekend_ms = weekend_ts * MS_PER_SEC + 500;

    assert(is_workday_ms(weekday_ms));
    assert(!is_workday_ms(weekend_ms));

    assert(is_workday(2024, 3, 18));
    assert(!is_workday(2024, 3, 16));

    const std::string weekday_iso = "2024-03-18T00:00:00Z";
    const std::string weekend_iso = "2024-03-16T00:00:00Z";
    const std::string weekday_iso_ms = "2024-03-18T00:00:00.500Z";
    const std::string weekend_iso_ms = "2024-03-16T00:00:00.500Z";

    assert(is_workday(weekday_iso));
    assert(!is_workday(weekend_iso));
    assert(is_workday_ms(weekday_iso_ms));
    assert(!is_workday_ms(weekend_iso_ms));

    assert(!is_workday("not-a-date"));
    assert(!is_workday_ms("2024-13-40T00:00:00.000Z"));

    return 0;
}
