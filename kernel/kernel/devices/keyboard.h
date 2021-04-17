#ifndef __KERNEL_DEVICES_KEYBOARD_H
#define __KERNEL_DEVICES_KEYBOARD_H 1

#include <stdint.h>

void KB_install(void);

void register_kb_handler(void (*handler)(uint8_t scan_code, char chr));

#endif