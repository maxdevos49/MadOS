#ifndef __BOOT_DRIVERS_VGA_H
#define __BOOT_DRIVERS_VGA_H 1

#include "tty.h"

#define VGA_MEMORY (uint8_t *)0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void VGA_tty_init(TTY_CONFIG* config);

#endif