#ifndef __KERNEL_PCI_BUS_H
#define __KERNEL_PCI_BUS_H 1

#include <kernel/pci/device.h>
#include <kernel/list.h>

struct pci_bus
{
    uint32_t number;

    struct list_head node;
    struct list_head children; /* List of child buses */
    struct list_head devices;  /* List of devices on this bus */
    struct list_head slots;//TODO what is this?
    struct pci_bus *parent; /* Parent bus this bridge is on */
    struct pci_dev *self;   /* Bridge device as seen by parent */


    struct pci_dev *dev; //TODO what is this
};

#endif