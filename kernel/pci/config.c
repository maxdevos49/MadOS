#include <kernel/pci/pci.h>
#include <kernel/io.h>

#include <stdint.h>

uint16_t PCI_config_readw(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t l_bus = (uint32_t)bus << 16;
    uint32_t l_slot = (uint32_t)slot << 11;
    uint32_t l_func = (uint32_t)func << 8;

    /*Create a configuration address and write it*/
    outl(PCI_CONFIG_ADDRESS_PORT, 
    l_bus 
    | l_slot 
    | l_func 
    | (offset & 0xfc) 
    | ((uint32_t)0x80000000)
    );

    return (uint16_t)(
        (inl(PCI_CONFIG_DATA_PORT) >> 
        ((offset & 2) * 8) ) & 0xffff);
}

uint16_t PCI_check_vendor(uint8_t bus, uint8_t slot)
{
    uint16_t vendor, device;

    if((vendor = PCI_config_readw(bus,slot, 0,0)) != 0xffff)
    {
        device = PCI_config_readw(bus,slot,0,2);

        //TODO
        //Device found now do something maybe??
    }

    return vendor;
}