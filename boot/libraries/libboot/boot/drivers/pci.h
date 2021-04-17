#ifndef __BOOT_DRIVERS_PCI_H
#define __BOOT_DRIVERS_PCI_H 1

#include "../stdint.h"
#include "../io.h"

#define PCI_CONFIG_PORT 0xcf8
#define PCI_DATA_PORT 0xcfc

typedef struct PCI_DRIVER
{
    //TODO
} PCI_DRIVER;

enum PCI_STANDARD_REGISTERS
{
    VENDOR_ID = 0x00,
    DEVICE_ID = 0x02,

    COMMAND = 0x04,
    STATUS = 0x06,

    REVISION_ID = 0x08,
    PROG_IF = 0x09,
    SUB_CLASS = 0x0a,
    CLASS_CODE = 0x0b,

    CACHE_LINE_SIZE = 0x0c,
    LATENCY_TIMER = 0x0d,
    HEADER_TYPE = 0x0e,
    BIST = 0x0f,

    BAR0 = 0x10,
    BAR1 = 0x14,
    BAR2 = 0x18,
    BAR3 = 0x1c,
    BAR4 = 0x20,
    BAR5 = 0x24,

    CARDBUS_CIS_POINTER = 0x28,

    SUBSYSTEM_VENDOR_ID = 0x2c,
    SUBSYSTEM_DEVICE_ID = 0x2e,

    EXPANSION_ROM_BASE_ADDRESS = 0x30,

    CAPABILITIES = 0x34,

    INTERRUPT_LINE = 0x3c,
    INTERRUPT_PIN = 0x3d,
    MIN_GRANT = 0x3e,
    MAX_LATENCY = 0x3f,
};

struct PCI_standard_header
{
    //reg = register
    //os = offset

    //reg 0x00
    uint16_t device_id;
    uint16_t vendor_id;

    //reg 0x01
    uint16_t status;  //os 0x04
    uint16_t command; //os 0x06

    //reg 0x02
    uint8_t class_code;  //os 0x08
    uint8_t sub_class;   //os 0x09
    uint8_t prog_if;     //os 0x0a
    uint8_t revision_id; //os 0x0b

    //reg 0x03
    uint8_t bist;            //os 0x0c
    uint8_t header_type;     //os 0x0d
    uint8_t latency_timer;   //os 0x0e
    uint8_t cache_line_size; //os 0x0f

    //reg 0x04
    uint32_t bar0; //os 0x10

    //reg 0x05
    uint32_t bar1; //os 0x14

    //reg 0x06
    uint32_t bar2; //os 0x18

    //reg 0x07
    uint32_t bar3; //os 0x1c

    //reg 0x08
    uint32_t bar4; //os 0x20

    //reg 0x09
    uint32_t bar5; //os 0x24

    //reg 0x0a
    uint32_t cardbus_cis_pointer; //os 0x28

    //reg 0x0b
    uint16_t subsystem_device_id; //os 0x2c
    uint16_t subsystem_vendor_id; //os 0x2e

    //reg 0x0c
    uint32_t expansion_rom_base_address; //os 0x30

    //reg 0x0d
    uint16_t reserved_1a;         //os 0x34
    uint8_t reserved_1b;          //os 0x36
    uint8_t capabilities_pointer; //os 0x37

    //reg 0x0e
    uint32_t reserved_2; //os 0x38

    //reg 0x0f
    uint8_t max_latency;    //os 0x3c
    uint8_t min_grant;      //os 0x3d
    uint8_t interrupt_pin;  //os 0x3e
    uint8_t interrupt_line; //os 0x3f
} __attribute__((__packed__));

struct PCI_to_PCI_bridge_header
{
    //reg = register
    //os = offset

    //reg 0x00
    uint16_t device_id; //os 0x00
    uint16_t vendor_id; //os 0x02

    //reg 0x01
    uint16_t status;  //os 0x04
    uint16_t command; //os 0x06

    //reg 0x02
    uint8_t class_code;  //os 0x08
    uint8_t sub_class;   //os 0x09
    uint8_t prog_if;     //os 0x0a
    uint8_t revision_id; //os 0x0b

    //reg 0x03
    uint8_t bist;            //os 0x0c
    uint8_t header_type;     //os 0x0d
    uint8_t latency_timer;   //os 0x0e
    uint8_t cache_line_size; //os 0x0f

    //reg 0x04
    uint32_t bar0; //os 0x10

    //reg 0x05
    uint32_t bar1; //os 0x14

    //reg 0x06
    uint8_t secondary_latency_timer; //os 0x18
    uint8_t subordinate_bus_number;  //os 0x19
    uint8_t secondary_bus_number;    //os 0x1a
    uint8_t primary_bus_number;      //os 0x1b

    //reg 0x07
    uint16_t secondary_status; //os 0x1c
    uint8_t io_limit;          //os 0x1e
    uint8_t io_base;           //os 0x1f

    //reg 0x08
    uint16_t memory_limit; //os 0x20
    uint16_t memory_base;  //os 0x22

