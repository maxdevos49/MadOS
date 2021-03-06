#ifndef __LOADER_H
#define __LOADER_H 1

#include "types.h"

enum BOOT_MODE
{
    BIOS,
    UEFI
};

struct CPU
{
    uint32_t maximum_standard_level;
    char *vendor_string; //12 bytes

    //Standard Level 1
    //EAX
    uint8_t extended_family; // Bits 27-20
    uint8_t extended_model;  //Bits 19-16
    uint8_t type;            //Bits 13-12
    uint8_t family;          //Bits 11-8
    uint8_t model;           //bits 7-4
    uint8_t stepping;        //bits 3-0

    uint8_t data[12]; //Above pointers point within here
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
    void *base;
};

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