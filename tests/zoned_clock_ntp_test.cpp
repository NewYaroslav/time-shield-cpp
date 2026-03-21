#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ZonedClock.hpp>

#include <cassert>
#include <cstdlib>
#include <cstdint>

/// \brief Tests ZonedClock lazy NTP startup with the fake NTP runner.
int main() {
    using namespace time_shield;

    NtpTimeService::instance().shutdown();

    ZonedClock clock(static_cast<tz_t>(SEC_PER_HOUR), true);
    assert(clock.use_ntp());
    assert(!clock.ntp_active());

    const ts_us_t realtime_before = static_cast<ts_us_t>(now_realtime_us());
    const ts_us_t utc_us = clock.utc_time_us();
    const ts_us_t realtime_after = static_cast<ts_us_t>(now_realtime_us());

    assert(clock.ntp_active());
    assert(NtpTimeService::instance().measure_count() >= 1);
    assert(NtpTimeService::instance().offset_us() >= 1000);
    assert(utc_us > realtime_before);
    assert(utc_us - realtime_before >= 500);
    assert(utc_us <= realtime_after + static_cast<ts_us_t>(NtpTimeService::instance().offset_us()) + 100000);

    const ts_ms_t utc_ms = clock.utc_time_ms();
    const ts_ms_t local_ms = clock.local_time_ms();
    const ts_ms_t expected_delta_ms = static_cast<ts_ms_t>(SEC_PER_HOUR) * MS_PER_SEC;
    assert(std::llabs((local_ms - utc_ms) - expected_delta_ms) <= 50);

    const DateTime snapshot = clock.now();
    assert(snapshot.utc_offset() == SEC_PER_HOUR);

    NtpTimeService::instance().shutdown();
    return EXIT_SUCCESS;
}

#else

int main() {
    return 0;
}

#endif
