#include "loader.h"

void bios_loader()
{
    BOOT_HEADER boot_header;

    //TODO Populate cpu information

    //TODO Populate gdt information

    //TODO Populate idt information

    //TODO Populate memory map information

    //TODO Populate framebuffer information

    //TODO Load elf kernel

    //TODO Start elf kenrel

    //Should never reach here
    while(1);
    __builtin_unreachable();
}