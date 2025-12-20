#include <time_shield/time_conversions.hpp>

#if defined(_WIN32)
#   ifdef min
#       undef min
#   endif
#   ifdef max
#       undef max
#   endif
#endif

#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>

namespace {

    struct CivilDate {
        int64_t year;
        int month;
        int day;
    };

    /// \brief Reference conversion from days since Unix epoch to civil date.
    TIME_SHIELD_CONSTEXPR CivilDate civil_from_days(int64_t z) noexcept {
        z += 719468;
        const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
        const int64_t doe = z - era * 146097;
        const int64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
        int64_t y = static_cast<int64_t>(yoe) + era * 400;
        const int64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
        const int64_t mp = (5 * doy + 2) / 153;
        const int64_t d = doy - (153 * mp + 2) / 5 + 1;
        const int64_t m = mp + (mp < 10 ? 3 : -9);
        y += (m <= 2);
        return {y, static_cast<int>(m), static_cast<int>(d)};
    }

    /// \brief Reference conversion from civil date to days since Unix epoch.
    TIME_SHIELD_CONSTEXPR int64_t days_from_civil(int64_t p_year, int p_month, int p_day) noexcept {
        const int64_t y = p_year - (p_month <= 2 ? 1 : 0);
        const int64_t era = (y >= 0 ? y : y - 399) / 400;
        const int64_t yoe = y - era * 400;
        const int64_t month = p_month + (p_month > 2 ? -3 : 9);
        const int64_t doy = (153 * month + 2) / 5 + static_cast<int64_t>(p_day) - 1;
        const int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
        return era * 146097 + doe - 719468;
    }

    struct DateParts {
        int64_t year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
    };

    /// \brief Reference conversion from UNIX seconds to date parts.
    DateParts reference_from_timestamp(int64_t ts) noexcept {
        int64_t days = ts / time_shield::SEC_PER_DAY;
        int64_t sec_of_day = ts % time_shield::SEC_PER_DAY;
        if (sec_of_day < 0) {
            sec_of_day += time_shield::SEC_PER_DAY;
            days -= 1;
        }

        const CivilDate civil = civil_from_days(days);
        const int64_t hour = sec_of_day / time_shield::SEC_PER_HOUR;
        const int64_t min = (sec_of_day - hour * time_shield::SEC_PER_HOUR) / time_shield::SEC_PER_MIN;
        const int64_t sec = sec_of_day - hour * time_shield::SEC_PER_HOUR - min * time_shield::SEC_PER_MIN;

        return {
            civil.year,
            civil.month,
            civil.day,
            static_cast<int>(hour),
            static_cast<int>(min),
            static_cast<int>(sec)
        };
    }

    void assert_date_parts(const time_shield::DateTimeStruct& dt, const DateParts& ref) {
        assert(dt.year == ref.year);
        assert(dt.mon == ref.month);
        assert(dt.day == ref.day);
        assert(dt.hour == ref.hour);
        assert(dt.min == ref.minute);
        assert(dt.sec == ref.second);
        (void)dt;
        (void)ref;
    }

    void assert_reference_match(int64_t ts) {
        const DateParts ref = reference_from_timestamp(ts);
        const time_shield::DateTimeStruct dt = time_shield::to_date_time<time_shield::DateTimeStruct>(ts);
        assert_date_parts(dt, ref);

        const int64_t year_ref = ref.year - time_shield::UNIX_EPOCH;
        const int64_t year_fast = time_shield::years_since_epoch<int64_t>(ts);
        assert(year_fast == year_ref);
        (void)year_ref;
        (void)year_fast;
    }

    void assert_date_to_unix_day_match(int64_t year, int month, int day) {
        const int64_t ref_day = days_from_civil(year, month, day);
        const time_shield::uday_t fast_day = time_shield::date_to_unix_day(year, month, day);
        const time_shield::uday_t legacy_day = time_shield::legacy::date_to_unix_day(year, month, day);

        assert(static_cast<int64_t>(fast_day) == ref_day);
        assert(static_cast<int64_t>(legacy_day) == ref_day);
        (void)ref_day;
        (void)fast_day;
        (void)legacy_day;
    }

    void assert_to_timestamp_match(int64_t year, int month, int day, int hour, int minute, int second) {
        const int64_t ref_day = days_from_civil(year, month, day);
        const int64_t ref_ts = ref_day * time_shield::SEC_PER_DAY
            + hour * time_shield::SEC_PER_HOUR
            + minute * time_shield::SEC_PER_MIN
            + second;

        const time_shield::ts_t fast_ts = time_shield::to_timestamp(year, month, day, hour, minute, second);
        const time_shield::ts_t legacy_ts = time_shield::legacy::to_timestamp(year, month, day, hour, minute, second);

        assert(static_cast<int64_t>(fast_ts) == ref_ts);
        assert(static_cast<int64_t>(legacy_ts) == ref_ts);
        (void)ref_ts;
        (void)fast_ts;
        (void)legacy_ts;
    }

