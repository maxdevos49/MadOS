#include <kernel/pci/core.h>
#include <kernel/pci/device.h>
#include <kernel/pci/bus.h>
#include <kernel/io.h>

#include <stdint.h>
#include <stdlib.h>

struct pci_root
{ //TODO figure out actual structure
    struct pci_bus *root_bus;
    struct pci_dev *root_dev;

    struct pci_bus bus[256];
    struct pci_dev device[100];
};

static struct pci_root *pci_root;

void PCI_configure()
{
    pci_root = malloc(sizeof(pci_root));

    for (int bus = 0; bus < 256; bus++) //this is brutforce. We are going to do the recursive discover by bus method. aka smarter but slightly more complex
    {
        for (int device = 0; device < 32; device++)
        {

            for (int function = 0; function < 8; function++)
            {
            }
        }
    }
}

//x86 specific 
int PCI_read(struct pci_dev *dev, uint8_t offset, size_t size, uint32_t *val)
{
    uint32_t address;
    uint32_t l_bus = (uint32_t)dev->bus->number << 16;
    uint32_t l_slot = (uint32_t)dev->slot << 11;
    uint32_t l_func = (uint32_t)dev->func << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | (offset & 0xfc) | ((uint32_t)0x80000000));

    outl(CONFIG_ADDRESS_PORT, address);

    if (size == 1)
        *val = (uint8_t)inl(CONFIG_DATA_PORT);
    else if (size == 2)
        *val = (uint16_t)inl(CONFIG_DATA_PORT);
    else if (size == 3)
        *val = inl(CONFIG_DATA_PORT);
    else
        return 1;

    return 0;
}

//x86 specific
int PCI_write(struct pci_dev *dev, uint8_t offset, size_t size, uint32_t val) //TODO size
{
    uint32_t address;
    uint32_t l_bus = (uint32_t)dev->bus->number << 16;
    uint32_t l_slot = (uint32_t)dev->slot << 11;
    uint32_t l_func = (uint32_t)dev->func << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | (offset & 0xfc) | ((uint32_t)0x80000000));

    if (size == 1)
    {
        uint32_t *old_val = NULL;

        PCI_read(dev, offset, sizeof(int), old_val);
        *old_val &= 0xffffff00;
        val |= *old_val; //Dont clobber old values we are not targeting

        outl(CONFIG_ADDRESS_PORT, address);
        outl(CONFIG_DATA_PORT, val);
    }
    else if (size == 2)
    {
        uint32_t *old_val = NULL;

        PCI_read(dev, offset, sizeof(int), old_val);
        *old_val &= 0xffff0000;
        val |= *old_val; //Dont clobber old values we are not targeting

        outl(CONFIG_ADDRESS_PORT, address);
        outl(CONFIG_DATA_PORT, val);
    }
    else if (size == 3)
    {
        outl(CONFIG_ADDRESS_PORT, address);
        outl(CONFIG_DATA_PORT, val);
    }
    else
    {
        return 0;
    }

    return 0;
}