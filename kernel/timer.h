#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

void install_timer();
void sleep_milliseconds(uint64_t milliseconds);
void sleep_ticks(uint64_t ticks);

#endif