    void test_known_cases() {
        assert_reference_match(0);
        assert_reference_match(time_shield::to_timestamp(2000, 2, 29, 0, 0, 0));
        assert_reference_match(time_shield::to_timestamp(1900, 2, 28, 0, 0, 0));
        assert_reference_match(time_shield::to_timestamp(1900, 3, 1, 0, 0, 0));
        assert_reference_match(time_shield::to_timestamp(2100, 2, 28, 0, 0, 0));
        assert_reference_match(time_shield::to_timestamp(2100, 3, 1, 0, 0, 0));
        assert_reference_match(-1);
        assert_reference_match(-time_shield::SEC_PER_DAY);
        assert_reference_match(-time_shield::SEC_PER_DAY - 1);

        assert_date_to_unix_day_match(1970, 1, 1);
        assert_date_to_unix_day_match(2000, 2, 29);
        assert_date_to_unix_day_match(1900, 2, 28);
        assert_date_to_unix_day_match(1900, 3, 1);
        assert_date_to_unix_day_match(2100, 2, 28);
        assert_date_to_unix_day_match(2100, 3, 1);

        assert_to_timestamp_match(1970, 1, 1, 0, 0, 0);
        assert_to_timestamp_match(2000, 2, 29, 23, 59, 59);
        assert_to_timestamp_match(1900, 2, 28, 12, 0, 0);
        assert_to_timestamp_match(1900, 3, 1, 0, 0, 1);
        assert_to_timestamp_match(2100, 2, 28, 11, 22, 33);
        assert_to_timestamp_match(2100, 3, 1, 4, 5, 6);
    }

    void test_random_ranges() {
        std::mt19937_64 rng(0x6c6f6e675f72616eULL);
        std::uniform_int_distribution<int64_t> epoch_dist(
            -static_cast<int64_t>(1ULL << 32),
            static_cast<int64_t>(1ULL << 32));

        const int64_t window = static_cast<int64_t>(1ULL << 32);
        const int64_t min_ts = std::numeric_limits<int64_t>::min();
        const int64_t max_ts = std::numeric_limits<int64_t>::max();
        std::uniform_int_distribution<int64_t> lower_tail(min_ts, min_ts + window);
        std::uniform_int_distribution<int64_t> upper_tail(max_ts - window, max_ts);

        const int64_t sample_count = 1000000;
        for (int64_t i = 0; i < sample_count; ++i) {
            assert_reference_match(epoch_dist(rng));
            assert_reference_match(lower_tail(rng));
            assert_reference_match(upper_tail(rng));
        }

        std::uniform_int_distribution<int> year_dist(1600, 2400);
        std::uniform_int_distribution<int> month_dist(1, 12);
        std::uniform_int_distribution<int> hour_dist(0, 23);
        std::uniform_int_distribution<int> minute_dist(0, 59);
        std::uniform_int_distribution<int> second_dist(0, 59);
        const int date_samples = 500000;
        for (int i = 0; i < date_samples; ++i) {
            const int year = year_dist(rng);
            const int month = month_dist(rng);
            const int day_limit = time_shield::days_in_month(year, month);
            std::uniform_int_distribution<int> day_dist(1, day_limit);
            const int day = day_dist(rng);

            assert_date_to_unix_day_match(year, month, day);

            const int hour = hour_dist(rng);
            const int minute = minute_dist(rng);
            const int second = second_dist(rng);
            assert_to_timestamp_match(year, month, day, hour, minute, second);
        }
    }

    void test_round_trip() {
        std::mt19937_64 rng(0x726f756e645f7472ULL);
        std::uniform_int_distribution<int> year_dist(1600, 2400);
        std::uniform_int_distribution<int> month_dist(1, 12);
        std::uniform_int_distribution<int> hour_dist(0, 23);
        std::uniform_int_distribution<int> minute_dist(0, 59);
        std::uniform_int_distribution<int> second_dist(0, 59);

        const int iterations = 200000;
        for (int i = 0; i < iterations; ++i) {
            const int year = year_dist(rng);
            const int month = month_dist(rng);
            const int day_limit = time_shield::days_in_month(year, month);
            std::uniform_int_distribution<int> day_dist(1, day_limit);
            const int day = day_dist(rng);
            const int hour = hour_dist(rng);
            const int minute = minute_dist(rng);
            const int second = second_dist(rng);

            const time_shield::ts_t ts = time_shield::to_timestamp(year, month, day, hour, minute, second);
            const time_shield::DateTimeStruct dt = time_shield::to_date_time<time_shield::DateTimeStruct>(ts);

            assert(dt.year == year);
            assert(dt.mon == month);
            assert(dt.day == day);
            assert(dt.hour == hour);
            assert(dt.min == minute);
            assert(dt.sec == second);
        }
    }

