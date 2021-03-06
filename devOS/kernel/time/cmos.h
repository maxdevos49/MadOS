#ifndef __KERNEL_TIME_CMOS_H
#define __KERNEL_TIME_CMOS_H 1

#include <stdint.h>
#include <stdbool.h>

#define CMOS_COMMAND_PORT 0x70
#define CMOS_DATA_PORT 0x71

#define CMOS_CENTURY_REGISTER_DEFAULT 0x00
#define CMOS_DISABLE_NMI true

void CMOS_read(uint8_t data[128]);
void CMOS_write(uint8_t data[128]);

uint8_t CMOS_read_register(uint8_t reg);
void CMOS_write_register(uint8_t reg, uint8_t value);

void CMOS_disable_NMI(bool disable);
void CMOS_set_century_register(uint8_t reg);
uint8_t CMOS_get_century_register();


#endif