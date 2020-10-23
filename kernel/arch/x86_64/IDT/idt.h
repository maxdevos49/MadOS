#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>

struct registers
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsp, rdi, rsi, rdx, rcx, rbx, rax;
    uint8_t int_num, err_code;
    uint64_t rip, cs, eflags, useresp, ss;
};

struct IDT64
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
};

//idt.c
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);
void idt_install();

//isr.c
void isrs_install(void);

//irq.c
void irq_install();
void irq_install_handler(int irq, void (*handler)(struct registers *r));
void irq_uninstall_handler(int irq);

#endif