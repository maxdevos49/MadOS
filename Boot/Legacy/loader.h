/**
 * 
 * Kernel Loader
 * 
 * */
#ifndef __LOADER_H
#define __LOADER_H 1

enum BOOT_MODE
{
    BIOS,
    UEFI
};

struct CPU
{
    unsigned int maximum_standard_level;
    char *vendor_string; //12 bytes

    //Standard Level 1
    //EAX
    unsigned char extended_family; // Bits 27-20
    unsigned char extended_model;  //Bits 19-16
    unsigned char type;            //Bits 13-12
    unsigned char family;          //Bits 11-8
    unsigned char model;           //bits 7-4
    unsigned char stepping;        //bits 3-0

    unsigned char data[12]; //Above pointers point within here
};

struct FRAMEBUFFER
{
    /**
     * Bit 0: valid = 1
     * Bit 1-7: Reserved 
     * */
    unsigned char status;
    unsigned char bpp;
    
    unsigned short width;
    unsigned short height;
    unsigned short pitch;

    void *font;//Bitmap font
    void *framebuffer;
};

struct DESCRIPTOR
{
    unsigned short size;
    void *base;
};

typedef struct BOOT_HEADER
{
    struct CPU cpu;
    enum BOOT_MODE mode;
    struct DESCRIPTOR gdt;
    struct DESCRIPTOR idt;
    struct DESCRIPTOR memory_map;
    struct FRAMEBUFFER framebuffer;
}BOOT_HEADER;

#endif