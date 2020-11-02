#ifndef __KERNEL_PCI_PCI_H
#define __KERNEL_PCI_PCI_H 1

#include <stdint.h>

//https://wiki.osdev.org/PCI

//Interesting thing here
//http://www.catb.org/esr/structure-packing/

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

#define PCI_standard_header_num 0x00
#define PCI_to_PCI_bridge_header_num 0x01
#define PCI_cardbus_bridge_header_num 0x02

//header_type register bitmask
#define PCI_header_type_mf 0x80;
#define PCI_header_type_num 0x7f

//bist register bitmask
#define PCI_bist_capable 0x80
#define PCI_bist_start 0x40
#define PCI_bist_completion_code 0x0f

//command register bitmask
#define PCI_command_interrupt_dissable 0x0400 //TODO beware of correctness
#define PCI_command_fb_to_back_enable 0x0200
#define PCI_command_serr_enable 0x0100
#define PCI_command_parity_error_response 0x0040
#define PCI_command_vga_palette_snoop 0x0020
#define PCI_command_memory_winval_enable 0x0010
#define PCI_command_special_cycles 0x0008
#define PCI_command_bus_master 0x0004
#define PCI_command_memory_space 0x0002
#define PCI_command_io_space 0x0001

//status register bitmask
#define PCI_status_detected_parity_bit 0x8000 //TODO beware of correctness
#define PCI_status_signaled_system_error 0x4000
#define PCI_status_received_master_abort 0x2000
#define PCI_status_received_target_abort 0x1000
#define PCI_status_signaled_target_abort 0x0800
#define PCI_status_devsel_timing 0x0600
#define PCI_status_master_data_parity_error 0x0100
#define PCI_status_fb_to_back_capable 0x0080
#define PCI_status_66mhz_capable 0x0020
#define PCI_status_capabilities_list 0x0010
#define PCI_status_interrupt_status 0x0008

//bar registers bitmask
#define PCI_bar_16_byte_aligned_base_address 0xfffffff0 //TODO beware of correctness
#define PCI_bar_prefetchable 0x00000008
#define PCI_bar_type 0x00000006
#define PCI_bar_always_0 0x00000001

//I/O space Bar Layout
#define PCI_io_4_byte_aligned_base_address 0xfffffffc //TODO beware of correctness
#define PCI_io_always_1 0x00000001

//forward declare structs because they are long as f*ck
struct PCI_Header_description;
struct PCI_standard_header;
struct PCI_to_PCI_bridge_header;
struct PCI_to_cardbus_header;

void PCI_install();

uint32_t PCI_config_read_dw(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void PCI_config_write_dw(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data);
void PCI_check_device(uint8_t bus, uint8_t device);
uint16_t PCI_check_vendor(uint8_t bus, uint8_t slot);
struct PCI_Header_description
{
    uint64_t address;
    uint8_t type;
    uint8_t bus;
    uint8_t device;
    uint8_t class_code;
    // uint8_t sub_code;
    // uint8_t prog_if;
};

struct PCI_standard_header
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
    uint16_t subsystem_id;        //os 0x2c
    uint16_t subsystem_vendor_id; //os 0x2f

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
    uint8_t interrupt_pin;   // os 0x3e
    uint8_t interrupt_line;  //os 0x3f
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

//todo probably dont make this static and define in .cpp file as it will make code size grow verrrryyy fast
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

#endif
