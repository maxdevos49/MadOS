#ifndef __LOADER_H
#define __LOADER_H 1

#include "stdint.h"

#define STRINGIFY(s) #s              //Stringify symbol name
#define STRINGIFY_MV(s) STRINGIFY(s) //Stringify macro value

#define VERSION_MAJOR 0  //Breaking API changes
#define VERSION_MINOR 1  //New features but no breaking changes
#define VERSION_BUGFIX 0 //Bug fixes

#define VERSION_STRING "v" STRINGIFY_MV(VERSION_MAJOR) "." STRINGIFY_MV(VERSION_MINOR) "." STRINGIFY_MV(VERSION_BUGFIX)

enum BOOT_MODE
{
    LEGACY,
    UEFI
};

struct CPU
{
    char brand_string[13]; //12 bytes + null terminator
    char model_string[49]; //16 bytes + null terminator
};

struct FRAMEBUFFER
{
    /**
     * Bit 0: valid = 1
     * Bit 1-7: Reserved 
     * */
    uint8_t status;
    uint8_t bpp;

    uint16_t width;
    uint16_t height;
    uint16_t pitch;

    void *font; //Bitmap font
    void *framebuffer;
};

struct DESCRIPTOR
{
    uint16_t size;
    uint32_t base;
} __attribute__((packed));

typedef struct
{
    struct CPU cpu;
    enum BOOT_MODE mode;
    struct DESCRIPTOR gdt;
    struct DESCRIPTOR idt;
    struct DESCRIPTOR memory_map;
    struct FRAMEBUFFER framebuffer;
} BOOT_HEADER;

#ifdef __LEGACY

#endif

#ifdef __UEFI

#endif

#endif