// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_TIMER_SCHEDULER_HPP_INCLUDED
#define _TIME_SHIELD_TIMER_SCHEDULER_HPP_INCLUDED

/// \file TimerScheduler.hpp
/// \brief Timer scheduler that provides Qt-like timer functionality.
///
/// TimerScheduler manages timers that can be processed either by a dedicated
/// worker thread or manually via process/update calls. Timers are rescheduled
/// using fixed-rate semantics, meaning the next activation time is based on the
/// previously scheduled fire time. Cancelled timers are removed lazily from the
/// internal queue, which can temporarily increase the queue size under frequent
/// start/stop cycles.

#include "config.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

namespace time_shield {

    class TimerScheduler;
    class Timer;

    namespace detail {

        using TimerClock = std::chrono::steady_clock;
        using TimerCallback = std::function<void()>;

        /// \brief Internal state shared between Timer and TimerScheduler.
        struct TimerState {
            TimerScheduler*            m_scheduler = nullptr;
            std::mutex                 m_callback_mutex;
            TimerCallback              m_callback;
            std::atomic<std::int64_t>  m_interval_ms{0};
            std::atomic<bool>          m_is_single_shot{false};
            std::atomic<bool>          m_is_active{false};
            std::atomic<bool>          m_is_running{false};
            std::size_t                m_id{0};
            std::atomic<std::uint64_t> m_generation{0};
            std::atomic<bool>          m_has_external_owner{false};
        };

        inline TimerState*& current_timer_state() {
            static thread_local TimerState* state = nullptr;
            return state;
        }

        struct RunningTimerScope {
            explicit RunningTimerScope(TimerState* state)
                : m_previous(current_timer_state()) {
                current_timer_state() = state;
            }

            ~RunningTimerScope() {
                current_timer_state() = m_previous;
            }

        private:
            TimerState* m_previous;
        };

        /// \brief Data stored in the priority queue of scheduled timers.
        struct ScheduledTimer {
            ScheduledTimer() = default;

            ScheduledTimer(TimerClock::time_point fire_time, std::size_t timer_id, std::uint64_t generation)
                : m_fire_time(fire_time), m_timer_id(timer_id), m_generation(generation) {}

            TimerClock::time_point m_fire_time{};
            std::size_t            m_timer_id{0};
            std::uint64_t          m_generation{0};
        };

        /// \brief Comparator that orders timers by earliest fire time.
        struct ScheduledComparator {
            bool operator()(const ScheduledTimer& lhs, const ScheduledTimer& rhs) const {
                return lhs.m_fire_time > rhs.m_fire_time;
            }
        };

        /// \brief Helper structure that represents a timer ready to run.
        struct DueTimer {
            DueTimer() = default;

            DueTimer(TimerClock::time_point fire_time,
                     std::uint64_t generation,
                     std::shared_ptr<TimerState> state)
                : m_fire_time(fire_time), m_generation(generation), m_state(std::move(state)) {}

            TimerClock::time_point             m_fire_time{};
            std::uint64_t                      m_generation{0};
            std::shared_ptr<TimerState>        m_state;
        };

    } // namespace detail

    using timer_state_ptr = std::shared_ptr<detail::TimerState>;

    /// \brief Scheduler that manages timer execution.
    class TimerScheduler {
    public:
        using clock = detail::TimerClock;

        TimerScheduler();
        ~TimerScheduler();

        TimerScheduler(const TimerScheduler&) = delete;
        TimerScheduler& operator=(const TimerScheduler&) = delete;
        TimerScheduler(TimerScheduler&&) = delete;
        TimerScheduler& operator=(TimerScheduler&&) = delete;

        /// \brief Starts a dedicated worker thread that processes timers.
        ///
        /// This method is non-blocking. It spawns a background thread that
        /// waits for timers to fire and executes their callbacks. While the
        /// worker thread is active, manual processing via process() or update()
        /// must not be used.
        void run();

