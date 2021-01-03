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

#include <kernel/tty.h>
#include <kernel/vga.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#define PCI_PROBE_ASSERT(test, offset)                                \
    if (test)                                                         \
    {                                                                 \
        printf("PCI config read/write Failed. Offset: %x\n", offset); \
    }

static struct pci_bus *root_bus;

// static const char *PCI_class_codes[] = {
//     "Unclassified",
//     "Mass Storage Controller",
//     "Network Controller",
//     "Display Controller",
//     "Multimedia Controller",
//     "Memory Controller",
//     "Bridge Device",
//     "Simple Communication Controller",
//     "Base System Peripheral",
//     "Input Device Controller",
//     "Docking Station",
//     "Processor",
//     "Serial Bus Controller",
//     "Wireless Controller",
//     "Intelligent Controller",
//     "Satelite Communication Controller",
//     "Encryption Controller",
//     "Signal Processing Controller",
//     "Processing Accelerator",
//     "Non Essential Instrumentation",
//     "Reserved",
//     "Co-Processor",
//     "Reserved",
//     "Unassigned Class"};

static void PCI_enumerate_bus(struct pci_bus *bus);

/**
 * Returns non zero value if vendor is not valid
 * */
static inline int PCI_check_vendor(struct pci_bus *bus, int slot, int function)
{
    uint16_t *val = NULL;

    PCI_read(bus->number, slot, function, VENDOR_ID, sizeof(short), (uint32_t *)val);

    return (*val == 0xffff);
}

// static uint32_t PCI_check_bar_size(struct pci_dev *dev, int bar_num)
// {
//     uint32_t bar;
//     uint32_t size;

//     //Get old value
//     PCI_PROBE_ASSERT(PCI_read_config_dword(dev, (BAR0 + (bar_num * 4)), &bar), (BAR0 + (bar_num * 4)));
//     //write ones
//     PCI_PROBE_ASSERT(PCI_write_config_dword(dev, (BAR0 + (bar_num * 4)), 0xfffffff0 | bar), (BAR0 + (bar_num * 4)));
//     //get size
//     PCI_PROBE_ASSERT(PCI_read_config_dword(dev, (BAR0 + (bar_num * 4)), &size), (BAR0 + (bar_num * 4)));
//     //restore value
//     PCI_PROBE_ASSERT(PCI_write_config_dword(dev, (BAR0 + (bar_num * 4)), bar), (BAR0 + (bar_num * 4)));

//     return (~(size & 0xfffffff0)) + 1; //TODO enum mask
//     // return size;
// }

// static void PCI_describe_bar(struct pci_dev *dev)
// {
//     TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
//     if (dev->header_type == 0)
//     { //5 bars

//         uint32_t bar;
//         for (int i = 0; i < 6; i++)
//         {
//             PCI_PROBE_ASSERT(PCI_read_config_dword(dev, (BAR0 + (i * 4)), &bar), (BAR0 * (i * 4)));

//             if ((bar & 0x00000001) == 0) //TODO mask enum
//             {
//                 //Memory
//                 if (((bar & 0xfffffff0)) == 0)
//                     continue;

//                 printf("\tBar %d: Memory Base: %x, Prefetchable: %x, Type: %x, Size: %x\n", i, (bar & 0xFFFFFFF0), (bar & 0x00000008) != 0, (bar & 0x00000006), PCI_check_bar_size(dev, i));
//             }
//             else
//             {
//                 if (((bar & 0xfffffffc)) == 0)
//                     continue;

//                 //I/O Space
//                 printf("\tBar %d: I/O Base: %x\n", i, (bar & 0xfffffffc) >> 2);
//             }
//         }
//     }
//     else if (dev->header_type == 1)
//     {
//         uint32_t bar;

//         //2 bars
//         for (int i = 0; i < 3; i++)
//         {
//             PCI_PROBE_ASSERT(PCI_read_config_dword(dev, (BAR0 + (i * 4)), &bar), (BAR0 * (i * 4)));

//             if ((bar & 0x00000001) == 0)
//             {
//                 //Memory
//                 if (((bar & 0xfffffff0)) == 0)
//                     continue;

//                 printf("\tBar %d: Memory Base: %x, Prefetchable: %x, Type: %x, Size: %x\n", i, (bar & 0xfffffff0), (bar & 0x00000008) != 0, (bar & 0x00000006), PCI_check_bar_size(dev, i));
//             }
//             else
//             {
//                 if (((bar & 0xfffffffc)) == 0)
//                     continue;

//                 //I/O Space
//                 printf("\tBar %d: I/O Base: %x\n", i, (bar & 0xfffffffc) >> 2);
//             }
//         }
//     }
//     TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
// }

// static void PCI_describe_device(struct pci_dev *dev)
// {
//     TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
//     printf("B: %d, S: %d, F: %d, V: %x, D: %x, HT: %x, C: %s, SC: %x\n", dev->bus->number, dev->slot, dev->func, dev->vendor_id, dev->device_id, dev->header_type, PCI_class_codes[dev->class_code], dev->subclass_code);
//     TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
// }

static struct pci_bus *PCI_probe_bus(struct pci_dev *bus_dev)
{
    struct pci_bus *bus = malloc(sizeof(struct pci_bus));

    INIT_LIST_HEAD(&bus->node);
    INIT_LIST_HEAD(&bus->children);
    INIT_LIST_HEAD(&bus->devices);
    INIT_LIST_HEAD(&bus->slots);

    list_add(&bus->node, &bus_dev->bus->children);

    bus->parent = bus_dev->bus;
    bus->self = bus_dev;

    //TODO enum bus device
    PCI_PROBE_ASSERT(PCI_read_config_byte(bus_dev, 0x1b, &bus->number), 0x1b);
    return bus;
}
/**
 * Probes information about a PCI device
 * */
