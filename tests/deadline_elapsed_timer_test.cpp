#include <time_shield/DeadlineTimer.hpp>
#include <time_shield/ElapsedTimer.hpp>

#include "test_assert.hpp"
#include <chrono>
#include <cstdint>
#include <thread>

int main() {
    using namespace time_shield;

    // DeadlineTimer basic behavior.
    DeadlineTimer deadline;
    TIME_SHIELD_TEST_CHECK(!deadline.is_running());
    TIME_SHIELD_TEST_CHECK(!deadline.has_expired());
    TIME_SHIELD_TEST_CHECK(deadline.remaining_time() == DeadlineTimer::duration::zero());

    const auto start = DeadlineTimer::clock::now();
    const auto deadline_tp = start + std::chrono::milliseconds(50);
    deadline.start(deadline_tp);
    TIME_SHIELD_TEST_CHECK(deadline.is_running());
    TIME_SHIELD_TEST_CHECK(!deadline.has_expired(start));
    TIME_SHIELD_TEST_CHECK(deadline.remaining_time(start) == std::chrono::milliseconds(50));
    const auto expected_deadline_ms = std::chrono::duration_cast<std::chrono::milliseconds>(deadline_tp.time_since_epoch()).count();
    const auto expected_deadline_sec = std::chrono::duration_cast<std::chrono::seconds>(deadline_tp.time_since_epoch()).count();
    TIME_SHIELD_TEST_CHECK(deadline.deadline_ms() == expected_deadline_ms);
    TIME_SHIELD_TEST_CHECK(deadline.deadline_sec() == expected_deadline_sec);
    TIME_SHIELD_TEST_CHECK(deadline.remaining_time_ms() >= 0);
    TIME_SHIELD_TEST_CHECK(deadline.remaining_time_ms() <= 60);
    TIME_SHIELD_TEST_CHECK(deadline.remaining_time_sec() == 0);
    TIME_SHIELD_TEST_CHECK(!deadline.has_expired(start + std::chrono::milliseconds(25)));
    TIME_SHIELD_TEST_CHECK(deadline.has_expired(start + std::chrono::milliseconds(60)));
    TIME_SHIELD_TEST_CHECK(deadline.remaining_time(start + std::chrono::milliseconds(60)) == DeadlineTimer::duration::zero());
    const auto now_25_ms = std::chrono::duration_cast<std::chrono::milliseconds>((start + std::chrono::milliseconds(25)).time_since_epoch()).count();
    const auto now_60_ms = std::chrono::duration_cast<std::chrono::milliseconds>((start + std::chrono::milliseconds(60)).time_since_epoch()).count();
    const auto now_25_sec = std::chrono::duration_cast<std::chrono::seconds>((start + std::chrono::milliseconds(25)).time_since_epoch()).count();
    const auto now_2_sec = std::chrono::duration_cast<std::chrono::seconds>((start + std::chrono::seconds(2)).time_since_epoch()).count();
    TIME_SHIELD_TEST_CHECK(!deadline.has_expired_ms(static_cast<ts_ms_t>(now_25_ms)));
    TIME_SHIELD_TEST_CHECK(deadline.has_expired_ms(static_cast<ts_ms_t>(now_60_ms)));
    TIME_SHIELD_TEST_CHECK(!deadline.has_expired_sec(static_cast<ts_t>(now_25_sec)));
    TIME_SHIELD_TEST_CHECK(deadline.has_expired_sec(static_cast<ts_t>(now_2_sec)));

    deadline.set_forever();
    TIME_SHIELD_TEST_CHECK(deadline.is_running());
    TIME_SHIELD_TEST_CHECK(deadline.is_forever());
    TIME_SHIELD_TEST_CHECK(!deadline.has_expired());
    TIME_SHIELD_TEST_CHECK(deadline.deadline() == DeadlineTimer::time_point::max());

    deadline.stop();
    TIME_SHIELD_TEST_CHECK(!deadline.is_running());
    TIME_SHIELD_TEST_CHECK(deadline.remaining_time() == DeadlineTimer::duration::zero());

    deadline.start(std::chrono::milliseconds(-1));
    TIME_SHIELD_TEST_CHECK(deadline.is_running());
    TIME_SHIELD_TEST_CHECK(deadline.has_expired());

    const auto before_short = DeadlineTimer::clock::now();
    deadline.start(std::chrono::nanoseconds(1));
    TIME_SHIELD_TEST_CHECK(deadline.is_running());
    TIME_SHIELD_TEST_CHECK(!deadline.has_expired(before_short));

    auto factory = DeadlineTimer::from_timeout(std::chrono::milliseconds(20));
    TIME_SHIELD_TEST_CHECK(factory.is_running());
    TIME_SHIELD_TEST_CHECK(!factory.has_expired());
    TIME_SHIELD_TEST_CHECK(factory.remaining_time() > DeadlineTimer::duration::zero());

    factory.add(std::chrono::milliseconds(30));
    TIME_SHIELD_TEST_CHECK(factory.remaining_time() >= std::chrono::milliseconds(25));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    factory.add(std::chrono::milliseconds(10));
    TIME_SHIELD_TEST_CHECK(factory.remaining_time() >= std::chrono::milliseconds(5));

    factory.add(DeadlineTimer::duration::zero());
    TIME_SHIELD_TEST_CHECK(factory.is_running());

    DeadlineTimer expired = DeadlineTimer::from_timeout(std::chrono::milliseconds(-5));
    TIME_SHIELD_TEST_CHECK(expired.has_expired());
    expired.add(std::chrono::milliseconds(10));
    TIME_SHIELD_TEST_CHECK(expired.is_running());
    TIME_SHIELD_TEST_CHECK(!expired.has_expired());

    DeadlineTimer near_max(DeadlineTimer::time_point::max() - DeadlineTimer::duration(1));
    near_max.add(DeadlineTimer::duration::max());
    TIME_SHIELD_TEST_CHECK(near_max.deadline() == DeadlineTimer::time_point::max());

    DeadlineTimer seconds_factory = DeadlineTimer::from_timeout_sec(static_cast<ts_t>(1));
    TIME_SHIELD_TEST_CHECK(seconds_factory.is_running());
    seconds_factory.start_sec(static_cast<ts_t>(2));
    TIME_SHIELD_TEST_CHECK(seconds_factory.remaining_time() <= std::chrono::seconds(2));
    TIME_SHIELD_TEST_CHECK(seconds_factory.remaining_time_sec() <= static_cast<ts_t>(2));
    TIME_SHIELD_TEST_CHECK(seconds_factory.remaining_time_sec() >= static_cast<ts_t>(0));
    seconds_factory.add_sec(static_cast<ts_t>(1));
    TIME_SHIELD_TEST_CHECK(seconds_factory.remaining_time() >= std::chrono::seconds(1));
    TIME_SHIELD_TEST_CHECK(seconds_factory.remaining_time_sec() >= static_cast<ts_t>(1));

    DeadlineTimer ms_factory = DeadlineTimer::from_timeout_ms(static_cast<ts_ms_t>(50));
    TIME_SHIELD_TEST_CHECK(ms_factory.is_running());
    ms_factory.start_ms(static_cast<ts_ms_t>(100));
    TIME_SHIELD_TEST_CHECK(ms_factory.remaining_time() <= std::chrono::milliseconds(100));
    TIME_SHIELD_TEST_CHECK(ms_factory.remaining_time_ms() <= static_cast<ts_ms_t>(100));
    TIME_SHIELD_TEST_CHECK(ms_factory.remaining_time_ms() >= static_cast<ts_ms_t>(0));
    ms_factory.add_ms(static_cast<ts_ms_t>(10));
    TIME_SHIELD_TEST_CHECK(ms_factory.remaining_time() >= std::chrono::milliseconds(10));
    TIME_SHIELD_TEST_CHECK(ms_factory.remaining_time_ms() >= static_cast<ts_ms_t>(10));

    DeadlineTimer ms_ctor(static_cast<ts_ms_t>(25));
    TIME_SHIELD_TEST_CHECK(ms_ctor.is_running());
    TIME_SHIELD_TEST_CHECK(ms_ctor.remaining_time() <= std::chrono::milliseconds(25));

    // ElapsedTimer checks.
    ElapsedTimer elapsed;
    TIME_SHIELD_TEST_CHECK(!elapsed.is_running());
    TIME_SHIELD_TEST_CHECK(!elapsed.is_valid());
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed() == ElapsedTimer::duration::zero());
    TIME_SHIELD_TEST_CHECK(elapsed.ms_since_reference() == 0);

    elapsed.start();
    TIME_SHIELD_TEST_CHECK(elapsed.is_running());
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    const auto first_span = elapsed.elapsed();
    TIME_SHIELD_TEST_CHECK(first_span >= std::chrono::milliseconds(1));
    const auto first_ms = elapsed.elapsed_ms();
    TIME_SHIELD_TEST_CHECK(first_ms >= 1);
    const auto first_ns = elapsed.elapsed_ns();
    TIME_SHIELD_TEST_CHECK(first_ns >= static_cast<std::int64_t>(first_ms - 1) * 1000000);
    const auto first_count = elapsed.elapsed_count<std::chrono::milliseconds>();
    TIME_SHIELD_TEST_CHECK(first_count == first_ms);
    TIME_SHIELD_TEST_CHECK(!elapsed.has_expired(static_cast<ts_ms_t>(1000)));
    TIME_SHIELD_TEST_CHECK(elapsed.has_expired(static_cast<ts_ms_t>(0)));
    TIME_SHIELD_TEST_CHECK(!elapsed.has_expired_sec(static_cast<ts_t>(1)));
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_sec() >= static_cast<ts_t>(0));
    const auto reference_ms = elapsed.ms_since_reference();
    TIME_SHIELD_TEST_CHECK(reference_ms == std::chrono::duration_cast<std::chrono::milliseconds>(elapsed.start_time().time_since_epoch()).count());

    const auto snapshot = ElapsedTimer::clock::now();
    const auto snapshot_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(snapshot.time_since_epoch()).count();
    const auto snapshot_ms = std::chrono::duration_cast<std::chrono::milliseconds>(snapshot.time_since_epoch()).count();
    const auto snapshot_sec = std::chrono::duration_cast<std::chrono::seconds>(snapshot.time_since_epoch()).count();
    const auto truncated_snapshot_ns = ElapsedTimer::time_point(std::chrono::duration_cast<ElapsedTimer::duration>(std::chrono::nanoseconds(snapshot_ns)));
    const auto truncated_snapshot_ms = ElapsedTimer::time_point(std::chrono::duration_cast<ElapsedTimer::duration>(std::chrono::milliseconds(snapshot_ms)));
    const auto truncated_snapshot_sec = ElapsedTimer::time_point(std::chrono::duration_cast<ElapsedTimer::duration>(std::chrono::seconds(snapshot_sec)));
    const auto expected_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(truncated_snapshot_ns - elapsed.start_time()).count();
    const auto expected_ms = std::chrono::duration_cast<std::chrono::milliseconds>(truncated_snapshot_ms - elapsed.start_time()).count();
    const auto expected_sec = std::chrono::duration_cast<std::chrono::seconds>(truncated_snapshot_sec - elapsed.start_time()).count();
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_ns(snapshot_ns) == expected_ns);
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_ms(snapshot_ms) == expected_ms);
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_sec(snapshot_sec) == expected_sec);

    const auto restarted_ms = elapsed.restart_ms(snapshot_ms);
    TIME_SHIELD_TEST_CHECK(restarted_ms == expected_ms);
    TIME_SHIELD_TEST_CHECK(elapsed.is_running());
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_ms(snapshot_ms) == 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    const auto after_ms_restart = ElapsedTimer::clock::now();
    const auto after_ms_restart_sec = std::chrono::duration_cast<std::chrono::seconds>(after_ms_restart.time_since_epoch()).count();
    const auto truncated_after_sec = ElapsedTimer::time_point(std::chrono::duration_cast<ElapsedTimer::duration>(std::chrono::seconds(after_ms_restart_sec)));
    const auto expected_after_sec = std::chrono::duration_cast<std::chrono::seconds>(truncated_after_sec - elapsed.start_time()).count();
    const auto restarted_sec = elapsed.restart_sec(after_ms_restart_sec);
    TIME_SHIELD_TEST_CHECK(restarted_sec == expected_after_sec);
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_sec(after_ms_restart_sec) == 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    const auto delta = elapsed.restart();
    
    // After restart_sec(), start_time can be quantized to a second boundary,
    // so delta may be smaller than first_span. Just ensure it is non-negative
    // and reasonably bounded.
    TIME_SHIELD_TEST_CHECK(delta >= ElapsedTimer::duration::zero());
    TIME_SHIELD_TEST_CHECK(delta < std::chrono::seconds(2));

    const auto after_restart = elapsed.elapsed();
    TIME_SHIELD_TEST_CHECK(after_restart < std::chrono::milliseconds(200));
    TIME_SHIELD_TEST_CHECK(!elapsed.has_expired(static_cast<ts_ms_t>(50)));
    TIME_SHIELD_TEST_CHECK(!elapsed.has_expired_sec(static_cast<ts_t>(1)));

    elapsed.invalidate();
    TIME_SHIELD_TEST_CHECK(!elapsed.is_running());
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed() == ElapsedTimer::duration::zero());
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_ms() == 0);
    TIME_SHIELD_TEST_CHECK(elapsed.elapsed_ns() == 0);
    TIME_SHIELD_TEST_CHECK(!elapsed.has_expired(static_cast<ts_ms_t>(1)));
    TIME_SHIELD_TEST_CHECK(!elapsed.has_expired_sec(static_cast<ts_t>(1)));
    TIME_SHIELD_TEST_CHECK(elapsed.ms_since_reference() == 0);

    ElapsedTimer autostart(true);
    TIME_SHIELD_TEST_CHECK(autostart.is_running());
    TIME_SHIELD_TEST_CHECK(autostart.elapsed() >= ElapsedTimer::duration::zero());

    (void)expected_deadline_ms;
    (void)expected_deadline_sec;
    (void)now_25_ms;
    (void)now_60_ms;
    (void)now_25_sec;
    (void)now_2_sec;
    (void)first_ms;
    (void)first_ns;
    (void)first_count;
    (void)reference_ms;
    (void)expected_ns;
    (void)expected_ms;
    (void)expected_sec;
    (void)restarted_ms;
    (void)expected_after_sec;
    (void)restarted_sec;

    return 0;
}
