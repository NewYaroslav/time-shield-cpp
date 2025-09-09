#include <time_shield/time_zone_struct.hpp>
#include <cassert>

/// \brief Basic checks for time zone conversion helpers.
int main() {
    using namespace time_shield;

    TimeZoneStruct tz = create_time_zone_struct(3, 0, true);
    assert(time_zone_struct_to_offset(tz) == 3 * SEC_PER_HOUR);

    tz = create_time_zone_struct(5, 30, false);
    assert(time_zone_struct_to_offset(tz) == -(5 * SEC_PER_HOUR + 30 * SEC_PER_MIN));

    return 0;
}
