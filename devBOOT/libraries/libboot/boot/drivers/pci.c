#include "pci.h"

#include "../stdio.h"

static const char *PCI_class_codes[] = {
    "Unclassified",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge Device",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device Controller",
    "Docking Station",
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent Controller",
    "Satelite Communication Controller",
    "Encryption Controller",
    "Signal Processing Controller",
    "Processing Accelerator",
    "Non Essential Instrumentation",
    "Reserved",
    "Co-Processor",
    "Reserved",
    "Unassigned Class"};

static void PCI_check_bus(uint32_t bus);
static void PCI_check_device(uint32_t bus, uint32_t device);
static void PCI_check_function(uint32_t bus, uint32_t device, uint8_t function);

void PCI_configuration(PCI_DRIVER *driver_list, uint32_t size)
{
    printf("Enumerating PCI Bus:\n");

    for (int i = 0; i < 256; i++)
        PCI_check_bus(i);

    return;
}

static void PCI_check_bus(uint32_t bus)
{
    uint32_t device = 0;

    for (device = 0; device < 32; device++)
        PCI_check_device(bus, device);
}

static void PCI_check_device(uint32_t bus, uint32_t device)
{
    uint8_t function = 0;
    uint8_t header_type;

    uint16_t vendor_id = PCI_read_config_word(bus, device, function, VENDOR_ID);
    if (vendor_id == 0xffff)
        return;

    printf("Bus: %d / Device: %d\n", bus, device);
    PCI_check_function(bus, device, function);

    header_type = PCI_read_config_byte(bus, device, function, HEADER_TYPE);
    if (PCI_is_multi_function(header_type))
    {
        for (function = 1; function < 8; function++)
        {
            vendor_id = PCI_read_config_word(bus, device, function, VENDOR_ID);

            if (vendor_id != 0xffff)
                PCI_check_function(bus, device, function);
        }
    }
}

static void PCI_check_function(uint32_t bus, uint32_t device, uint8_t function)
{
    uint16_t vendor_id = PCI_read_config_word(bus, device, function, VENDOR_ID);
    uint16_t device_id = PCI_read_config_word(bus, device, function, DEVICE_ID);

    uint8_t revision_id = PCI_read_config_byte(bus, device, function, REVISION_ID);
    uint8_t prog_if = PCI_read_config_byte(bus, device, function, PROG_IF);
    uint8_t sub_class = PCI_read_config_byte(bus, device, function, SUB_CLASS);
    uint8_t class_code = PCI_read_config_byte(bus, device, function, CLASS_CODE);
    printf("\tF: %d / %04x:%04x / %s / %02x / %02x\n",function, vendor_id, device_id, PCI_class_codes[class_code],sub_class, prog_if);

    // uint8_t
}