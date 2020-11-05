#ifndef __KERNEL_PCI_BUS_H
#define __KERNEL_PCI_BUS_H 1

#include <kernel/pci/device.h>

struct pci_bus
{
    // struct list_head node;
    // struct pci_bus *parent;
    // struct pci_bus *parent;    /* Parent bus this bridge is on */
    // struct list_head children; /* List of child buses */
    // struct list_head devices;  /* List of devices on this bus */
    struct pci_dev *self; /* Bridge device as seen by parent */
    // struct list_head slots;
    struct pci_dev *dev;
    uint8_t number;
};

#endif