#include "ata.h"

#include "stdint.h"

struct IDEChannelRegisters
{
    uint16_t base;  // I/O Base.
    uint16_t ctrl;  // Control Base
    uint16_t bmide; // Bus Master IDE
    uint8_t nIEN;   // nIEN (No Interrupt);
} channels[2];

unsigned char ide_buf[2048] = {0};
unsigned static char ide_irq_invoked = 0;
unsigned static char atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};