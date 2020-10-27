#ifndef __KERNEL_TIMER_H
#define __KERNEL_TIMER_H 1

#include <stdint.h>

void install_timer();
void sleep_milliseconds(uint64_t milliseconds);
void sleep_ticks(uint64_t ticks);

#endif