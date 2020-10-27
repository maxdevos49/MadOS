#ifndef __TIME_H
#define __TIME_H


typedef long long int clock_t;

typedef long long int time_t; //TODO platform specific

struct tm
{
    int tm_sec;   //seconds after minute
    int tm_min;   //minutes after hour
    int tm_hour;  //hours after midnight
    int tm_mday;  //day of the month
    int tm_mon;   //monsths since january
    int tm_year;  //years since 1900
    int tm_wday;  //days since sunday
    int tm_yday;  //days since january 1
    int tm_isdst; //Is daylight savings
};

clock_t clock(void);
char *ctime(const time_t *timer);

//TODO system specific macro
//#define CLOCKS_PER_SEC

#ifndef NULL
#define NULL 0
#endif

#endif