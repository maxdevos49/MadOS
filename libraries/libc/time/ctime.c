#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/time.h>

#define CURRENT_CENTURY 20

static char time_str[24];
/**
 * Converts the time_t type to a string time
 * */
char *ctime(const time_t *timer)
{
    time_t tm;

    if (timer != NULL) //Test
        tm = *timer;
    else
        tm = time(NULL);

    struct tm t; //testing something

    t.tm_hour = 0;
    t.tm_isdst = 0;
    t.tm_mday = 0;
    t.tm_min = 0;
    t.tm_mon = 0;
    t.tm_sec = 0;
    t.tm_wday = 0;
    t.tm_yday = 0;
    t.tm_year = 0;

    //Most of this is a miracle that it works but it does I guess.(I think it does atleast)

    //Get Year
    t.tm_year = tm / SECS_PER_YEAR + 1970;

    //Get day of year
    t.tm_yday = ((tm % SECS_PER_YEAR) / SECS_PER_DAY) - leap_years_between(1970, t.tm_year) + 1;

    bool leap = is_leap(t.tm_year);
    int32_t days_so_far = 0;
    int i = 0;
    //get the month
    for (i = 0; t.tm_yday > days_so_far; i++)
    {
        t.tm_mon = i + 1;
        days_so_far += month_lengths[leap][i];
    }

    //get day of month
    t.tm_mday = ((days_so_far + month_lengths[leap][i]) - t.tm_yday) - 1;

    //Get Hour of day
    t.tm_hour = ((tm % SECS_PER_YEAR) % SECS_PER_DAY) / SECS_PER_HOUR;

    //Get minute of hour
    t.tm_min = (((tm % SECS_PER_YEAR) % SECS_PER_DAY) % SECS_PER_HOUR) / SECS_PER_MIN;

    //Get seconds of min
    t.tm_sec = (((tm % SECS_PER_YEAR) % SECS_PER_DAY) % SECS_PER_HOUR) % SECS_PER_MIN;

    //Get day of the week
    t.tm_wday = (t.tm_mday + (t.tm_mon + 1) + (t.tm_year - 2000) + (CURRENT_CENTURY)) % 7;

    //Build string
    uint8_t str_index = 0;
    time_str[str_index] = '\0';

    //TODO use sprintf() later
    printf("%s %s %d %d:%d:%d %d\n", days_of_the_week[0][t.tm_wday], months_of_the_year[0][t.tm_mon - 1], t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, t.tm_year);

    return time_str;
}