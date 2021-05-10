#ifndef __KERNEL_PCI_PCI_DEVICE_H
#define __KERNEL_PCI_PCI_DEVICE_H 1

#include <kernel/pci/driver.h>
#include <kernel/pci/bus.h>
#include <kernel/list.h>

#include <stdint.h>

enum pci_dev_standard_registers
{
    VENDOR_ID = 0x02,
    DEVICE_ID = 0x00,

    STATUS = 0x04,
    COMMAND = 0x06,

    CLASS_CODE = 0x08,
    SUB_CLASS_CODE = 0x09,
    PROG_IF = 0x0a,
    REVISION_ID = 0x0b,

    BIST = 0x0c,
    HEADER = 0x0d,
    LATENCY_TIMER = 0x0e,
    CACHE_LINE_SIZE = 0x0f,

    BAR0 = 0x10,
    BAR1 = 0x14,
    BAR2 = 0x18,
    BAR3 = 0x1c,
    BAR4 = 0x20,
    BAR5 = 0x24,

    SUBSYSTEM_DEVICE_ID = 0x2c,
    SUBSYSTEM_VENDOR_ID = 0x2e,

    INTERRUPT_PIN = 0x3e,
    INTERRUPT_LINE = 0x3f

};

struct pci_dev;

struct pci_dev
{
    uint16_t vendor_id;
    uint16_t device_id;

    uint16_t subsystem_vendor_id;
    uint16_t subsystem_device_id;

    uint8_t revision_id;
    uint8_t slot;

    uint8_t func;
    uint8_t class_code;

    uint8_t subclass_code;
    uint8_t prog_if;

    uint8_t multi_function : 1;
    uint8_t header_type : 3;

    uint8_t bist_capable : 1;
    // uint32_t bist_start: 1;
    // uint32_t bist_completion_code: 4;

    uint8_t latency_timer;   //os 0x0e
    uint8_t cache_line_size; //os 0x0f

    uint8_t interrupt_line;
    uint8_t interrupt_pin;

    struct pci_driver *driver;

    struct list_head node;
    struct list_head bus_list; //TODO how is this used?

    struct pci_bus *bus;
    struct pci_bus *subordinate; //TODO What is this
};

#endif