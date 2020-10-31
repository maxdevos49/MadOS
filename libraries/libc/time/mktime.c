#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#include<sys/time.h>

/**
 * Returns the seconds from the unix epoch
 * */
time_t mktime(struct tm *tp)
{
    //get total days
    int64_t total_days = 0;
    int total_leap_years = leap_years_between(1970, tp->tm_year + 1900);
    int total_normal_years = ((tp->tm_year + 1900) - 1970) - total_leap_years;

    total_days += total_normal_years * year_lengths[0];
    total_days += total_leap_years * year_lengths[1];

    //Current year
    bool _is_leap = is_leap(tp->tm_year);

    //Count all previous month days
    for (int i = 0; i < tp->tm_mon - 1; i++)
        total_days += month_lengths[_is_leap][i];

    //Get days of current month
    total_days += tp->tm_mday - 1;

    int64_t total_seconds = total_days * SECS_PER_DAY;

    total_seconds += tp->tm_hour * SECS_PER_HOUR;
    total_seconds += tp->tm_min * SECS_PER_MIN;
    total_seconds += tp->tm_sec;

    return total_seconds;
}



