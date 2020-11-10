#include <kernel/pci/pci.h>
#include <kernel/io.h>

#include <kernel/timer.h>
#include <kernel/tty.h>
#include <kernel/vga.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//https://pci-ids.ucw.cz/read/PC/1234/1111

void print_bar(const char *bar_name, uint32_t bar);

void PCI_install()
{
    printf("PCI Info:\n");

    //Brute force every possible bus and device
    for (int bus = 0; bus < 256; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            uint32_t reg_0 = PCI_config_read_dw(bus, device, 0, 0);

            if ((reg_0 & 0x0000ffff) != 0xffff)
            {
                TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
                printf("Bus #: %d, Device #: %d\n"  , bus, device);
                TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);

                printf("\tDevice ID: %x, Vendor ID: %x\n", (reg_0 & 0xffff0000) >> 16, reg_0 & 0x0000ffff);

                uint32_t reg_2 = PCI_config_read_dw(bus, device, 0, 0x08);

                printf("\tClass Code Name: %s, Sub Class: %x, Prog IF: %x, Rev Id: %x\n", PCI_class_codes[(reg_2 & 0xff000000) >> 24], (reg_2 & 0x00ff0000) >> 16, (reg_2 & 0x0000ff00) >> 8, (reg_2 & 0x000000ff));
                uint32_t reg_4 = PCI_config_read_dw(bus, device, 0, 0x10);
                print_bar("Bar 0", reg_4);
                uint32_t reg_5 = PCI_config_read_dw(bus, device, 0, 0x18);
                print_bar("Bar 1", reg_5);
                uint32_t reg_6 = PCI_config_read_dw(bus, device, 0, 0x14);
                print_bar("Bar 2", reg_6);
                uint32_t reg_7 = PCI_config_read_dw(bus, device, 0, 0x1c);
                print_bar("Bar 3", reg_7);
                uint32_t reg_8 = PCI_config_read_dw(bus, device, 0, 0x20);
                print_bar("Bar 4", reg_8);
                uint32_t reg_9 = PCI_config_read_dw(bus, device, 0, 0x24);
                print_bar("Bar 5", reg_9);
            }
        }
    }
}

void print_bar(const char *bar_name, uint32_t bar)
{
    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    if ((bar & PCI_bar_always_0) == 0)
    {
        if (((bar & PCI_bar_16_byte_aligned_base_address)) == 0)
        {
            TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
            return;
        }

        //Memory Space
        printf("\t\t%s: Memory Base: %x, Prefetchable: %x, Type: %x\n", bar_name, (bar & PCI_bar_16_byte_aligned_base_address), (bar & PCI_bar_prefetchable) != 0, (bar & PCI_bar_type));
    }
    else if((bar & PCI_io_always_1) == 1)
    {
        if (((bar & PCI_io_4_byte_aligned_base_address)) == 0)
        {
            TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
            return;
        }
        //I/O Space
        printf("\t\t%s: I/O Base: %x\n", bar_name, (bar & PCI_io_4_byte_aligned_base_address) >> 2);
    }

    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
}