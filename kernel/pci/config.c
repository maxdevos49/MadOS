#include <kernel/pci/pci.h>
#include <kernel/io.h>

#include <kernel/timer.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void PCI_install()
{
    printf("PCI Info:\n");

    for (int bus = 0; bus < 256; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            uint32_t reg_0 = PCI_config_read_dw(bus, device, 0, 0);

            if ((reg_0 & 0x0000ffff) != 0xffff)
            {
                printf("Bus #: %d, Device #: %d, Vendor id: %x\n", bus, device);
                printf("\tDevice ID: %x, Vendor ID: %x\n", reg_0 & 0xffff0000 >> 16, reg_0 & 0x0000ffff);

                uint32_t reg_2 = PCI_config_read_dw(bus, device, 0, 0x08);

                printf("\tClass Code: %x, Class Code Name: %s\n", reg_2 & 0xff000000 >> 24, PCI_class_codes[reg_2 & 0xff000000 >> 24]);
            }
        }
    }
}

/**
 * Reads a DW from a PCI configuration header
 * */
uint32_t PCI_config_read_dw(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t l_bus = (uint32_t)bus << 16;
    uint32_t l_slot = (uint32_t)slot << 11;
    uint32_t l_func = (uint32_t)func << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | (offset & 0xfc) | ((uint32_t)0x80000000));

    /*Create a configuration address and write it*/
    outl(PCI_CONFIG_ADDRESS_PORT, address);

    return inl(PCI_CONFIG_DATA_PORT);
}

/**
 * Writes a DW to a PCI configuration header
 * */
void PCI_config_write_dw(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data)
{
    uint32_t address;
    uint32_t l_bus = (uint32_t)bus << 16;
    uint32_t l_slot = (uint32_t)slot << 11;
    uint32_t l_func = (uint32_t)func << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | (offset & 0xfc) | ((uint32_t)0x80000000));

    outl(PCI_CONFIG_ADDRESS_PORT, address);

    outl(PCI_CONFIG_DATA_PORT, data);
}

/**
 * Check to see if a device is present in a slot
 * */
// bool PCI_check_device(uint8_t bus, uint8_t device)
// {
//     uint8_t function = 0;
//     uint16_t vendor_id = 0xffff;

// }

// //TODO what is this
// uint16_t PCI_check_vendor(uint8_t bus, uint8_t slot)
// {
//     uint16_t vendor, device;

//     if ((vendor = PCI_config_read_dw(bus, slot, 0, 0) & 0x0000ffff) != 0xffff)
//     {
//         device = PCI_config_read_dw(bus, slot, 0, 2);

//         //TODO
//         //Device found now do something maybe??
//     }

//     return vendor;
// }