#ifndef __KERNEL_DEVICES_VESA_H
#define __KERNEL_DEVICES_VESA_H 1

#include <kernel/devices/tty.h>

void VESA_install();
void VESA_tty_init(TTY_CONFIG *config, GRAPHICS_CONTEXT *ctx);

#endif