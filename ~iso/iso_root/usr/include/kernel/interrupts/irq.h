#ifndef __KERNEL_INTERRUPTS_IRQ_H
#define __KERNEL_INTERRUPTS_IRQ_H 1

#include <stdbool.h>
#include <stdint.h>

void IRQs_install();

void IRQ_install_handler(int irq, void (*handler)(struct registers *r));
void IRQ_uninstall_handler(int irq);

/**
 * Use to determine if Interrupts are enabled
 * */
static inline bool IRQ_interrupts_enabled(void)
{
    unsigned long flags;
    asm volatile("pushf\n\t"
                 "pop %0"
                 : "=g"(flags));
    return flags & (1 << 9);
}

/**
 * Disables and returns the interrupt state
 * */
static inline uint64_t IRQ_stash_and_disable(void)
{
    uint64_t flags;
    asm volatile("pushf\n\tcli\n\tpop %0"
                 : "=r"(flags)
                 :
                 : "memory");
}

/**
 * Restores interrupts to there former state
 * */
static inline void IRQ_restore(uint64_t flags)
{
    asm volatile("push %0\n\tpopf"
                 :
                 : "rm"(flags)
                 : "memory", "cc");
}

/**
 * Unconditionally disables interrupts
 * */
static inline void IRQ_disable()
{
    asm volatile("cli");
}

/**
 * Unconditionally enables interrupts
 * */
static inline void IRQ_enable()
{
    asm volatile("sti");
}
#endif