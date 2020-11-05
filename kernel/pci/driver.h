#ifndef __KERNEL_PCI_PCI_DRIVER_H
#define __KERNEL_PCI_PCI_DRIVER_H 1

#include <stdint.h>

struct pci_driver
{
    const char* name;
    // const struct pci_device_id id_table;
    
    //Called by PCI core when it thinks a PCI driver wants to control it
    int (*probe) (struct pci_dev *dev, const struct pci_device_id *id);

    //PCI core calls when the device is removed from the system or unloaded from the kernel
    int (*remove)(struct pci_dev *dev, uint32_t state);

    //(Optional)PCI core calls when the device is suspended. 
    int (*suspend)(struct pci_dev *dev, uint32_t state);

    //(Optional)PCI core calls after it a device is no longer suspended
    int (*resume)(struct pci_dev *dev);

};

#endif