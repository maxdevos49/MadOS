#include <kernel/pci/core.h>
#include <kernel/pci/device.h>
#include <kernel/pci/bus.h>
#include <kernel/io.h>

#include <stdint.h>


int PCI_read(struct pci_dev *dev, int offset, uint32_t *val)//todo size/offset
{
    uint32_t address;
    uint32_t l_bus = (uint32_t)dev->bus->number << 16;
    uint32_t l_slot = (uint32_t)dev->slot << 11;
    uint32_t l_func = (uint32_t)dev->func << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | (offset & 0xfc) | ((uint32_t)0x80000000));

    /*Create a configuration address and write it*/
    outl(CONFIG_ADDRESS_PORT, address);

    *val = inl(CONFIG_DATA_PORT);

    return 0;
}

int PCI_write(struct pci_dev *dev, int offset, uint32_t val)//TODO size
{
    uint32_t address;
    uint32_t l_bus = (uint32_t)dev->bus->number << 16;
    uint32_t l_slot = (uint32_t)dev->slot << 11;
    uint32_t l_func = (uint32_t)dev->func << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | (offset & 0xfc) | ((uint32_t)0x80000000));

    /*Create a configuration address and write it*/
    outl(CONFIG_ADDRESS_PORT, address);

    outl(CONFIG_DATA_PORT, val);

    return 0;
}