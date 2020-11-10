/**
 * PCI Core
 * 
 * The purpose of the PCI core is not to be a driver. It is meant to 
 * enumerate all the devices attached to the PCI bus, configure them
 * if needed, and assign a driver if present. It also provides some 
 * standardized functions to access common functionality among all
 * pci devices like reading or writing of the configuration data 
 * header.
 * 
 * Resources: 
 * 1. https://www.oreilly.com/library/view/linux-device-drivers/0596005903/ch12.html
 * 2. https://tldp.org/LDP/tlk/dd/pci.html
 * 3. https://wiki.osdev.org/PCI
 * 
 * Interesting reads I found while researching PCI
 * 1. http://www.catb.org/esr/structure-packing/
 * 
 * */
#include <kernel/pci/core.h>
#include <kernel/pci/device.h>
#include <kernel/pci/bus.h>
#include <kernel/io.h>
#include <kernel/list.h>

#include <stdint.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <stdio.h>

#define PCI_PROBE_ASSERT(test, offset)                                \
    if (test)                                                         \
    {                                                                 \
        printf("PCI config read/write Failed. Offset: %x\n", offset); \
    }

static struct pci_bus *root_bus;
static int bus;

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

static const char *PCI_header_type[] = {
    "PCI Standard Header",
    "PCI to PCI bridge Header"
    "PCI to Cardbus Header"};

/**
 * Returns non zero value if vendor is not valid
 * */
static inline int PCI_check_vendor(struct pci_bus *bus, int slot, int function)
{
    uint16_t *val = NULL;

    PCI_read(bus->number, slot, function, 0x02, sizeof(short), (uint32_t *)val);

    return (*val == 0xffff);
}

/**
 * Returns non zero value if not multifunction
 * */
static inline int PCI_check_multifunction(struct pci_bus *bus, int slot)
{
    uint8_t *val = NULL;
    PCI_read(bus->number, slot, 0, 0x0d, sizeof(char), (uint32_t *)val);

    return (*val & 0x80) == 0;
}

static void PCI_describe_device(struct pci_dev *dev)
{
    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
    printf("B: %d, S: %d, F: %d, Vend: %x, Dev: %x, Class: %s, SC: %x\n", dev->bus->number, dev->slot, dev->func, dev->vendor_id, dev->device_id, PCI_class_codes[dev->class_code], dev->subclass_code);
    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    // TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
    // printf("Bus #: %d, Slot #: %d, Function: %d\n", dev->bus->number, dev->slot, dev->func);
    // TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_WHITE);


    // printf("\tDev ID: %x, Vend ID: %x, Header: %s, Multi Func: %x\n", dev->device_id, dev->vendor_id, PCI_header_type[dev->header_type], dev->multi_function); //Why is device_id == 0x0000
    // printf("\tSubsystem Device ID: %x, Subsystem Vendor ID: %x, Revision ID: %x\n", dev->subsystem_device_id, dev->subsystem_vendor_id, dev->revision_id); //Why is device_id == 0x0000
    // printf("\tClass: %s, Sub Class: %x, Prog IF %x, Bist Capable: %x\n", PCI_class_codes[dev->class_code], dev->subclass_code, dev->prog_if, dev->bist_capable);
    // printf("\tLatency Timer: %x, Cache Line Size: %x, Int Line: %x, Int Pin: %x\n", dev->latency_timer,dev->cache_line_size, dev->interrupt_line, dev->interrupt_pin);
    // TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
}

/**
 * Probes information about a PCI device
 * */
static struct pci_dev *PCI_probe_device(struct pci_bus *bus, int slot, int function)
{
    struct pci_dev *dev = malloc(sizeof(struct pci_dev));

    INIT_LIST_HEAD(&dev->node);
    list_add(&bus->devices, &dev->node);

    dev->bus = bus;
    dev->slot = slot;
    dev->func = function;

    PCI_PROBE_ASSERT(PCI_read_config_word(dev, 0x00, &dev->device_id), 0x00);
    PCI_PROBE_ASSERT(PCI_read_config_word(dev, 0x02, &dev->vendor_id), 0x02);

    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x08, &dev->class_code), 0x08);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x09, &dev->subclass_code), 0x09);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x0a, &dev->prog_if), 0x0a);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x0b, &dev->revision_id), 0x0b);

    PCI_PROBE_ASSERT(PCI_read_config_word(dev, 0x2c, &dev->subsystem_device_id), 0x2c);
    PCI_PROBE_ASSERT(PCI_read_config_word(dev, 0x2e, &dev->subsystem_vendor_id), 0x2e);

    uint8_t header_type;
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x0c, &header_type), 0x0c);
    dev->header_type = (header_type & 0x03);
    dev->multi_function = (header_type & 0x80) >> 7;

    uint8_t bist;
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x0c, &bist), 0x0c);
    dev->bist_capable = (bist & 0x80) >> 7;

    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x0e, &dev->latency_timer), 0x0e);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x0f, &dev->cache_line_size), 0x0f);

    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x3e, &dev->interrupt_pin), 0x3e);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, 0x3f, &dev->interrupt_line), 0x3f);

    PCI_describe_device(dev);

    return dev;
}

