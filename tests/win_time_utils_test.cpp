#include <time_shield/time_utils.hpp>
#include <cassert>

/// \brief Windows specific checks for high resolution timers.
int main() {
#ifdef _WIN32
    using namespace time_shield;
    const int64_t start = now_realtime_us();
    const int64_t end = now_realtime_us();
    assert(end >= start);
    (void)start;
    (void)end;
#endif
    return 0;
}