static struct pci_dev *PCI_probe_device(struct pci_bus *bus, int slot, int function)
{
    struct pci_dev *dev = malloc(sizeof(struct pci_dev));

    INIT_LIST_HEAD(&dev->node);
    list_add(&dev->node, &bus->devices);

    dev->bus = bus;
    dev->slot = slot;
    dev->func = function;

    PCI_PROBE_ASSERT(PCI_read_config_word(dev, DEVICE_ID, &dev->device_id), DEVICE_ID);
    PCI_PROBE_ASSERT(PCI_read_config_word(dev, VENDOR_ID, &dev->vendor_id), VENDOR_ID);

    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, CLASS_CODE, &dev->class_code), CLASS_CODE);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, SUB_CLASS_CODE, &dev->subclass_code), SUB_CLASS_CODE);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, PROG_IF, &dev->prog_if), PROG_IF);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, REVISION_ID, &dev->revision_id), REVISION_ID);

    PCI_PROBE_ASSERT(PCI_read_config_word(dev, SUBSYSTEM_DEVICE_ID, &dev->subsystem_device_id), SUBSYSTEM_DEVICE_ID);
    PCI_PROBE_ASSERT(PCI_read_config_word(dev, SUBSYSTEM_VENDOR_ID, &dev->subsystem_vendor_id), SUBSYSTEM_VENDOR_ID);

    uint8_t header_reg;
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, HEADER, &header_reg), HEADER);
    dev->header_type = (header_reg & 0x0f);
    dev->multi_function = (header_reg & 0x80) >> 7;

    uint8_t bist;
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, BIST, &bist), BIST);
    dev->bist_capable = (bist & 0x80) >> 7;

    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, LATENCY_TIMER, &dev->latency_timer), LATENCY_TIMER);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, CACHE_LINE_SIZE, &dev->cache_line_size), CACHE_LINE_SIZE);

    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, INTERRUPT_PIN, &dev->interrupt_pin), INTERRUPT_PIN);
    PCI_PROBE_ASSERT(PCI_read_config_byte(dev, INTERRUPT_LINE, &dev->interrupt_line), INTERRUPT_LINE);

    //Is this a  PCI-to-PCI bridge?
    if (dev->class_code == 0x06 && dev->subclass_code == 0x04)
    {
        struct pci_bus *new_bus = PCI_probe_bus(dev);
        if (new_bus->number != dev->bus->number)
            PCI_enumerate_bus(new_bus);
        else
            free(new_bus);
    }

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

        for (int function = 1; function < 8; function++)
        {
            //Not all functions are required to be implemented
            if (PCI_check_vendor(bus, slot, function))
                continue;

            //populate new device info
            dev = PCI_probe_device(bus, slot, function);
        }
    }
}

void PCI_configure()
{
    printf("Enumerating PCI Bus\n");
    root_bus = malloc(sizeof(struct pci_bus));

    root_bus->number = 0;
    root_bus->parent = NULL;

    INIT_LIST_HEAD(&root_bus->node);
    INIT_LIST_HEAD(&root_bus->children);
    INIT_LIST_HEAD(&root_bus->devices);

    PCI_enumerate_bus(root_bus);

    // struct list_head *start_node = &root_bus->devices;
    // struct list_head *dev_node = start_node->prev;

    // while (dev_node != start_node)
    // {

    //     struct pci_dev *dev = list_entry(dev_node, struct pci_dev, node);

    //     PCI_describe_device(dev);
    //     PCI_describe_bar(dev);

    //     dev_node = dev_node->prev;
    // }
}

//x86 specific
int PCI_read(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset, size_t size, uint32_t *val)
{
    uint32_t l_bus = ((uint32_t)bus) << 16;
    uint32_t l_slot = ((uint32_t)slot) << 11;
    uint32_t l_func = ((uint32_t)func) << 8;
    uint32_t address = (uint32_t)(l_bus | l_slot | l_func | ((uint32_t)(offset & 0xfc)) | ((uint32_t)0x80000000));

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
    uint32_t bitmask = 0xffffffff;
    uint8_t shift_offset = 0;
    uint32_t l_bus = ((uint32_t)bus) << 16;
    uint32_t l_slot = ((uint32_t)slot) << 11;
    uint32_t l_func = ((uint32_t)func) << 8;

    uint32_t address = (uint32_t)(l_bus | l_slot | l_func | ((uint32_t)(offset & 0xfc)) | ((uint32_t)0x80000000));

    if (size == sizeof(char))
    {
        switch (offset % 4)
        {
        case 0: //0x00
            bitmask = 0x00ffffff;
            shift_offset = 24;
            break;
        case 1: //0x01
            bitmask = 0xff00ffff;
            shift_offset = 16;
            break;
        case 2: //0x02
            bitmask = 0xffff00ff;
            shift_offset = 8;
            break;
        case 3: //0x03
            bitmask = 0xffffff00;
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
            bitmask = 0x0000ffff;
            shift_offset = 16;
            break;
        case 2: //0x02
            bitmask = 0xffff0000;
            shift_offset = 0;
            break;
        default:
            return 2;
            break;
        }
    }
    else if (size == sizeof(int))
    {
        bitmask = 0x0000000;
        shift_offset = 0;
    }
    else
    {
        return 1;
    }

    uint32_t *old_val = NULL;
    PCI_PROBE_ASSERT(PCI_read(bus, slot, func, offset, sizeof(int), old_val), offset);

    *old_val &= bitmask;
    *old_val |= (val << shift_offset);

    outl(CONFIG_ADDRESS_PORT, address);
    outl(CONFIG_DATA_PORT, *old_val);

    return 0;
}