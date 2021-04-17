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
static void PCI_check_bar(uint32_t bus, uint32_t device, uint32_t function);
static uint64_t PCI_check_bar_size(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset);

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
    printf("\tF: %d / %04x:%04x / %s / %02x / %02x\n", function, vendor_id, device_id, PCI_class_codes[class_code], sub_class, prog_if);

    // if (class_code == 0x01 && sub_class == 0x01 && prog_if == 0x80)
    PCI_check_bar(bus, device, function);
}

#define BAR_INDEX(bar_index) (((bar_index - BAR0) / 4))
#define BAR_ALWAYS_0(bar) ((bar & 0x00000001) == 0)
#define BAR_ALWAYS_1(bar) ((bar & 0x00000001) == 1)
#define BAR_MEMORY_TYPE(bar) ((bar & 0x00000006) >> 1)
#define BAR_IS_PREFETCHABLE(bar) ((bar & 0x00000008) >> 3)
#define BAR_64_BIT_ADDRESS(bar1, bar2) ((bar1 & 0xfffffff0) + ((bar2 & 0xffffffff) << 32))
#define BAR_16_BYTE_ALIGNED_ADDRESS(bar) (bar & 0xfffffff0)
#define BAR_4_BYTE_ALIGNED_ADDRESS(bar) (bar & 0xfffffffc)

static void PCI_check_bar(uint32_t bus, uint32_t device, uint32_t function)
{
    uint32_t bar_index = BAR0;

    while (bar_index != (BAR5 + 4))
    {
        uint32_t bar = PCI_read_config_dword(bus, device, function, bar_index);

        if (bar == 0)
        {
            bar_index += 4;
            continue;
        }

        if (BAR_ALWAYS_0(bar))
        { //Memory Space Bar
            if (BAR_MEMORY_TYPE(bar) == 0x00)
            { //32 bit Bar
                printf("\t\tBar%d - Memory - Address: 0x%08x / Size: 0x%08x / 0x%02x / 0x%02x\n",
                       BAR_INDEX(bar_index),
                       BAR_16_BYTE_ALIGNED_ADDRESS(bar),
                       PCI_check_bar_size(bus, device, function, bar_index),
                       BAR_IS_PREFETCHABLE(bar),
                       BAR_MEMORY_TYPE(bar));
                bar_index += 4;
            }
            else if (BAR_MEMORY_TYPE(bar) == 0x02)
            { // 64 bit Bar
                uint32_t bar2 = PCI_read_config_dword(bus, device, function, bar_index + 4);

                printf("\t\tBar%d - Memory - Address: 0x%08x / Size: 0x%08x / 0x%02x / 0x%02x\n",
                       BAR_INDEX(bar_index),
                       BAR_64_BIT_ADDRESS(bar, bar2),
                       PCI_check_bar_size(bus, device, function, bar_index),
                       BAR_IS_PREFETCHABLE(bar),
                       BAR_MEMORY_TYPE(bar));
                bar_index += 8;
            }
        }
        else if (BAR_ALWAYS_1(bar))
        {
            printf("\t\tBar%d -   IO   - Ports: 0x%04x -> 0x%04x / Range: %d\n",
                   BAR_INDEX(bar_index),
                   BAR_4_BYTE_ALIGNED_ADDRESS(bar),
                   BAR_4_BYTE_ALIGNED_ADDRESS(bar) + PCI_check_bar_size(bus, device, function, bar_index),
                   PCI_check_bar_size(bus, device, function, bar_index)+1);
            bar_index += 4;
        }
    }
}

static uint64_t PCI_check_bar_size(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset)
{
    uint64_t size = 0;
    const uint32_t original_bar = PCI_read_config_dword(bus, device, function, offset);

    if (BAR_ALWAYS_0(original_bar))
    { //Memory Space
        if (BAR_MEMORY_TYPE(original_bar) == 0x00)
        { //32 bit size
            PCI_write_config_dword(bus, device, function, offset, 0xffffffff);
            size = (~PCI_read_config_dword(bus, device, function, offset)) + 1;

            PCI_write_config_dword(bus, device, function, offset, original_bar);
        }
        else if (BAR_MEMORY_TYPE(original_bar) == 0x02)
        { //64 bit size
            const uint32_t original_bar2 = PCI_read_config_dword(bus, device, function, offset + 4);

            PCI_write_config_dword(bus, device, function, offset, 0xffffffff);
            PCI_write_config_dword(bus, device, function, offset + 4, 0xffffffff);

            size = (~(BAR_64_BIT_ADDRESS(PCI_read_config_dword(bus, device, function, offset), PCI_read_config_dword(bus, device, function, offset + 4)))) + 1;

            PCI_write_config_dword(bus, device, function, offset, original_bar);
            PCI_write_config_dword(bus, device, function, offset + 4, original_bar2);
        }
    }
    else if (BAR_ALWAYS_1(original_bar))
    { //IO Space
        PCI_write_config_dword(bus, device, function, offset, 0xffffffff);
        size = (~PCI_read_config_dword(bus, device, function, offset)) + 1;

        PCI_write_config_dword(bus, device, function, offset, original_bar);
    }

    return size;
}
