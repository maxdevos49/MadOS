#ifndef __KERNEL_TIMER_H
#define __KERNEL_TIMER_H 1

#include <stdint.h>
#include <kernel/time/rtc.h>

#define TIMER_RTC_RATE 10
#define TIMER_PIT_FREQUENCY 100


void TIMER_install();

void TIMER_set_time(struct RTC_time *t);
int64_t TIMER_get_time();

void TIMER_sleep_milliseconds(uint64_t milliseconds);
void TIMER_sleep_microseconds(uint64_t microseconds);
void TIMER_sleep_ticks(uint64_t ticks);

#endif