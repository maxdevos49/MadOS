#ifndef __LIBC_SYS_TIME_H
#define __LIBC_SYS_TIME_H 1

#include <stdint.h>

#define MINS_PER_HOUR 60
#define HOURS_PER_DAY 24
#define DAYS_PER_WEEK 7
#define MONTHS_PER_YEAR 12
#define DAYS_PER_NYEAR 365
#define DAYS_PER_LYEAR 366
#define YEAR_BASE 1900

#define SECS_PER_MIN 60
#define SECS_PER_HOUR (60 * 60)
#define SECS_PER_DAY (SECS_PER_HOUR * 24)
#define SECS_PER_YEAR (SECS_PER_DAY * 365)

#define is_leap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

static inline int leap_years_before(int year)
{
    year--;
    return (year / 4) - (year / 100) + (year / 400);
}

static inline int leap_years_between(int start, int end)
{
    return leap_years_before(end) - leap_years_before(start);
}

static const uint8_t month_lengths[2][MONTHS_PER_YEAR] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

static const uint16_t year_lengths[2] = {DAYS_PER_NYEAR, DAYS_PER_LYEAR};

static const char *days_of_the_week[2][7] = {
    {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"},
    {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}};

static const char *months_of_the_year[2][12] = {
    {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"},
    {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}};

#endif