    //reg 0x09
    uint16_t prefetchable_memory_limit; //os 0x24
    uint16_t prefetchable_memory_base;  //os 0x26

    //reg 0x0a
    uint32_t prefetchable_base_upper_32_bits; //os 0x28

    //reg 0x0b
    uint32_t prefetchable_limit_upper_32_bits; //os 0x2c

    //reg 0x0c
    uint16_t io_limit_upper_16_bits; //os 0x30
    uint16_t io_base_upper_16_bits;  //os 0x32

    //reg 0x0d
    uint16_t reserved_1a;       //0x34
    uint8_t reserved_1b;        //0x36
    uint8_t capability_pointer; //0x37

    //reg 0x0e
    uint32_t expansion_rom_address; //os 0x38

    //reg 0x0f
    uint16_t bridge_control; //os 0x3c
    uint8_t interrupt_pin;
    uint8_t interrupt_line; //os 0x3f
} __attribute__((__packed__));

struct PCI_to_cardbus_header
{
    //reg = register
    //os = offset

    //reg 0x00
    uint16_t device_id; //os 0x00
    uint16_t vendor_id; //os 0x02

    //reg 0x01
    uint16_t status;  //os 0x04
    uint16_t command; //os 0x06

    //reg 0x02
    uint8_t class_code;  //os 0x08
    uint8_t sub_class;   //os 0x09
    uint8_t prog_if;     //os 0x0a
    uint8_t revision_id; //os 0x0b

    //reg 0x03
    uint8_t bist;            //os 0x0c
    uint8_t header_type;     //os 0x0d
    uint8_t latency_timer;   //os 0x0e
    uint8_t cache_line_size; //os 0x0f

    //reg 0x04
    uint32_t cardbus_socket_address; //os 0x10

    //reg 0x05
    uint16_t secondary_status;           //os 0x14
    uint8_t reserved_1;                  //os 0x16
    uint8_t offset_of_capabilities_list; //os 0x17

    //reg 0x06
    uint8_t cardbus_latency_timer;  //of 0x18
    uint8_t subordinate_bus_number; //0x19
    uint8_t cardbus_bus_number;     //0x1a
    uint8_t pci_bus_number;         //0x1b

    //reg 0x07
    uint32_t memory_base_address_0; //0x1c

    //reg 0x08
    uint32_t memory_limit_0; //0x20

    //reg 0x09
    uint32_t memory_base_address_1; //0x24

    //reg 0x0a
    uint32_t memory_limit_1; //0x28

    //reg 0x0b
    uint32_t io_base_address_0; //0x2c

    //reg 0x0c
    uint32_t io_limit_0; // 0x30

    //reg 0x0d
    uint32_t io_base_address_1; //0x34

    //reg 0x0e
    uint32_t io_limit_1; //0x38

    //reg 0x0f
    uint16_t bridge_control; //os 0x3c
    uint8_t interrupt_pin;   //os 0x3e
    uint8_t interrupt_line;  //os 0x3f

    //reg 0x10
    uint16_t subsystem_vendor_id; //0x40
    uint16_t subsytem_device_id;  //0x42

    //reg 0x11
    uint32_t legacy_16_bit_card_base_address; // 0x44
} __attribute__((__packed__));

void PCI_configuration(PCI_DRIVER *driver_list, uint32_t size);

static inline int PCI_is_multi_function(uint8_t register_header_type)
{
    return 0x80 & register_header_type;
}

static inline uint32_t PCI_read_config_dword(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset)
{
    uint32_t address = (uint32_t)(
        (((uint32_t)bus) << 16) | (((uint32_t)device) << 11) | (((uint32_t)func) << 8) | ((uint32_t)(offset & 0xfc)) | ((uint32_t)0x80000000));

    outl(PCI_CONFIG_PORT, address);
    return (uint32_t)inl(PCI_DATA_PORT);
}

static inline uint16_t PCI_read_config_word(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset)
{
    uint32_t reg = PCI_read_config_dword(bus, device, func, offset);

    if ((offset % 4) == 2)
        return (uint16_t)((reg & 0xffff0000) >> 16);

    return (uint16_t)(reg & 0x0000ffff);
}

static inline uint8_t PCI_read_config_byte(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset)
{
    uint32_t reg = PCI_read_config_dword(bus, device, func, offset);

    switch (offset % 4)
    {
    case 1:
        return (uint8_t)((reg & 0x0000ff00) >> 8);
    case 2:
        return (uint8_t)((reg & 0x00ff0000) >> 16);
    case 3:
        return (uint8_t)((reg & 0xff000000) >> 24);
    }

    return (uint8_t)(reg & 0x000000ff);
}

// static inline int PCI_write_config_byte() {}
// static inline int PCI_write_config_word() {}
static inline void PCI_write_config_dword(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint32_t value)
{
    uint32_t address = (uint32_t)(
        (((uint32_t)bus) << 16) | (((uint32_t)device) << 11) | (((uint32_t)func) << 8) | ((uint32_t)(offset & 0xfc)) | ((uint32_t)0x80000000));

    outl(PCI_CONFIG_PORT, address);
    outl(PCI_DATA_PORT, value);
}

#endif