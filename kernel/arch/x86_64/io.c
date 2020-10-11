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

void remap_pic()
{

    uint8_t a1, a2;

    //store the masked value of each PIC
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    //Tell PIC's to start the initialization sequence
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    //Tell each PIC there offset
    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 8);

    //
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    outb(PIC1_DATA, ICW1_8086);
    outb(PIC2_DATA, ICW1_8086);

    //restore the saved mask of the PIC
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}