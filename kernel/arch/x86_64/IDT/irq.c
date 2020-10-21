#include "../io.h"
#include "idt.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

//Function pointers to handle custom IRQ handlers for given IRQ
void *irq_routines[16] =
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0};

void irq_install_handler(int irq, void (*handler)(struct registers *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

void irq_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irq_install()
{
    irq_remap();
    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8e);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8e);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8e);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8e);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8e);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8e);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8e);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8e);
    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8e);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8e);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8e);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8e);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8e);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8e);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8e);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8e);
    
    // asm volatile ("sti");
}

void irq_handler(struct registers *r)
{
    //Blank function pointer
    void (*handler)(struct registers * r);

    //Check for a custom handler
    handler = irq_routines[r->int_no - 32]; //-32 because routines are 0-15
    if (handler)
    {
        handler(r);
    }

    //if IDT entry that was invoked was higher then 40(IRQ8-15),
    // then we need to send an EOI to the slave controller
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    //Any case an eoi must be sent to the master pic 
    outb(0x20, 0x20);
}