#include <time_shield/time_conversions.hpp>
#include <time_shield/time_parser.hpp>
#include "test_assert.hpp"
#include <string>

/// \brief Validates first/last workday helpers across overload sets.
int main() {
    using namespace time_shield;

    const ts_t june_first = to_timestamp(2024, 6, 1);
    const ts_t june_third = to_timestamp(2024, 6, 3);
    const ts_t june_fourth = to_timestamp(2024, 6, 4);
    const ts_t june_twenty_sixth = to_timestamp(2024, 6, 26);
    const ts_t june_twenty_seventh = to_timestamp(2024, 6, 27);
    const ts_t june_twenty_eighth = to_timestamp(2024, 6, 28);

    TIME_SHIELD_TEST_CHECK(first_workday_day(2024, 6) == 3);
    TIME_SHIELD_TEST_CHECK(last_workday_day(2024, 6) == 28);
    TIME_SHIELD_TEST_CHECK(count_workdays_in_month(2024, 6) == 20);
    TIME_SHIELD_TEST_CHECK(workday_index_in_month(2024, 6, 27) == 19);

    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month(june_third));
    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month(june_first));
    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month(june_fourth));

    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month(2024, 6, 3));
    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month(2024, 6, 1));

    const ts_ms_t june_third_ms = june_third * MS_PER_SEC + 250;
    const ts_ms_t june_fourth_ms = june_fourth * MS_PER_SEC;
    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month_ms(june_third_ms));
    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month_ms(june_fourth_ms));

    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month(june_twenty_eighth));
    TIME_SHIELD_TEST_CHECK(!is_last_workday_of_month(june_twenty_seventh));

    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month(2024, 6, 28));
    TIME_SHIELD_TEST_CHECK(!is_last_workday_of_month(2024, 6, 29));

    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month_ms(june_twenty_eighth * MS_PER_SEC));
    TIME_SHIELD_TEST_CHECK(!is_last_workday_of_month_ms((june_twenty_eighth + SEC_PER_DAY) * MS_PER_SEC));

    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month(june_third, 1));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month(june_fourth, 1));
    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month(june_fourth, 2));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month(june_third, 0));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month(june_third, 25));

    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month(2024, 6, 3, 1));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month(2024, 6, 4, 1));
    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month(2024, 6, 4, 2));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month(2024, 6, 3, 30));

    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month_ms(june_third_ms, 1));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month_ms(june_fourth_ms, 1));

    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(june_twenty_eighth, 1));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(june_twenty_seventh, 2));
    TIME_SHIELD_TEST_CHECK(!is_within_last_workdays_of_month(june_twenty_sixth, 2));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(june_twenty_sixth, 3));
    TIME_SHIELD_TEST_CHECK(!is_within_last_workdays_of_month(june_twenty_eighth, 0));
    TIME_SHIELD_TEST_CHECK(!is_within_last_workdays_of_month(june_twenty_eighth, 40));

    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(2024, 6, 28, 1));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(2024, 6, 27, 2));
    TIME_SHIELD_TEST_CHECK(!is_within_last_workdays_of_month(2024, 6, 27, 1));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month(2024, 6, 26, 3));

    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month_ms(june_twenty_eighth * MS_PER_SEC, 1));
    TIME_SHIELD_TEST_CHECK(!is_within_last_workdays_of_month_ms(june_twenty_sixth * MS_PER_SEC, 2));

    const std::string sept_first = "2024-09-02T09:00:00Z"; // Monday after a Sunday start
    const std::string sept_second = "2024-09-03T09:00:00Z";
    const std::string sept_first_ms = "2024-09-02T09:00:00.250Z";

    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month(sept_first));
    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month(sept_second));
    TIME_SHIELD_TEST_CHECK(is_first_workday_of_month_ms(sept_first_ms));
    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month_ms("2024-09-03T09:00:00.250Z"));

    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month(sept_second, 2));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month(sept_second, 1));
    TIME_SHIELD_TEST_CHECK(is_within_first_workdays_of_month_ms("2024-09-03T09:00:00.000Z", 2));

    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month("2024-06-28T12:00:00Z"));
    TIME_SHIELD_TEST_CHECK(is_last_workday_of_month_ms("2024-06-28T12:00:00.000Z"));
    TIME_SHIELD_TEST_CHECK(!is_last_workday_of_month("2024-06-27T12:00:00Z"));

    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month("2024-06-26T10:00:00Z", 3));
    TIME_SHIELD_TEST_CHECK(!is_within_last_workdays_of_month("2024-06-26T10:00:00Z", 2));
    TIME_SHIELD_TEST_CHECK(is_within_last_workdays_of_month_ms("2024-06-27T10:00:00.500Z", 2));

    TIME_SHIELD_TEST_CHECK(!is_first_workday_of_month("not-a-date"));
    TIME_SHIELD_TEST_CHECK(!is_within_first_workdays_of_month_ms("invalid", 2));

    (void)june_first;
    (void)june_third;
    (void)june_fourth;
    (void)june_twenty_sixth;
    (void)june_twenty_seventh;
    (void)june_twenty_eighth;
    (void)june_third_ms;
    (void)june_fourth_ms;
    (void)sept_first;
    (void)sept_second;
    (void)sept_first_ms;

    return 0;
}
