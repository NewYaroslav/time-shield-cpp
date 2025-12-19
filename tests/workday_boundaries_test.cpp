#include <time_shield/time_conversions.hpp>
#include <time_shield/time_parser.hpp>
#include <cassert>
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

    assert(first_workday_day(2024, 6) == 3);
    assert(last_workday_day(2024, 6) == 28);
    assert(count_workdays_in_month(2024, 6) == 20);
    assert(workday_index_in_month(2024, 6, 27) == 19);

    assert(is_first_workday_of_month(june_third));
    assert(!is_first_workday_of_month(june_first));
    assert(!is_first_workday_of_month(june_fourth));

    assert(is_first_workday_of_month(2024, 6, 3));
    assert(!is_first_workday_of_month(2024, 6, 1));

    const ts_ms_t june_third_ms = june_third * MS_PER_SEC + 250;
    const ts_ms_t june_fourth_ms = june_fourth * MS_PER_SEC;
    assert(is_first_workday_of_month_ms(june_third_ms));
    assert(!is_first_workday_of_month_ms(june_fourth_ms));

    assert(is_last_workday_of_month(june_twenty_eighth));
    assert(!is_last_workday_of_month(june_twenty_seventh));

    assert(is_last_workday_of_month(2024, 6, 28));
    assert(!is_last_workday_of_month(2024, 6, 29));

    assert(is_last_workday_of_month_ms(june_twenty_eighth * MS_PER_SEC));
    assert(!is_last_workday_of_month_ms((june_twenty_eighth + SEC_PER_DAY) * MS_PER_SEC));

    assert(is_within_first_workdays_of_month(june_third, 1));
    assert(!is_within_first_workdays_of_month(june_fourth, 1));
    assert(is_within_first_workdays_of_month(june_fourth, 2));
    assert(!is_within_first_workdays_of_month(june_third, 0));
    assert(!is_within_first_workdays_of_month(june_third, 25));

    assert(is_within_first_workdays_of_month(2024, 6, 3, 1));
    assert(!is_within_first_workdays_of_month(2024, 6, 4, 1));
    assert(is_within_first_workdays_of_month(2024, 6, 4, 2));
    assert(!is_within_first_workdays_of_month(2024, 6, 3, 30));

    assert(is_within_first_workdays_of_month_ms(june_third_ms, 1));
    assert(!is_within_first_workdays_of_month_ms(june_fourth_ms, 1));

    assert(is_within_last_workdays_of_month(june_twenty_eighth, 1));
    assert(is_within_last_workdays_of_month(june_twenty_seventh, 2));
    assert(!is_within_last_workdays_of_month(june_twenty_sixth, 2));
    assert(is_within_last_workdays_of_month(june_twenty_sixth, 3));
    assert(!is_within_last_workdays_of_month(june_twenty_eighth, 0));
    assert(!is_within_last_workdays_of_month(june_twenty_eighth, 40));

    assert(is_within_last_workdays_of_month(2024, 6, 28, 1));
    assert(is_within_last_workdays_of_month(2024, 6, 27, 2));
    assert(!is_within_last_workdays_of_month(2024, 6, 27, 1));
    assert(is_within_last_workdays_of_month(2024, 6, 26, 3));

    assert(is_within_last_workdays_of_month_ms(june_twenty_eighth * MS_PER_SEC, 1));
    assert(!is_within_last_workdays_of_month_ms(june_twenty_sixth * MS_PER_SEC, 2));

    const std::string sept_first = "2024-09-02T09:00:00Z"; // Monday after a Sunday start
    const std::string sept_second = "2024-09-03T09:00:00Z";
    const std::string sept_first_ms = "2024-09-02T09:00:00.250Z";

    assert(is_first_workday_of_month(sept_first));
    assert(!is_first_workday_of_month(sept_second));
    assert(is_first_workday_of_month_ms(sept_first_ms));
    assert(!is_first_workday_of_month_ms("2024-09-03T09:00:00.250Z"));

    assert(is_within_first_workdays_of_month(sept_second, 2));
    assert(!is_within_first_workdays_of_month(sept_second, 1));
    assert(is_within_first_workdays_of_month_ms("2024-09-03T09:00:00.000Z", 2));

    assert(is_last_workday_of_month("2024-06-28T12:00:00Z"));
    assert(is_last_workday_of_month_ms("2024-06-28T12:00:00.000Z"));
    assert(!is_last_workday_of_month("2024-06-27T12:00:00Z"));

    assert(is_within_last_workdays_of_month("2024-06-26T10:00:00Z", 3));
    assert(!is_within_last_workdays_of_month("2024-06-26T10:00:00Z", 2));
    assert(is_within_last_workdays_of_month_ms("2024-06-27T10:00:00.500Z", 2));

    assert(!is_first_workday_of_month("not-a-date"));
    assert(!is_within_first_workdays_of_month_ms("invalid", 2));

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