    void run_benchmark() {
        constexpr int64_t iterations = 10000000;
        int64_t accumulator_fast = 0;
        int64_t accumulator_legacy = 0;

        const int64_t start_ts = -static_cast<int64_t>(1ULL << 32);
        const int64_t step = 4093;

        const auto start_fast = std::chrono::steady_clock::now();
        for (int64_t i = 0; i < iterations; ++i) {
            const int64_t ts = start_ts + i * step;
            const time_shield::DateTimeStruct dt = time_shield::to_date_time<time_shield::DateTimeStruct>(ts);
            accumulator_fast += dt.year + dt.mon + dt.day;
        }
        const auto end_fast = std::chrono::steady_clock::now();

        const auto start_legacy = std::chrono::steady_clock::now();
        for (int64_t i = 0; i < iterations; ++i) {
            const int64_t ts = start_ts + i * step;
            const time_shield::DateTimeStruct dt = time_shield::legacy::to_date_time<time_shield::DateTimeStruct>(ts);
            accumulator_legacy += dt.year + dt.mon + dt.day;
        }
        const auto end_legacy = std::chrono::steady_clock::now();

        const auto fast_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_fast - start_fast).count();
        const auto legacy_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_legacy - start_legacy).count();

        std::cout << "fast_date64 benchmark (1e7 timestamps)\n";
        std::cout << "fast_ns: " << fast_ns << " legacy_ns: " << legacy_ns << '\n';
        if (fast_ns > 0) {
            const double ratio = static_cast<double>(legacy_ns) / static_cast<double>(fast_ns);
            std::cout << "legacy/fast ratio: " << ratio << '\n';
        }
        std::cout << "accumulators: " << accumulator_fast << " " << accumulator_legacy << '\n';

        auto bench_date_range = [&](const char* label, int base_year, int year_span, int month_cycle) {
            int64_t acc_day_fast = 0;
            int64_t acc_day_legacy = 0;
            const auto start_day_fast_local = std::chrono::steady_clock::now();
            for (int64_t i = 0; i < iterations; ++i) {
                const int year = base_year + static_cast<int>(i % year_span);
                const int month = 1 + static_cast<int>(i % month_cycle);
                const int day = 1 + static_cast<int>(i % 28);
                const time_shield::uday_t unix_day = time_shield::date_to_unix_day(year, month, day);
                acc_day_fast += unix_day;
            }
            const auto end_day_fast_local = std::chrono::steady_clock::now();

            const auto start_day_legacy_local = std::chrono::steady_clock::now();
            for (int64_t i = 0; i < iterations; ++i) {
                const int year = base_year + static_cast<int>(i % year_span);
                const int month = 1 + static_cast<int>(i % month_cycle);
                const int day = 1 + static_cast<int>(i % 28);
                const time_shield::uday_t unix_day = time_shield::legacy::date_to_unix_day(year, month, day);
                acc_day_legacy += unix_day;
            }
            const auto end_day_legacy_local = std::chrono::steady_clock::now();

            const auto day_fast_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_day_fast_local - start_day_fast_local).count();
            const auto day_legacy_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_day_legacy_local - start_day_legacy_local).count();

            std::cout << "date_to_unix_day benchmark (" << label << ")\n";
            std::cout << "fast_ns: " << day_fast_ns << " legacy_ns: " << day_legacy_ns << '\n';
            if (day_fast_ns > 0) {
                const double ratio = static_cast<double>(day_legacy_ns) / static_cast<double>(day_fast_ns);
                std::cout << "legacy/fast ratio: " << ratio << '\n';
            }
            std::cout << "accumulators: " << acc_day_fast << " " << acc_day_legacy << '\n';

            int64_t acc_ts_fast = 0;
            int64_t acc_ts_legacy = 0;
            const auto start_ts_fast_local = std::chrono::steady_clock::now();
            for (int64_t i = 0; i < iterations; ++i) {
                const int year = base_year + static_cast<int>(i % year_span);
                const int month = 1 + static_cast<int>(i % month_cycle);
                const int day = 1 + static_cast<int>(i % 28);
                const int hour = static_cast<int>(i % 24);
                const int minute = static_cast<int>(i % 60);
                const int second = static_cast<int>(i % 60);
                acc_ts_fast += time_shield::to_timestamp(year, month, day, hour, minute, second);
            }
            const auto end_ts_fast_local = std::chrono::steady_clock::now();

            const auto start_ts_legacy_local = std::chrono::steady_clock::now();
            for (int64_t i = 0; i < iterations; ++i) {
                const int year = base_year + static_cast<int>(i % year_span);
                const int month = 1 + static_cast<int>(i % month_cycle);
                const int day = 1 + static_cast<int>(i % 28);
                const int hour = static_cast<int>(i % 24);
                const int minute = static_cast<int>(i % 60);
                const int second = static_cast<int>(i % 60);
                acc_ts_legacy += time_shield::legacy::to_timestamp(year, month, day, hour, minute, second);
            }
            const auto end_ts_legacy_local = std::chrono::steady_clock::now();

            const auto ts_fast_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_ts_fast_local - start_ts_fast_local).count();
            const auto ts_legacy_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_ts_legacy_local - start_ts_legacy_local).count();

            std::cout << "to_timestamp benchmark (" << label << ")\n";
            std::cout << "fast_ns: " << ts_fast_ns << " legacy_ns: " << ts_legacy_ns << '\n';
            if (ts_fast_ns > 0) {
                const double ratio = static_cast<double>(ts_legacy_ns) / static_cast<double>(ts_fast_ns);
                std::cout << "legacy/fast ratio: " << ratio << '\n';
            }
            std::cout << "accumulators: " << acc_ts_fast << " " << acc_ts_legacy << '\n';
        };

        auto bench_timestamp_math_range = [&](const char* label, int base_year, int year_span, int month_cycle) {
            int64_t acc_fast = 0;
            int64_t acc_legacy = 0;
            const auto start_fast_local = std::chrono::steady_clock::now();
            for (int64_t i = 0; i < iterations; ++i) {
                const int year = base_year + static_cast<int>(i % year_span);
                const int month = 1 + static_cast<int>(i % month_cycle);
                const int day = 1 + static_cast<int>(i % 28);
                const int hour = static_cast<int>(i % 24);
                const int minute = static_cast<int>(i % 60);
                const int second = static_cast<int>(i % 60);
                acc_fast += time_shield::to_timestamp_unchecked(year, month, day, hour, minute, second);
            }
            const auto end_fast_local = std::chrono::steady_clock::now();

            const auto start_legacy_local = std::chrono::steady_clock::now();
            for (int64_t i = 0; i < iterations; ++i) {
                const int year = base_year + static_cast<int>(i % year_span);
                const int month = 1 + static_cast<int>(i % month_cycle);
                const int day = 1 + static_cast<int>(i % 28);
                const int hour = static_cast<int>(i % 24);
                const int minute = static_cast<int>(i % 60);
                const int second = static_cast<int>(i % 60);
                acc_legacy += time_shield::legacy::to_timestamp_unchecked(year, month, day, hour, minute, second);
            }
            const auto end_legacy_local = std::chrono::steady_clock::now();

            const auto fast_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_fast_local - start_fast_local).count();
            const auto legacy_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_legacy_local - start_legacy_local).count();

            std::cout << "to_timestamp unchecked benchmark (" << label << ")\n";
            std::cout << "fast_ns: " << fast_ns << " legacy_ns: " << legacy_ns << '\n';
            if (fast_ns > 0) {
                const double ratio = static_cast<double>(legacy_ns) / static_cast<double>(fast_ns);
                std::cout << "legacy/fast ratio: " << ratio << '\n';
            }
            std::cout << "accumulators: " << acc_fast << " " << acc_legacy << '\n';
        };

        bench_date_range("1970..2100", 1970, 131, 12);
        bench_date_range("1600..1969", 1600, 370, 12);
        bench_date_range("1970..2400", 1970, 431, 12);
        bench_date_range("wide 1600..2400", 1600, 801, 12);
        bench_date_range("negative -2400..-1600", -2400, 801, 12);
        bench_timestamp_math_range("1970..2100", 1970, 131, 12);
        bench_timestamp_math_range("1600..1969", 1600, 370, 12);
        bench_timestamp_math_range("1970..2400", 1970, 431, 12);
    }
}

int main() {
    test_known_cases();
    test_random_ranges();
    test_round_trip();
    run_benchmark();
    return 0;
}
