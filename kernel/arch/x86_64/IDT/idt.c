#include <stdint.h>
#include <string.h>
#include "idt.h"

extern struct IDT64 _idt[256]; // 4096 byte IDT
extern void load_IDT();

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags)
{
    _idt[num].zero = 0;
    _idt[num].offset_low = (uint16_t)(((uint64_t)&base & 0x000000000000ffff));
    _idt[num].offset_mid = (uint16_t)(((uint64_t)&base & 0x00000000ffff0000) >> 16);
    _idt[num].offset_high = (uint32_t)(((uint64_t)&base & 0xffffffff00000000) >> 32);
    _idt[num].ist = 0;
    _idt[num].selector = sel;     //Code segment selector defined in gdt
    _idt[num].types_attr = flags; //Indicates it as a 32 bit interupt gate
}

void idt_install()
{
    //Clear out entire IDT with zeros
    memset(&_idt, 0, sizeof(struct IDT64) * 256);

    load_IDT();
}