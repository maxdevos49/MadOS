#ifndef __KERNEL_PCI_PCI_DEVICE_H
#define __KERNEL_PCI_PCI_DEVICE_H 1

#include <stdint.h>
#include <kernel/pci/driver.h>
#include <kernel/pci/bus.h>

struct pci_dev
{
    // struct list_head bus_list;//TODO whats is this?
    struct pci_bus *bus;
    struct pci_bus *subordinate; //Is this needed?
    uint8_t slot;
    uint32_t func;
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;
    uint8_t revision_id;

    uint32_t class_code;
    struct pci_driver *driver;
};



#endif