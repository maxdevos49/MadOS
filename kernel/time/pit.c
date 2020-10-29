#include <kernel/time/pit.h>
#include <kernel/io.h>
#include <kernel/interrupts/irq.h>

// void PIT_periodic_frequency(int hz)
// {
//     int divisor = 1193180 / hz;              /* Calculate our divisor */
//     outb(PIT_COMMAND_PORT, 0x36);            //channel 0, hilobyte, square wave mode, 16-bit binary
//     outb(PIT_CH0_DATA_PORT, divisor & 0xFF); /* Set low byte of divisor */
//     outb(PIT_CH0_DATA_PORT, divisor >> 8);   /* Set high byte of divisor */
// }

// //TODO only reads channel 0. Also unsure if needed
// uint32_t PIT_read_count()
// {
//     uint64_t f = IRQ_stash_and_disable();

//     outb(PIT_COMMAND_PORT, PIT_CHANNEL_0 | PIT_ACCESS_LATCH); //select channel 0 with the latch command

//     uint8_t low_byte = inb(PIT_CH0_DATA_PORT);
//     uint8_t high_byte = inb(PIT_CH0_DATA_PORT);

//     IRQ_restore(f);

//     return ((uint16_t)high_byte << 8) | (uint16_t)low_byte;
// }

void PIT_configure(uint8_t channel_port, uint8_t mode, uint16_t hz)
{
    uint64_t f = IRQ_stash_and_disable();

    outb(PIT_COMMAND_PORT, ((channel_port & PIT_CH0_DATA_PORT)
                                ? PIT_CHANNEL_0
                                : PIT_CHANNEL_2) |
                               PIT_ACCESS_HILOBYTE | mode);

    //set frequency
    uint32_t divisor = PIT_BASE_FREQUENCY / hz;
    outb(channel_port, divisor & 0xff);
    outb(channel_port, divisor >> 8);

    IRQ_restore(f);
}

void PIT_enable_periodic_irq0()
{
    outb(PIT_COMMAND_PORT, PIT_CHANNEL_0 | PIT_MODE_SQUAREGEN);
}

void PIT_disable_periodic_irq0()
{
    PIT_configure(PIT_CH0_DATA_PORT, PIT_MODE_COUNTDOWN, 1);
}

void PIT_set_periodic_frequency(uint16_t hz)
{
    PIT_configure(PIT_CH0_DATA_PORT, PIT_MODE_SQUAREGEN, hz);
}

uint16_t PIT_get_closest_frequency(uint16_t hz)
{
    //TODO
}