        /// \brief Requests the worker thread to stop and waits for it to exit.
        void stop();

        /// \brief Processes all timers that are ready to fire at the moment of the call.
        ///
        /// The method is non-blocking: it does not wait for future timers.
        /// It must not be called while the worker thread started by run() is
        /// active.
        void process();

        /// \brief Alias for process() for compatibility with update-based loops.
        void update();

        /// \brief Returns number of timer states that are still alive.
        ///
        /// Method is intended for tests to verify resource cleanup.
        std::size_t active_timer_count_for_testing();

    private:
        friend class Timer;

        timer_state_ptr create_timer_state();
        void destroy_timer_state(const timer_state_ptr& state);
        void start_timer(const timer_state_ptr& state, clock::time_point when);
        void stop_timer(const timer_state_ptr& state);

        void worker_loop();
        void collect_due_timers_locked(std::vector<detail::DueTimer>& due, clock::time_point now);
        void execute_due_timers(std::vector<detail::DueTimer>& due);
        void finalize_timer(const detail::DueTimer& due_timer);

        std::mutex                                                                 m_mutex;
        std::condition_variable                                                    m_cv;
        std::thread                                                                m_thread;
        bool                                                                       m_is_worker_running{false};
        bool                                                                       m_stop_requested{false};
        std::priority_queue<detail::ScheduledTimer, std::vector<detail::ScheduledTimer>, detail::ScheduledComparator> m_queue;
        std::unordered_map<std::size_t, std::weak_ptr<detail::TimerState>>         m_timers;
        std::size_t                                                                m_next_id{1};
    };

    /// \brief Timer that mimics the behavior of Qt timers.
    class Timer {
    public:
        using Callback = detail::TimerCallback;

        explicit Timer(TimerScheduler& scheduler);
        ~Timer();

        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
        Timer(Timer&&) = delete;
        Timer& operator=(Timer&&) = delete;

        /// \brief Sets the interval used by the timer.
        ///
        /// Negative durations are clamped to zero. An interval of zero means
        /// the timer is rescheduled immediately after firing.
        template<class Rep, class Period>
        void set_interval(std::chrono::duration<Rep, Period> interval) noexcept;

        /// \brief Returns the currently configured interval.
        std::chrono::milliseconds interval() const noexcept;

        /// \brief Starts the timer using the previously configured interval.
        void start();

        /// \brief Starts the timer with the specified interval.
        template<class Rep, class Period>
        void start(std::chrono::duration<Rep, Period> interval);

        /// \brief Stops the timer.
        ///
        /// The operation is non-blocking: the method does not wait for a
        /// running callback to finish. Use stop_and_wait() to synchronously
        /// wait for completion.
        void stop();

        /// \brief Stops the timer and waits until an active callback finishes.
        ///
        /// Must not be called from inside the timer callback itself.
        void stop_and_wait();

        /// \brief Sets whether the timer should fire only once.
        void set_single_shot(bool is_single_shot) noexcept;

        /// \brief Returns true if the timer fires only once.
        bool is_single_shot() const noexcept;

        /// \brief Returns true if the timer is active.
        bool is_active() const noexcept;

        /// \brief Returns true if the timer callback is being executed.
        bool is_running() const noexcept;

        /// \brief Sets the callback that should be invoked when the timer fires.
        void set_callback(Callback callback);

        /// \brief Creates a single-shot timer that invokes the callback once.
        ///
        /// The helper keeps the timer alive until the callback finishes.
        template<class Rep, class Period>
        static void single_shot(TimerScheduler& scheduler,
                                std::chrono::duration<Rep, Period> interval,
                                Callback callback);

    private:
        TimerScheduler&               m_scheduler;
        timer_state_ptr               m_state;
    };

    // ---------------------------------------------------------------------
    // TimerScheduler inline implementation
    // ---------------------------------------------------------------------

    inline TimerScheduler::TimerScheduler() = default;

