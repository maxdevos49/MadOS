#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#define SECS_PER_MIN 60
#define MINS_PER_HOUR 60
#define HOURS_PER_DAY 24
#define DAYS_PER_WEEK 7
#define MONTHS_PER_YEAR 12
#define DAYS_PER_NYEAR 365
#define DAYS_PER_LYEAR 366
#define SECS_PER_HOUR (SECS_PER_MIN * MINS_PER_HOUR)
#define SECS_PER_DAY ((uint64_t)SECS_PER_HOUR * HOURS_PER_DAY)
#define YEAR_BASE 1900

#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

static const uint8_t month_lengths[2][MONTHS_PER_YEAR] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

static const uint16_t year_lengths[2] = {DAYS_PER_NYEAR, DAYS_PER_LYEAR};

static inline int leap_years_before(int year)
{
    year--;
    return (year / 4) - (year / 100) + (year / 400);
}


static inline int leap_years_between(int start, int end)
{
    return leap_years_before(end) - leap_years_before(start);
}

time_t mktime(struct tm *tp)
{
    //get total days
    int64_t total_days = 0;
    int total_leap_years = leap_years_between(1970, tp->tm_year + 1900);
    int total_normal_years = ((tp->tm_year + 1900) - 1970) - total_leap_years;

    total_days += total_normal_years * year_lengths[0];
    total_days += total_leap_years * year_lengths[1];

    //Current year
    bool is_leap = false;
    if (isleap(tp->tm_year))
        is_leap = true;

    //Count all previous month days
    for (int i = 0; i < tp->tm_mon - 1; i++)
        total_days += month_lengths[is_leap][i];

    //Get days of current month
    total_days += tp->tm_mday - 1;

    int64_t total_seconds = total_days * SECS_PER_DAY;

    total_seconds += tp->tm_hour * SECS_PER_HOUR;
    total_seconds += tp->tm_min * SECS_PER_MIN;
    total_seconds += tp->tm_sec;

    //convert days to seconds and then milliseconds
    return total_seconds;
}



