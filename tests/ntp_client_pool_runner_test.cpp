#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include <time_shield/ntp_client_pool_runner.hpp>

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

using namespace time_shield;

struct FakePool {
    std::atomic<uint64_t> m_measure_calls{0};
    std::atomic<int64_t> m_off_us{0};

    FakePool() = default;
    FakePool(const FakePool&) = delete;
    FakePool& operator=(const FakePool&) = delete;

    FakePool(FakePool&& other) noexcept {
        m_measure_calls.store(other.m_measure_calls.load());
        m_off_us.store(other.m_off_us.load());
    }

    FakePool& operator=(FakePool&& other) noexcept {
        if (this != &other) {
            m_measure_calls.store(other.m_measure_calls.load());
            m_off_us.store(other.m_off_us.load());
        }
        return *this;
    }

    bool measure() {
        const uint64_t calls = m_measure_calls.fetch_add(1) + 1;
        m_off_us.store(static_cast<int64_t>(calls * 1000));
        return true;
    }

    int64_t offset_us() const noexcept { return m_off_us.load(); }
    int64_t utc_time_us() const noexcept { return m_off_us.load(); }
    int64_t utc_time_ms() const noexcept { return m_off_us.load() / 1000; }
    std::vector<NtpSample> last_samples() const { return {}; }
};

int main() {
    using Runner = BasicPoolRunner<FakePool>;

    {
        Runner runner;
        assert(runner.start(std::chrono::milliseconds(20), true));

        const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        while (runner.measure_count() < 3 && std::chrono::steady_clock::now() < deadline) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        assert(runner.running());
        assert(runner.last_measure_ok());
        assert(runner.measure_count() >= 3);
        assert(runner.offset_us() > 0);

        const uint64_t before = runner.measure_count();
        runner.stop();
        const uint64_t after = runner.measure_count();
        assert(before == after);
        assert(!runner.running());
    }

    {
        Runner runner;
        assert(runner.start(std::chrono::milliseconds(500), false));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        assert(runner.measure_count() == 0);

        assert(runner.force_measure());

        const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(200);
        while (runner.measure_count() < 1 && std::chrono::steady_clock::now() < deadline) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        runner.stop();
        runner.stop();
    }

    return 0;
}

#else
int main() {
    return 0;
}
#endif
