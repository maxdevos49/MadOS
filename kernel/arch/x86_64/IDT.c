#include "IDT.h"
#include "io.h"
#include <stdio.h>
#include "KB_scan_code_1.h"

extern IDT64_t _idt[256]; // 4096 byte IDT

extern uint64_t isr1; // interupt service routine one function pointer, Defined in assembly

extern void load_IDT();

void init_IDT()
{

    printf("Defining IDT\n");

    _idt[1].zero = 0;
    _idt[1].offset_low = (uint16_t)(((uint64_t)&isr1 & 0x000000000000ffff));
    _idt[1].offset_mid = (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 16);
    _idt[1].offset_high = (uint32_t)(((uint64_t)&isr1 & 0xffffffff00000000) >> 32);
    _idt[1].ist = 0;
    _idt[1].selector = 0x08;   //Code segment selector defined in gdt
    _idt[1].types_attr = 0x8e; //Indicates it as a 32 bit interupt gate

    printf("Remapping PIC\n");

    remap_pic();

    printf("Reprogramming PIC chip\n");

    //Reprogram the PIC's with some magic code
    outb(PIC1_DATA, 0xfd);
    outb(PIC2_DATA, 0xff);

    printf("Loading IDT\n");

    // Load IDT and reenable interrupts
    load_IDT();
}

void (*main_keyboard_handler)(uint8_t, char); // This was a pain in the ass. Turns out function pointers are a definition + declartion so extern is required in header file

extern void isr1_handler()
{
    uint8_t scan_code = inb(0x60);
    uint8_t chr = 0;

    if (scan_code < 0x3a)
    {
        chr = scan_code_lookup_table[scan_code];
    }

    if (main_keyboard_handler != 0) //function pointer is not null
    {
        main_keyboard_handler(scan_code, chr);
    }

    //Acknowledge the interrupt
    outb(PIC1_COMMAND, 0x20);
    outb(PIC2_COMMAND, 0x20);
}
