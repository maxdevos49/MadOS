#include "io.h"

void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t return_val;

    asm volatile("inb %1, %0"
                 : "=a"(return_val)
                 : "Nd"(port));

    return return_val;
}

static inline void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
    /* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}

void remap_pic()
{

    // uint8_t a1, a2;

    // store the masked value of each PIC
    // a1 = inb(PIC1_DATA);
    // a2 = inb(PIC2_DATA);

    //Tell PIC's to start the initialization sequence
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    //Tell each PIC there offset
    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    //
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW1_8086);
    io_wait();
    outb(PIC2_DATA, ICW1_8086);
    io_wait();

    //restore the saved mask of the PIC
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}