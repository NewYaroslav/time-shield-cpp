#include <time_shield/TimerScheduler.hpp>

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

/// \brief Basic behavioral checks for TimerScheduler and Timer classes.
int main() {
    using namespace time_shield;

    TimerScheduler scheduler;

    // Single shot timer processed via process().
    Timer single_timer(scheduler);
    std::atomic<int> single_counter{0};
    single_timer.set_single_shot(true);
    single_timer.set_callback([&single_counter]() { single_counter.fetch_add(1); });
    const auto start_time = std::chrono::steady_clock::now();
    single_timer.start(std::chrono::seconds(1));
    while (single_counter.load() == 0) {
        scheduler.process();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    const auto elapsed = std::chrono::steady_clock::now() - start_time;
    assert(elapsed >= std::chrono::milliseconds(900));
    assert(elapsed < std::chrono::seconds(2));
    assert(single_counter.load() == 1);
    assert(!single_timer.is_active());

    // Repeating timer stopped from callback.
    Timer repeating_timer(scheduler);
    std::atomic<int> repeating_counter{0};
    repeating_timer.set_callback([&repeating_counter, &repeating_timer]() {
        const int value = repeating_counter.fetch_add(1) + 1;
        if (value >= 3) {
            repeating_timer.stop();
        }
    });
    repeating_timer.start(std::chrono::milliseconds(0));
    for (int i = 0; i < 5 && repeating_counter.load() < 3; ++i) {
        scheduler.process();
    }
    assert(repeating_counter.load() >= 3);

    // Worker thread driven timer with stop_and_wait().
    Timer worker_timer(scheduler);
    std::atomic<int> worker_counter{0};
    worker_timer.set_single_shot(true);
    worker_timer.set_callback([&worker_counter]() {
        worker_counter.fetch_add(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    });
    scheduler.run();
    worker_timer.start(std::chrono::milliseconds(10));
    for (int i = 0; i < 50 && !worker_timer.is_running(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    worker_timer.stop_and_wait();
    scheduler.stop();
    assert(worker_counter.load() == 1);

    // Static single shot helper.
    std::atomic<int> helper_counter{0};
    Timer::single_shot(scheduler, std::chrono::milliseconds(0), [&helper_counter]() {
        helper_counter.fetch_add(1);
    });
    scheduler.process();
    assert(helper_counter.load() == 1);

    return 0;
}
