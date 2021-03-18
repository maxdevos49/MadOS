#ifndef __BOOT_DRIVERS_VESA_H
#define __BOOT_DRIVERS_VESA_H 1

#include "../stdint.h"

struct FRAMEBUFFER_INFO
{
    uint16_t width;
    uint16_t height;
    uint8_t bpp;
    uint16_t mode;
    uint16_t pitch;
    uint32_t buffer;
} __attribute__((packed));

#endif