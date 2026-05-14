#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ntp_time_service.hpp>

#include "test_assert.hpp"
#include <chrono>

using namespace time_shield;

int main() {
    auto& service = NtpTimeService::instance();
    service.shutdown();

    auto& same_service = NtpTimeService::instance();
    TIME_SHIELD_TEST_CHECK(&service == &same_service);
    TIME_SHIELD_TEST_CHECK(detail::NtpTimeServiceTestAccess<detail::FakeNtpRunner>::atexit_registration_count() == 1);

    TIME_SHIELD_TEST_CHECK(service.init(std::chrono::milliseconds(10), true));
    const int64_t cached_offset = service.offset_us();
    TIME_SHIELD_TEST_CHECK(cached_offset > 0);
    TIME_SHIELD_TEST_CHECK(service.running());

    detail::NtpTimeServiceTestAccess<detail::FakeNtpRunner>::begin_process_shutdown();

    TIME_SHIELD_TEST_CHECK(detail::NtpTimeServiceTestAccess<detail::FakeNtpRunner>::is_process_shutting_down());
    TIME_SHIELD_TEST_CHECK(!service.running());
    TIME_SHIELD_TEST_CHECK(!service.init(std::chrono::milliseconds(10), true));
    TIME_SHIELD_TEST_CHECK(service.offset_us() == cached_offset);

    const int64_t utc_before = service.utc_time_us();
    const int64_t utc_after = service.utc_time_us();
    TIME_SHIELD_TEST_CHECK(utc_after >= utc_before);
    TIME_SHIELD_TEST_CHECK(!service.running());
    return 0;
}

#else
int main() {
    return 0;
}
#endif