static void PCI_enumerate_bus(struct pci_bus *bus)
{

    for (int slot = 0; slot < 32; slot++)
    {
        //valid vendor ID?
        if (PCI_check_vendor(bus, slot, 0))
            continue;

        //valid device so probe it
        struct pci_dev *dev = PCI_probe_device(bus, slot, 0);

        //determine if needed to check for other functions
        if (!dev->multi_function)
            continue;

        //TODO is a device a bus?

        for (int function = 1; function < 8; function++)
        {
            //Not all functions are required to be implemented
            if (PCI_check_vendor(bus, slot, function))
                continue;

            //populate new device info
            dev = PCI_probe_device(bus, slot, function);

            list_add(&bus->devices, &dev->node);

            //     //TODO is a device a bus?
        }
    }
}

void PCI_configure()
{
    bus = 0;
    root_bus = malloc(sizeof(struct pci_bus));

    root_bus->number = bus;
    root_bus->parent = NULL;

    INIT_LIST_HEAD(&root_bus->node);
    INIT_LIST_HEAD(&root_bus->children);
    INIT_LIST_HEAD(&root_bus->devices);

    //TODO get init bus device or is this NULL?
    PCI_enumerate_bus(root_bus);
}

//x86 specific
int PCI_read(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset, size_t size, uint32_t *val)
{
    uint32_t address;
    uint32_t l_bus = ((uint32_t)bus) << 16;
    uint32_t l_slot = ((uint32_t)slot) << 11;
    uint32_t l_func = ((uint32_t)func) << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | ((uint32_t)(offset & 0xfc)) | ((uint32_t)0x80000000));

    outl(CONFIG_ADDRESS_PORT, address);

    uint32_t bitmask = 0xffffffff;
    uint8_t shift_offset = 0;

    if (size == sizeof(char))
    {
        switch (offset % 4)
        {
        case 0: //0x00
            bitmask = 0xff000000;
            shift_offset = 24;
            break;
        case 1: //0x01
            bitmask = 0x00ff0000;
            shift_offset = 16;
            break;
        case 2: //0x02
            bitmask = 0x0000ff00;
            shift_offset = 8;
            break;
        case 3: //0x03
            bitmask = 0x000000ff;
            shift_offset = 0;
            break;
        default:
            //Should never happen but incase it does we can report it
            return 1;
            break;
        }
    }
    else if (size == sizeof(short))
    {
        switch (offset % 4)
        {
        case 0: //0x00
            bitmask = 0xffff0000;
            shift_offset = 16;
            break;
        case 2: //0x02
            bitmask = 0x0000ffff;
            shift_offset = 0;
            break;
        default:
            return 2;
            break;
        }
    }
    else if (size == sizeof(int))
    {
        if ((offset % 4) != 0)
            return 4;

        bitmask = 0xffffffff;
        shift_offset = 0;
    }

    *val = (uint32_t)((inl(CONFIG_DATA_PORT) & bitmask) >> shift_offset);

    return 0;
}

//x86 specific
int PCI_write(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset, size_t size, uint32_t val)
{
    uint32_t address;
    uint32_t l_bus = (uint32_t)bus << 16;
    uint32_t l_slot = (uint32_t)slot << 11;
    uint32_t l_func = (uint32_t)func << 8;

    address = (uint32_t)(l_bus | l_slot | l_func | (offset & 0xfc) | ((uint32_t)0x80000000));

    if (size == 1)
    {
        uint32_t *old_val = NULL;

        PCI_read(bus, slot, func, offset, sizeof(int), old_val);
        *old_val &= 0xffffff00;
        val |= *old_val; //Dont clobber old values we are not targeting

        outl(CONFIG_ADDRESS_PORT, address);
        outl(CONFIG_DATA_PORT, val);
    }
    else if (size == 2)
    {
        uint32_t *old_val = NULL;

        PCI_read(bus, slot, func, offset, sizeof(int), old_val);
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