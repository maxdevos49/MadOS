#ifndef __KERNEL_DEVICES_VGA_H
#define __KERNEL_DEVICES_VGA_H 1
//https://pdos.csail.mit.edu/6.828/2008/readings/hardware/vgadoc/VGABIOS.TXT

#include <kernel/devices/tty.h>

#define VGA_MEMORY (uint8_t *)0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void VGA_tty_init(TTY_CONFIG* config);

#endif