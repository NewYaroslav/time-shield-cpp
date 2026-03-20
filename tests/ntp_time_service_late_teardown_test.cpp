#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#define TIME_SHIELD_TEST_FAKE_NTP
#include <time_shield/ntp_time_service.hpp>

#include <cassert>
#include <chrono>

using namespace time_shield;

int main() {
    auto& service = NtpTimeService::instance();
    service.shutdown();

    auto& same_service = NtpTimeService::instance();
    assert(&service == &same_service);
    assert(detail::NtpTimeServiceTestAccess<detail::FakeNtpRunner>::atexit_registration_count() == 1);

    assert(service.init(std::chrono::milliseconds(10), true));
    const int64_t cached_offset = service.offset_us();
    assert(cached_offset > 0);
    assert(service.running());

    detail::NtpTimeServiceTestAccess<detail::FakeNtpRunner>::begin_process_shutdown();

    assert(detail::NtpTimeServiceTestAccess<detail::FakeNtpRunner>::is_process_shutting_down());
    assert(!service.running());
    assert(!service.init(std::chrono::milliseconds(10), true));
    assert(service.offset_us() == cached_offset);

    const int64_t utc_before = service.utc_time_us();
    const int64_t utc_after = service.utc_time_us();
    assert(utc_after >= utc_before);
    assert(!service.running());
    return 0;
}

#else
int main() {
    return 0;
}
#endif
