#ifndef __KERNEL_INTERRUPTS_PIC_H
#define __KERNEL_INTERRUPTS_PIC_H 1

#define PIC1_COMMAND 0x20 //Master PIC
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0 //Slave PIC
#define PIC2_DATA 0xa1

#define ICW1_INIT 0x10 //Initialization control word one
#define ICW1_ICW4 0x01
#define ICW1_8086 0x01

void PIC_remap();

#endif