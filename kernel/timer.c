#include <kernel/timer.h>
#include <kernel/interrupts/irq.h>
#include <kernel/time/pit.h>

#include <stdint.h>
#include <stdio.h>

static volatile uint64_t cpu_ticks = 0;

void timer_handler()
{
    cpu_ticks++;

    if (cpu_ticks % 100 == 0)
    {
        printf(".");
    }
}

void sleep_ticks(uint64_t ticks)
{
    uint64_t end_ticks = ticks + cpu_ticks;

    while (cpu_ticks < end_ticks)
        ;
}

void sleep_milliseconds(uint64_t milliseconds)
{
    sleep_ticks(milliseconds);
}

void Timer_install()
{
    PIT_set_periodic_frequency(100);
    IRQ_install_handler(0, timer_handler);
}