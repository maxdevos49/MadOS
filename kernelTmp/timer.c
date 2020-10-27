#include <kernel/timer.h>
#include <kernel/interrupts/idt.h>
#include <kernel/io.h>

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

    while (cpu_ticks < end_ticks);
}

void sleep_milliseconds(uint64_t milliseconds)
{

    sleep_ticks(milliseconds);
}

void install_timer()
{
    timer_phase(100);
    irq_install_handler(0, timer_handler);
}