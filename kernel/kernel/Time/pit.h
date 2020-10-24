#ifndef __KERNEL_PIT_H
#define __KERNEL_PIT_H

#include <stdint.h>

#define PIT_CHANNEL1_DATA 0x40
#define PIT_CHANNEL2_DATA 0x41
#define PIT_CHANNEL3_DATA 0x42
#define PIT_COMMAND 0x43




void init_pit();
void disable_pit();
void reset_pit_defaults();

// bool is_pit_periodic();
// bool is_pit_periodic_capable();

// void set_pit_periodic();
// void set_pit_non_periodic();

void set_pit_frequency(uint32_t hz);
uint32_t get_pit_frequency();

#endif