    inline TimerScheduler::~TimerScheduler() {
        stop();
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& entry : m_timers) {
            if (auto state = entry.second.lock()) {
                std::lock_guard<std::mutex> callback_lock(state->m_callback_mutex);
                state->m_callback = {};
            }
        }
        m_timers.clear();
        while (!m_queue.empty()) {
            m_queue.pop();
        }
    }

    inline void TimerScheduler::run() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_is_worker_running) {
            return;
        }
        m_stop_requested = false;
        m_is_worker_running = true;
        m_thread = std::thread(&TimerScheduler::worker_loop, this);
    }

    inline void TimerScheduler::stop() {
        std::vector<timer_state_ptr> orphan_states;
        std::thread                 worker_to_join;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_is_worker_running) {
                m_stop_requested = true;
                m_cv.notify_all();
                worker_to_join = std::move(m_thread);
            } else {
                m_stop_requested = false;
            }

            for (auto it = m_timers.begin(); it != m_timers.end();) {
                auto state = it->second.lock();
                if (!state) {
                    it = m_timers.erase(it);
                    continue;
                }

                if (!state->m_has_external_owner.load(std::memory_order_relaxed)) {
                    orphan_states.push_back(state);
                    it = m_timers.erase(it);
                } else {
                    ++it;
                }
            }
        }

        if (worker_to_join.joinable()) {
            worker_to_join.join();
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_is_worker_running = false;
            m_stop_requested = false;
        }

        for (auto& state : orphan_states) {
            if (!state) {
                continue;
            }
            std::lock_guard<std::mutex> callback_lock(state->m_callback_mutex);
            state->m_callback = {};
            state->m_is_active.store(false, std::memory_order_relaxed);
        }
    }

    inline void TimerScheduler::process() {
        std::vector<detail::DueTimer> due;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            assert(!m_is_worker_running && "process() must not be called while the worker thread is active");
            const auto now = clock::now();
            collect_due_timers_locked(due, now);
        }
        execute_due_timers(due);
    }

    inline void TimerScheduler::update() {
        process();
    }

    inline std::size_t TimerScheduler::active_timer_count_for_testing() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::size_t count = 0;
        for (const auto& entry : m_timers) {
            if (!entry.second.expired()) {
                ++count;
            }
        }
        return count;
    }

    inline timer_state_ptr TimerScheduler::create_timer_state() {
        auto state = std::make_shared<detail::TimerState>();
        state->m_scheduler = this;
        std::lock_guard<std::mutex> lock(m_mutex);
        state->m_id = m_next_id++;
        m_timers[state->m_id] = state;
        return state;
    }

    inline void TimerScheduler::destroy_timer_state(const timer_state_ptr& state) {
        if (!state) {
            return;
        }
        {
            std::lock_guard<std::mutex> callback_lock(state->m_callback_mutex);
            state->m_callback = {};
        }
        std::lock_guard<std::mutex> lock(m_mutex);
        state->m_is_active.store(false, std::memory_order_relaxed);
        state->m_generation.fetch_add(1, std::memory_order_relaxed);
        if (state->m_id != 0) {
            m_timers.erase(state->m_id);
        }
        state->m_scheduler = nullptr;
    }

    inline void TimerScheduler::start_timer(const timer_state_ptr& state, clock::time_point when) {
        if (!state) {
            return;
        }
        std::lock_guard<std::mutex> lock(m_mutex);
        state->m_is_active.store(true, std::memory_order_relaxed);
        const auto generation = state->m_generation.fetch_add(1, std::memory_order_relaxed) + 1;
        m_queue.push(detail::ScheduledTimer{when, state->m_id, generation});
        m_cv.notify_all();
    }

    inline void TimerScheduler::stop_timer(const timer_state_ptr& state) {
        if (!state) {
            return;
        }
        std::lock_guard<std::mutex> lock(m_mutex);
        state->m_is_active.store(false, std::memory_order_relaxed);
        state->m_generation.fetch_add(1, std::memory_order_relaxed);
        m_cv.notify_all();
    }

    inline void TimerScheduler::worker_loop() {
        std::vector<detail::DueTimer> due;
        std::unique_lock<std::mutex> lock(m_mutex);
        while (!m_stop_requested) {
            if (m_queue.empty()) {
                m_cv.wait(lock, [this] { return m_stop_requested || !m_queue.empty(); });
                continue;
            }

            const auto next_fire_time = m_queue.top().m_fire_time;
            const bool woke_by_condition = m_cv.wait_until(
                lock,
                next_fire_time,
                [this, next_fire_time] {
                    return m_stop_requested || m_queue.empty() || m_queue.top().m_fire_time < next_fire_time;
                }
            );

            if (m_stop_requested) {
                break;
            }

            if (woke_by_condition) {
                continue;
            }

            const auto now = clock::now();
            collect_due_timers_locked(due, now);

            lock.unlock();
            execute_due_timers(due);
            due.clear();
            lock.lock();
        }
    }

    inline void TimerScheduler::collect_due_timers_locked(std::vector<detail::DueTimer>& due, clock::time_point now) {
        while (!m_queue.empty()) {
            const auto& top = m_queue.top();
            if (top.m_fire_time > now) {
                break;
            }

            detail::ScheduledTimer item = top;
            m_queue.pop();

            auto it = m_timers.find(item.m_timer_id);
            if (it == m_timers.end()) {
                continue;
            }

            auto state = it->second.lock();
            if (!state) {
                m_timers.erase(it);
                continue;
            }

            if (!state->m_is_active.load(std::memory_order_relaxed) ||
                state->m_generation.load(std::memory_order_relaxed) != item.m_generation) {
                continue;
            }

            state->m_is_running.store(true, std::memory_order_release);
            due.push_back(detail::DueTimer{item.m_fire_time, item.m_generation, std::move(state)});
        }
    }

    inline void TimerScheduler::execute_due_timers(std::vector<detail::DueTimer>& due) {
        for (auto& timer : due) {
            detail::TimerCallback callback;
            if (timer.m_state) {
                std::lock_guard<std::mutex> callback_lock(timer.m_state->m_callback_mutex);
                callback = timer.m_state->m_callback;
            }
            if (callback) {
                detail::RunningTimerScope running_scope(timer.m_state.get());
                try {
                    callback();
                } catch (...) {
                    // TODO: integrate with logging once a logging facility is available.
                }
            }
            finalize_timer(timer);
        }
    }

    inline void TimerScheduler::finalize_timer(const detail::DueTimer& due_timer) {
        auto state = due_timer.m_state;
        if (!state) {
            return;
        }

        std::unique_lock<std::mutex> lock(m_mutex);
        state->m_is_running.store(false, std::memory_order_release);
        if (!state->m_is_active.load(std::memory_order_relaxed)) {
            return;
        }

        if (state->m_is_single_shot.load(std::memory_order_relaxed)) {
            state->m_is_active.store(false, std::memory_order_relaxed);
            state->m_generation.fetch_add(1, std::memory_order_relaxed);
            return;
        }

        if (state->m_generation.load(std::memory_order_relaxed) != due_timer.m_generation) {
            return;
        }

        const auto interval_ms = state->m_interval_ms.load(std::memory_order_relaxed);
        const auto next_fire_time = due_timer.m_fire_time + std::chrono::milliseconds(interval_ms);
        const auto next_generation = state->m_generation.fetch_add(1, std::memory_order_relaxed) + 1;
        m_queue.push(detail::ScheduledTimer{next_fire_time, state->m_id, next_generation});
        m_cv.notify_all();
    }

    // ---------------------------------------------------------------------
    // Timer inline implementation
    // ---------------------------------------------------------------------

    inline Timer::Timer(TimerScheduler& scheduler)
        : m_scheduler(scheduler), m_state(scheduler.create_timer_state()) {
        if (m_state) {
            m_state->m_has_external_owner.store(true, std::memory_order_relaxed);
        }
    }

    inline Timer::~Timer() {
        if (!m_state) {
            return;
        }

        if (detail::current_timer_state() != m_state.get()) {
            stop_and_wait();
        } else {
            m_scheduler.stop_timer(m_state);
        }

        m_state->m_has_external_owner.store(false, std::memory_order_relaxed);
        m_scheduler.destroy_timer_state(m_state);
    }

    template<class Rep, class Period>
    void Timer::set_interval(std::chrono::duration<Rep, Period> interval) noexcept {
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(interval).count();
        if (milliseconds < 0) {
            milliseconds = 0;
        }
        m_state->m_interval_ms.store(milliseconds, std::memory_order_relaxed);
    }

    inline std::chrono::milliseconds Timer::interval() const noexcept {
        const auto milliseconds = m_state->m_interval_ms.load(std::memory_order_relaxed);
        return std::chrono::milliseconds(milliseconds);
    }

    inline void Timer::start() {
        const auto milliseconds = m_state->m_interval_ms.load(std::memory_order_relaxed);
        const auto delay = TimerScheduler::clock::now() + std::chrono::milliseconds(milliseconds);
        m_scheduler.start_timer(m_state, delay);
    }

    template<class Rep, class Period>
    void Timer::start(std::chrono::duration<Rep, Period> interval) {
        set_interval(interval);
        start();
    }

    inline void Timer::stop() {
        m_scheduler.stop_timer(m_state);
    }

    inline void Timer::stop_and_wait() {
        assert(detail::current_timer_state() != m_state.get()
               && "stop_and_wait() must not be called from inside callback");
        m_scheduler.stop_timer(m_state);
        while (m_state->m_is_running.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    inline void Timer::set_single_shot(bool is_single_shot) noexcept {
        m_state->m_is_single_shot.store(is_single_shot, std::memory_order_relaxed);
    }

    inline bool Timer::is_single_shot() const noexcept {
        return m_state->m_is_single_shot.load(std::memory_order_relaxed);
    }

    inline bool Timer::is_active() const noexcept {
        return m_state->m_is_active.load(std::memory_order_relaxed);
    }

    inline bool Timer::is_running() const noexcept {
        return m_state->m_is_running.load(std::memory_order_relaxed);
    }

    inline void Timer::set_callback(Callback callback) {
        std::lock_guard<std::mutex> lock(m_state->m_callback_mutex);
        m_state->m_callback = std::move(callback);
    }

    template<class Rep, class Period>
    void Timer::single_shot(TimerScheduler& scheduler,
                            std::chrono::duration<Rep, Period> interval,
                            Callback callback) {
        auto state = scheduler.create_timer_state();
        if (!state) {
            return;
        }

        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(interval).count();
        if (milliseconds < 0) {
            milliseconds = 0;
        }

        state->m_is_single_shot.store(true, std::memory_order_relaxed);
        state->m_interval_ms.store(milliseconds, std::memory_order_relaxed);

        auto* scheduler_ptr = state->m_scheduler;

        Callback user_callback_local = std::move(callback);

        {
            std::lock_guard<std::mutex> lock(state->m_callback_mutex);
            state->m_callback = [state, scheduler_ptr, user_callback_local]() mutable {
                if (user_callback_local) {
                    user_callback_local();
                }

                auto state_ptr = state;
                if (!state_ptr) {
                    return;
                }

                {
                    std::lock_guard<std::mutex> callback_lock(state_ptr->m_callback_mutex);
                    state_ptr->m_callback = {};
                }

                if (scheduler_ptr) {
                    scheduler_ptr->destroy_timer_state(state_ptr);
                }
            };
        }

        const auto fire_time = TimerScheduler::clock::now() + std::chrono::milliseconds(milliseconds);
        scheduler.start_timer(state, fire_time);
    }

} // namespace time_shield

#endif // _TIME_SHIELD_TIMER_SCHEDULER_HPP_INCLUDED
