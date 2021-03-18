#include <boot/loader.h>

#include <boot/stdio.h>
#include <boot/stdlib.h>
#include <boot/string.h>
#include <boot/cpu.h>

#include <boot/drivers/tty.h>
#include <boot/drivers/vga.h>
#include <boot/drivers/serial.h>
#include <boot/drivers/vesa.h>
#include <boot/drivers/pci.h>

extern struct DESCRIPTOR GDT_DESCRIPTOR;
extern struct DESCRIPTOR IDT_DESCRIPTOR;
extern struct DESCRIPTOR MEMORYMAP_DESCRIPTOR;
extern struct FRAMEBUFFER_INFO FRAMEBUFFER_INFO;
extern char FONT[4096];

void legacy_loader()
{

    //Serial Debugging is outputted to stdio on host machine
    SERIAL_init();

    TTY_CONFIG tty_config;
    VGA_tty_init(&tty_config);

    //Initialize the tty interface for the screen character output
    int err_code;
    if ((err_code = TTY_init(&tty_config)) != 0)
    {
        serial_printf("[ERROR: %2x] TTY initialization failed. Halting Boot.", err_code);

        while (1)
            ;
        __builtin_unreachable();
    }

    printf("\nEntering Loader: %s\n", VERSION_STRING);

    BOOT_HEADER boot;
    boot.mode = LEGACY;
    printf("Boot Mode: Legacy BIOS\n");

    //CPU Info
    CPU_info(&boot.cpu);

    //GDT Info
    boot.gdt.size = GDT_DESCRIPTOR.size;
    boot.gdt.base = GDT_DESCRIPTOR.base;
    printf("GDT Information:\n\tSize: 0x%04x\n\t Base: 0x%08x\n", boot.gdt.size, boot.gdt.base);

    //IDT Info
    boot.idt.size = IDT_DESCRIPTOR.size;
    boot.idt.base = IDT_DESCRIPTOR.base;
    printf("IDT Information:\n\t Size: 0x%04x\n\t Base: 0x%08x\n", boot.idt.size, boot.idt.base);

    //TODO setup exception interrupts and remap pics??

    // Memory Map
    boot.memory_map.size = MEMORYMAP_DESCRIPTOR.size;
    boot.memory_map.base = MEMORYMAP_DESCRIPTOR.base;
    printf("Memory Map Information:\n\tSize: 0x%04x\n\t Base: 0x%08x\n", boot.memory_map.size, boot.memory_map.base);

    //Framebuffer
    boot.framebuffer.font = FONT;
    boot.framebuffer.buffer = (void*)FRAMEBUFFER_INFO.buffer;
    boot.framebuffer.width = FRAMEBUFFER_INFO.width;
    boot.framebuffer.height = FRAMEBUFFER_INFO.height;
    boot.framebuffer.bpp = FRAMEBUFFER_INFO.bpp;
    boot.framebuffer.pitch = FRAMEBUFFER_INFO.pitch;

    printf("Framebuffer Information:\n");
    printf("\tFont Base: 0x%08x\n", boot.framebuffer.font);
    printf("\tFramebuffer Base: 0x%08x\n", boot.framebuffer.buffer);
    printf("\tWidth: %d\n", boot.framebuffer.width);
    printf("\tHeight: %d\n", boot.framebuffer.height);
    printf("\tBPP: %d\n", boot.framebuffer.bpp);
    printf("\tPitch: %d\n", boot.framebuffer.pitch);

    //TODO PCI Enumeration
    PCI_configuration(NULL, 0);

    //TODO Setup ACHI/SATA/USB controller/USB Mass Storage Drivers

    //TODO Load elf kernel

    //TODO Start elf kernel

    //Should never reach here
    while (1)
        ;
    __builtin_unreachable();
}