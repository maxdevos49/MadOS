#include <boot/loader.h>

#include <boot/stdio.h>
#include <boot/string.h>
#include <boot/cpu.h>

#include <boot/drivers/tty.h>
#include <boot/drivers/vga.h>
#include <boot/drivers/serial.h>

extern struct DESCRIPTOR GDT_DESCRIPTOR;
extern struct DESCRIPTOR IDT_DESCRIPTOR;

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
    printf("GDT Information -> Size: 0x%04x, Base: 0x%08x\n", boot.gdt.size, boot.gdt.base);

    //IDT Info
    boot.idt.size = IDT_DESCRIPTOR.size;
    boot.idt.base = IDT_DESCRIPTOR.base;
    printf("IDT Information -> Size: 0x%04x, Base: 0x%08x\n", boot.idt.size, boot.idt.base);

    //TODO Populate memory map information

    //TODO Populate framebuffer information

    //TODO Setup achi/sata

    //TODO Load elf kernel

    //TODO Start elf kernel

    //Should never reach here
    while (1)
        ;
    __builtin_unreachable();
}