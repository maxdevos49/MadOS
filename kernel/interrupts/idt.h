#ifndef __KERNEL_INTERRUPTS_IDT_H
#define __KERNEL_INTERRUPTS_IDT_H 1

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

void IDT_install();
void IDT_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);

#endif