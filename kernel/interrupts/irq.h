#ifndef __KERNEL_INTERRUPTS_IRQ_H
#define __KERNEL_INTERRUPTS_IRQ_H 1

void irq_install();
void irq_install_handler(int irq, void (*handler)(struct registers *r));
void irq_uninstall_handler(int irq);

#endif