#include <kernel/io.h>
#include <kernel/interrupts/idt.h>

#include <stdio.h>

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

void irq_install()
{
    printf("Installing IRQ Gates\n");

    idt_set_gate(32, (uint64_t)irq0, 0x08, 0x8e);
    idt_set_gate(33, (uint64_t)irq1, 0x08, 0x8e); //keyboard
    idt_set_gate(34, (uint64_t)irq2, 0x08, 0x8e);
    idt_set_gate(35, (uint64_t)irq3, 0x08, 0x8e);
    idt_set_gate(36, (uint64_t)irq4, 0x08, 0x8e);
    idt_set_gate(37, (uint64_t)irq5, 0x08, 0x8e);
    idt_set_gate(38, (uint64_t)irq6, 0x08, 0x8e);
    idt_set_gate(39, (uint64_t)irq7, 0x08, 0x8e);
    idt_set_gate(40, (uint64_t)irq8, 0x08, 0x8e);
    idt_set_gate(41, (uint64_t)irq9, 0x08, 0x8e);
    idt_set_gate(42, (uint64_t)irq10, 0x08, 0x8e);
    idt_set_gate(43, (uint64_t)irq11, 0x08, 0x8e);
    idt_set_gate(44, (uint64_t)irq12, 0x08, 0x8e);
    idt_set_gate(45, (uint64_t)irq13, 0x08, 0x8e);
    idt_set_gate(46, (uint64_t)irq14, 0x08, 0x8e);
    idt_set_gate(47, (uint64_t)irq15, 0x08, 0x8e);
}

void irq_handler(struct registers *regs)
{
    //Blank function pointer
    void (*handler)(struct registers *);

    //Check for a custom handler
    handler = irq_routines[regs->int_num - 32]; //-32 because routines are 0-15

    if (handler)
    {
        handler(regs);
    }

    //if IDT entry that was invoked was higher then 40(IRQ8-15),
    // then we need to send an EOI to the slave controller
    if (regs->int_num >= 40)
    {
        outb(0xA0, 0x20);
    }

    //Any case an eoi must be sent to the master pic
    outb(0x20, 0x20);
}