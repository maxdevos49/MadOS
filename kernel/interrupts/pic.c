#include <kernel/interrupts/pic.h>
#include <kernel/io.h>

void PIC_remap()
{

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
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}