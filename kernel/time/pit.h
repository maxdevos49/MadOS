#ifndef __KERNEL_TIME_PIT_H
#define __KERNEL_TIME_PIT_H 1

#include <stdint.h>

#define PIT_BASE_FREQUENCY 1193182

#define PIT_CH0_DATA_PORT 0x40
#define PIT_CH2_DATA_PORT 0x42

#define PIT_COMMAND_PORT 0x43

//Select what channel is being configured
#define PIT_CHANNEL_0 0x00
#define PIT_CHANNEL_2 0xC0

//Allows to read the Hi and Lo bytes to be read without the first read being outdated by the time of the second read
#define PIT_ACCESS_LATCH 0x00

#define PIT_ACCESS_LOBYTE 0x10
#define PIT_ACCESS_HIBYTE 0x20
#define PIT_ACCESS_HILOBYTE 0x30

#define PIT_MODE_COUNTDOWN 0x00
#define PIT_MODE_ONESHOT 0x02
#define PIT_MODE_RATEGEN 0x04
#define PIT_MODE_SQUAREGEN 0x06
#define PIT_MODE_SOFTSTROBE 0x08
#define PIT_MODE_HARDSTROBE 0x0A

// uint32_t PIT_read_count();

void PIT_configure(uint8_t channel_port, uint8_t mode, uint16_t hz);

void PIT_enable_periodic_irq0();
void PIT_disable_periodic_irq0();
void PIT_set_periodic_frequency(uint16_t hz);
uint16_t PIT_get_closest_frequency(uint16_t hz);

#endif