#include <time_shield/time_conversions.hpp>
#include <time_shield/detail/floor_math.hpp>

#include <array>
#include <cassert>
#include <cstdint>

int main() {
    using namespace time_shield;

    const auto check_ms = [](ts_ms_t ts_ms) {
        const int64_t sec_floor = detail::floor_div<int64_t>(
            static_cast<int64_t>(ts_ms), MS_PER_SEC);
        const ts_t start_sec = start_of_year(static_cast<ts_t>(sec_floor));
        const ts_t end_sec = end_of_year(static_cast<ts_t>(sec_floor));
        const ts_ms_t ref_start_ms = static_cast<ts_ms_t>(start_sec * MS_PER_SEC);
        const ts_ms_t ref_end_ms = static_cast<ts_ms_t>(end_sec * MS_PER_SEC + MS_PER_SEC - 1);
        assert(start_of_year_ms(ts_ms) == ref_start_ms);
        assert(end_of_year_ms(ts_ms) == ref_end_ms);
        assert(ref_start_ms <= ts_ms);
        assert(ts_ms <= ref_end_ms);
    };

    const std::array<ts_ms_t, 10> near_epoch_ms = {
        -2001, -2000, -1001, -1000, -999, -2, -1, 0, 1, 999
    };
    for (ts_ms_t ts_ms : near_epoch_ms) {
        check_ms(ts_ms);
    }
    check_ms(1000);
    check_ms(1001);

    const std::array<int64_t, 10> years = {
        1969, 1970, 1971, 1972, 1900, 2000, 2100, 2400, 1600, -2400
    };
    const std::array<int64_t, 7> start_offsets = {
        -1000, -999, -1, 0, 1, 999, 1000
    };
    const std::array<int64_t, 5> end_offsets = {
        -1000, -1, 0, 1, 1000
    };

    for (int64_t year : years) {
        const ts_t year_start_sec = to_timestamp(year, 1, 1, 0, 0, 0);
        const ts_ms_t year_start_ms = static_cast<ts_ms_t>(year_start_sec * MS_PER_SEC);
        for (int64_t offset : start_offsets) {
            check_ms(year_start_ms + offset);
        }

        const ts_t year_mid_sec = to_timestamp(year, 6, 1, 0, 0, 0);
        const ts_t year_end_sec = end_of_year(year_mid_sec);
        const ts_ms_t year_end_ms = static_cast<ts_ms_t>(year_end_sec * MS_PER_SEC + MS_PER_SEC - 1);
        for (int64_t offset : end_offsets) {
            check_ms(year_end_ms + offset);
        }
    }

    const std::array<ts_ms_t, 11> extremes = {
        MIN_TIMESTAMP_MS + 1,
        MIN_TIMESTAMP_MS + 999,
        MIN_TIMESTAMP_MS + 1000,
        MIN_TIMESTAMP_MS,
        MIN_TIMESTAMP_MS - 1,
        MIN_TIMESTAMP_MS - 999,
        MIN_TIMESTAMP_MS - 1000,
        MAX_TIMESTAMP_MS,
        MAX_TIMESTAMP_MS - 1,
        MAX_TIMESTAMP_MS - 999,
        MAX_TIMESTAMP_MS - 1000
    };
    for (ts_ms_t ts_ms : extremes) {
        check_ms(ts_ms);
    }

    (void)detail::floor_mod<int64_t>(0, 1);
    return 0;
}
