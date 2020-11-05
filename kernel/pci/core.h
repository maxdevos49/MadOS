#ifndef __KERNEL_PCI_PCI_CORE_H
#define __KERNEL_PCI_PCI_CORE_H 1

//https://www.oreilly.com/library/view/linux-device-drivers/0596005903/ch12.html

#include <kernel/pci/driver.h>
#include <kernel/pci/device.h>

#include <stdint.h>

enum pci_io{
    CONFIG_ADDRESS_PORT = 0xCF8,
    CONFIG_DATA_PORT = 0xCFC
};

int PCI_register_driver(struct pci_driver *driver);
int PCI_unregister_driver(struct pci_driver *driver);

//Dont call in a interrupt i guess
struct pci_dev *PCI_get_device(uint32_t vendor, uint32_t device, struct pci_dev *from);
struct pci_dev *PCI_get_subsys(uint32_t vendor, uint32_t device, uint32_t ss_vendor, uint32_t ss_device, struct pci_dev *from);
struct pci_dev *PCI_get_slot(uint8_t bus, uint8_t func);

int PCI_enable_device(struct pci_dev *dev);
uint64_t PCI_resource_start(struct pci_dev *dev, int bar);
uint64_t PCI_resource_end(struct pci_dev *dev, int bar);
uint64_t PCI_resource_flags(struct pci_dev *dev, int bar);

int PCI_read(struct pci_dev *dev, int offset, uint32_t *val);
int PCI_write(struct pci_dev *dev, int offset, uint32_t val);

static inline int PCI_read_config_byte(struct pci_dev *dev, uint8_t offset, uint8_t *val)
{
    return PCI_read(dev, offset, (uint32_t *)val);
}
static inline int PCI_read_config_word(struct pci_dev *dev, uint8_t offset, uint16_t *val)
{
    return PCI_read(dev, offset, (uint32_t *)val);
}
static inline int PCI_read_config_dword(struct pci_dev *dev, uint8_t offset, uint32_t *val)
{
    return PCI_read(dev, offset, val);
}

static inline int PCI_write_config_byte(struct pci_dev *dev, uint8_t offset, uint8_t val)
{
    return PCI_write(dev, offset, (uint32_t)val);
}
static inline int PCI_write_config_word(struct pci_dev *dev, uint8_t offset, uint16_t val)
{
    return PCI_write(dev, offset, (uint32_t)val);
}
static inline int PCI_write_config_dword(struct pci_dev *dev, uint8_t offset, uint32_t val)
{
    return PCI_write(dev, offset, val);
}

#endif