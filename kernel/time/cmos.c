#include <kernel/time/cmos.h>
#include <kernel/io.h>
#include <kernel/interrupts/idt.h>

static uint8_t century_register = CMOS_CENTURY_REGISTER_DEFAULT;
static bool disable_nmi = CMOS_DISABLE_NMI;

/**
 * Disables or enables NMI(Non maskable interrupts)
 * */
void CMOS_disable_NMI(bool disable)
{
    disable_nmi = disable;
}

/**
 * Sets the register to use to find the current century reported by the RTC. 
 * This is non standard but reported by acpi table
 * */
void CMOS_set_century_register(uint8_t reg)
{
    century_register = reg;
}

uint8_t CMOS_get_century_register()
{
    return century_register;
}

/**
 * Reads then entire CMOS ram into a buffer
 * */
void CMOS_read(uint8_t data[128])
{
    uint8_t index;
    for (index = 0; index < 128; index++)
    {
        interrupts_disable();

        if (!disable_nmi)
            outb(CMOS_COMMAND_PORT, index);
        else
            outb(CMOS_COMMAND_PORT, index | 0x80);

        data[index] = inb(CMOS_DATA_PORT);

        interrupts_enable();
    }
}

/**
 * Writes to the entire CMOS ram from a buffer. Dont use unless 
 * you know what you are doing. Could result in BIOS checksum issues
 * */
void CMOS_write(uint8_t data[128])
{
    uint8_t index;
    for (index = 0; index < 128; index++)
    {
        interrupts_disable();

        if (!disable_nmi)
            outb(CMOS_COMMAND_PORT, index);
        else
            outb(CMOS_COMMAND_PORT, index | 0x80);

        outb(CMOS_DATA_PORT, data[index]);

        interrupts_enable();
    }
}

/**
 * Reads a register from the CMOS ram. Correctly takes into account 
 * the NMI setting.
 * */
uint8_t CMOS_read_register(uint8_t reg)
{
    if (!disable_nmi)
        reg = reg | 0x80;

    outb(CMOS_COMMAND_PORT, reg);
    return inb(CMOS_DATA_PORT);
}

/**
 * Writes to a register in the CMOS ram. Correctly takes into account
 * the NMI setting
 * */
void CMOS_write_register(uint8_t reg, uint8_t value)
{
    if (!disable_nmi)
        reg = reg | 0x80;

    outb(CMOS_COMMAND_PORT, reg);
    outb(CMOS_DATA_PORT, value);
}
