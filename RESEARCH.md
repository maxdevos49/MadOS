# OS DEV Research

Notes, examples, and links of any useful resources I find interesting or useful. Most notes /examples will be me copying what I am reading to help remember and comprehend.


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [OS DEV Research](#os-dev-research)
  - [GDT](#gdt)
  - [IDT](#idt)
  - [ISR](#isr)
      - [Exceptions](#exceptions)
  - [IRQ](#irq)
  - [8259 PIC](#8259-pic)
    - [How the 8259 PIC works](#how-the-8259-pic-works)
    - [Programming the 8259 PIC](#programming-the-8259-pic)
  - [CMOS](#cmos)
    - [Accessing CMOS Registers](#accessing-cmos-registers)
    - [RTC](#rtc)
    - [Getting Current Data And time from RTC](#getting-current-data-and-time-from-rtc)
  - [PIT](#pit)
      - [Channel 0](#channel-0)
  - [C and ASM Interop](#c-and-asm-interop)
    - [Passing Arguments to C Functions from ASM](#passing-arguments-to-c-functions-from-asm)
  - [C and Hardware Interop](#c-and-hardware-interop)
    - [Memory Mapped Devices](#memory-mapped-devices)
  - [# PCI](#-pci)
    - [PCI Configuration Space](#pci-configuration-space)
    - [Configuration Space Access Mechanism #1](#configuration-space-access-mechanism-1)
      - [CONFIG_ADDRESS bit table](#config_address-bit-table)
      - [Registers](#registers)
        - [vendorID](#vendorid)
        - [deviceID](#deviceid)
        - [class](#class)
  - [# MMU](#-mmu)
    - [Translation](#translation)
    - [Protection](#protection)
- [Paging](#paging)
    - [Page Directory](#page-directory)
- [VESA](#vesa)

<!-- /code_chunk_output -->

GDT
---
__Global Descriptor Table__

The GDT plays a vital role in the memory protection of an x86 machine. The GDT defines base access privileges for certain parts of memory. We can use an entry in the GDT to generate segment violation exceptions(segfaults) that give the kernel the opportunity to end a process doing something it shouldn't. Most modern operating systems use "Paging" to do this as it is a lot more versatile and allows for higher flexibility.

The GDT itself is a list of 64-bit long entries. These entries define where in memory that the allowed region will start, as well as the limit of this region, and the access privileges associated with this entry. The first entry in a GDT is a NULL descriptor. No segment register should be set to 0 or it will generate a General Protection fault as a protection feature of the processor.

Each GDT entry also defines whether or not the current segment that the processor is running in is for System use(Ring 0) or for Application use (Ring 3). As a basic rule, any application causes an exception when it tries to access system or Ring 0 data. This protection exists to prevent an application from causing the kernel to crash. As far as the GDT is concerned, the ring levels here tell the processor if its allowed to execute privileged instructions. For example the ability to run the instructions `cli` or `sti` would effectively stop the kernel from running.

Sources:
http://www.osdever.net/bkerndev/Docs/gdt.htm


IDT
---
__Interrupt Descriptor Table__

The IDT is used in order to show the processor what Interrupt Service Routine (ISR) to call to handle either an exception or an `int` opcode (in assembly). IDT entries are also called by Interrupt Request whenever a device has completed a request and needs to be serviced.

Each IDT entry is similar to that of a GDT entry. Both have a base address, hold an access flag, and both are 64-bits long. The major difference between the GDT and IDT is in the meaning of the fields. In an IDT, the base address specified in the descriptor is actually the address of the Interrupt Service Routine that the processor should call when the interrupt is 'raised'(called). An IDT does not have a limit, instead it has a segment that you need to specify/ The segment must be the same segment that the given ISR is located in. This allows the processor to give control to the kernel through an interrupt that has occurred when the processor is in a different ring(like when an application is running).

The access flags of an IDT entry are also similar to a GDT entry's There is a field to say if the descriptor is actually present or not. There is a field for the Descriptor Privilege Level (DPL) to say which ring the highest number that is allowed to use the given interrupt. The Major difference is the rest of teh access flag definition. The lower 5-bits of the access byte is always set to 01110 in binary. This is 14 in decimal.

Access Flag: 
```
P (1 bit)        --> 0-1        - Present
DPL (2 bits)     --> 0-3        - Ring level
Always (5 bits)  --> 01110(14)  - Required same bits

Ex:
11101110b -> Present, Ring 3, Always bits
10001110b -> Present, Ring 0, Always bits
```

Adding new ISR entries can be done at anytime after the IDT has been initially registered.

Sources:
http://www.osdever.net/bkerndev/Docs/idt.htm

ISR
---
__Interrupt Service Routine__

ISRs are used to save the current processor state and set up the appropriate segment registers needed for kernel mode before the kernels C-level interrupt handler is called.

#### Exceptions
An Exception is a special case the processor encounters when it cannot continue normal execution. This could be something like dividing by zero: The result is an unknown non-real number, so the processor will cause an exception so the kernel can stop the process or task from causing any problems. If the IDT does not have a ISR defined for the given exception the processor will reset. This often results in a boot loop.

The first 32 entries in the IDT correspond to Exceptions that can possibly be generated by the processor, and therefore need to be handled. Some exceptions will push another value to the stack: an Error Code value which is specific to the exception caused.

| Exception # | Description | Error Code? |
|-------------|-------------|------------|
| 0 | Division By Zero Exception | No |
| 1 | Debug Exception | No |
| 2 | Non Maskable Interrupt Exception | No |
| 3 | Breakpoint Exception | No |
| 4 | Into Detected Overflow Exception | No |
| 5 | Out of Bounds Exception | No |
| 6 | Invalid Opcode Exception | No |
| 7 | No Coprocessor Exception | No |
| 8 | Double Fault Exception | Yes |
| 9 | Coprocessor Segment Overrun Exception | No |
| 10 | Bad TSS Exception | Yes |
| 11 | Segment Not Present Exception | Yes |
| 12 | Stack Fault Exception | Yes |
| 13 | General Protection Fault Exception | Yes |
| 14 | Page Fault Exception | Yes|
| 15 | Unknown Interrupt Exception | No |
| 16 | Coprocessor Fault Exception | No |
| 17 | Alignment Check Exception (486+) | No |
| 18 | Machine Check Exception (Pentium/586+) | No |
| 19-31 | Reserved Exceptions | No |

Sources: 
http://www.osdever.net/bkerndev/Docs/isrs.htm

IRQ
---
__Interrupt Request__

| IRQ | Invoker | Default Enabled |
|-----|---------|-----------------|
| 0 | PIT | Yes |
| 1 | Keyboard | Yes |
| 8 | RTC | No |

TODO

Sources:
http://www.osdever.net/bkerndev/Docs/irqs.htm

8259 PIC
---
__8259 Programmable Interrupt Controller__

The PIC is one of the most important chips making up the x86 architecture. Without it, the x86 architecture would not be an interrupt driven architecture. The function of the pic is to manage hardware interrupts and send them to the appropriate system interrupt. This allows the system to respond to devices needs without loss of time from things like polling.

It is important to note that the APIC has replaced the 8259 PIC in modern systems to allow for multiprocessing.

### How the 8259 PIC works
The PIC controls the CPU's interrupt mechanism, by accepting several interrupt request and feeding them to the processor in order. For instance, when the keyboard registers a keyhit, it sends a puls along its interrupt line (IRQ 1) to the PIC chip, which translates the IRQ into a system interrupt and sends a message to interrupt the CPU from whatever it is doing. 

In the beginning there was only 1 PIC chip but later on a second PIC chip was added to bring the amount of IRQs from 8 to 15. IRQ2 is reserved on the first PIC chip to cascade interrupts into the second PIC chip resulting in 15 IRQs instead of 16. All IRQs first flow into the Master PIC(IRQs 0-7) and if they need too are cascaded into the Slave PIC(IRQs 8-15)

### Programming the 8259 PIC
Each chip(master and slave) has a command port and a data port. 

| Chip | Purpose | I/O Port |
|------|---------|----------|
| Master | Command | 0x20 |
| Master | Data | 0x21 |
| Slave | Command | 0xa0 |
| Slave | Data | 0xa1 |

When no command is issued, the data port allows us to access the interrupt mask of the PIC.
```c
uint8_t master_bit_mask = inb(0x21);
uint8_t slave_bit_mask = inb(0xa1);
```

For example if the master PIC returned 0xfd it would mean that only IRQ1 is enabled. Looking at the binary representation of 0xfd helps to understand this: `0xfd == 11111101b`. Bits with a value of 1 represent a disabled IRQ


Sources:
https://wiki.osdev.org/8259_PIC
http://www.osdever.net/bkerndev/Docs/irqs.htm

CMOS
---

CMOS is a tiny bit if very low power static memory that lives on the same chip as the Real-Time Clock (RTC). The CMOS (and RTC) can only be accessed through IO ports 0x70 and 0x71. 

| Port | Use |
|------|-----|
| 0x70 | Command Port |
| 0x71 | Data Port |

The function of the CMOS memory is to store 50 (or 114) bytes of "Setup" information for the BIOS while the computer is turned off. A dedicated battery keeps this information present. 

CMOS values are accessed a byte at a time, and each byte is individually addressable. Each CMOS address is traditionally called a register. The first 14 CMOS registers access and control the Real-Time Clock. In fact the only truly useful registers remaining on the CMOS are the Real-Time Clock registers, and register 0x10. All other registers are obsolete or are not standardized and are therefore useless.

### Accessing CMOS Registers

To access a CMOS register it is extremely simple but you must take into account how you want to handle NMI. To select a register for reading or writing by sending the register number to port 0x70(CMOS_COMMAND). Since the 0x80 bit or Port 0x70 controls NMI, you always end up setting that, too. So your CMOS controller always needs to know whether your OS wants NMI enabled or not. Selecting a register can be done as the following:

```c
uint8_t NMI_disable_bit = 0x80;
uint8_t register_number = 0x10;
outb(CMOS_COMMAND, (NMI_disable_bit<<7>>) | register_number);
```

Once the register is selected, you either read the value of that register on Port 0x71(CMOS_DATA) or you write to it.

```c
//reading value from register 10 on CMOS
uint8_t value = inb(CMOS_DATA);
//or
//Write new value(0x45) to register 10 on CMOS
outb(CMOS_DATA, 0x45);
```

Note: Reading or writing to Port 0x71(CMOS_DATA) seems to default the "selected register" back to 0xD. So you need to reselect the register every time you want to access a CMOS register.

Note 2: Writing to specific CMOS registers can invalidate a BIOS checksum resulting in the BIOS unable to boot. Do not write to any register that you are not aware of its use.

### RTC
__Real-Time Clock__

The RTC keeps track of the date and time, even when the computer is powered down. Before the RTC the computer would have to ask every boot for the current date or time.

The RTC can generate clock ticks on IRQ8 (similarly to PIT on IRQ0). The highest feasible clock frequency is 8KHz. Using the RTC clock this way may actually generate more stable clock pulses than the PIT can generate. It also frees up the PIT for timing events that need near microsecond accuracy. Additionally, the RTC can generate an IRQ8 at a particular time of day. 

### Getting Current Data And time from RTC

To get each of the following values from the RTC, you should first ensure that you wont be effected by an update. Then select the associated "CMOS register" in the usual way, and read the value from Port 0x71.

| Register | Contents | Range |
|----------|----------|-------|
| 0x00 | Seconds | 0-59 |
| 0x02 | Minutes | 0-59 |
| 0x04 | Hours | 0-23 or 1-12(highest bit pm) |
| 0x06 | Weekday | 1-7, Sunday = 1 |
| 0x07 | Day of Month | 1-31 |
| 0x08 | Month | 1-12 |
| 0x09 | Year | 0-99 |
| 0x32 | Century (maybe) | 19-20 |
| 0x0A | Status Register A | Na ? |
| 0x0B | Status Register B | Na ? |



Sources:
https://wiki.osdev.org/CMOS

PIT
---
__Programmable Interval Timer__

The PIT or also called the System Clock, is a very useful chip for accurately generating interrupts at regular intervals. 

The chip has 3 channels(indexed 0 - 2):
  - 0: Tied to IRQ0 to interrupt the CPU at predictable and regular times
  - 1: No longer present
  - 2: Connected to the system speaker.(Beep speaker)

Typically you are only concerned with Channels 0 and 2. Channel 2 can be used to make your computer beep. 

#### Channel 0
Channel 0 can be programmed for multiple things such as scheduling processes or making your current task to wait for a period of time. By default this channel is set to generate IRQ0 18.222 times per second. The BIOS sets up this default. 

To set the rate at which channel 0 of the timer fires an IRQ0, we must use our outb function to write to I/O ports. 

- Data/Divisor registers: 0x40,0x41,0x42
- Command register: 0x43

Each data register corresponds to each channel respectively. The timer will divide its input clock of 1.19MHz(1193180Hz) by the number in the given data register to figure out how many times per second to fire the signal for that channel. You must first select the channel that you want to update using the command register before writing to the data/divisor register.

TODO

Sources:
http://www.osdever.net/bkerndev/Docs/pit.htm

C and ASM Interop
---

### Passing Arguments to C Functions from ASM
```
arg1 - rdi
arg2 - rsi
arg3 - rdx
arg4 - rcx
arg5 - r8
arg6 - r9
Then anything after that gets pushed onto the stack in reverse
thats for x86_64
```
C and Hardware Interop
---

### Memory Mapped Devices
Devices that are mapped to memory are mapped to your ram instead if using ports or use both. For complicated MMD interfaces using a struct can be a useful way to avoid errors and also make your code more readable.

# PCI
---
__Peripheral Component Interconnect__

The PCI bus was defined to establish a high performance and low cost local bus that would remain through several generations of products.The PCI bus component and add-in card interface is processor independent, enabling an efficient transition to future processors, as well as use with multiple processor architectures. A single PCI bus can drive a maximum of 10 loads. (Remember when counting the number of loads on the bus, a connector counts as one load and the PCI device counts as another, and sometimes two.)

### PCI Configuration Space
The PCI specification provides for totally software driven initialization and configuration of each device (or target) on the PCI Bus via a separate Configuration Address Space. All PCI devices, except host bus bridges, are required to provide 256 bytes of configuration registers for this purpose.

Configuration read/write cycles are used to access the Configuration Space of each target device. A target is selected during a configuration access when its *IDSEL* signal is asserted. The *IDSEL* acts as the classic "chip select" signal. During the address phase of the configuration cycle, the processor can address one of 64 32-bit registers within the configuration space by placing the required register number on address lines 2 and 7(AD[7..2]) and the byte enable lines.

PCI devices are inherently little-endian, meaning all multiple byte fields have the least significant values at the lower addresses. This requires a big-endian processor, such as a Power PC, to perform the proper byte swapping of data read from or written to the PCI device, including any accesses to the Configuration Address Space.

Systems must provide a mechanism that allows access to the PCI configuration space, as most CPUs do not have any such mechanism. This task is usually performed by the Host to PCI Bridge (Host Bridge). Two distinct mechanisms are defined to allow the software to generate the required configuration accesses. Configuration mechanism #1 is the preferred method, while mechanism #2 is provided for backwards compatibility. Only configuration mechanism #1 will be described here, as it the only access mechanism that will be used on the future.

### Configuration Space Access Mechanism #1 
Two 32-bit I/O location are used the first location (0xCF8) is named CONFIG_ADDRESS, and the second (0xCFC) is called CONFIG_DATA. CONFIG_ADDRESS specifies the configuration address that is required to be accesses, while accesses to CONFIG_DATA will actually generate the configuration access and will transfer the data to or from the CONFIG_DATA register.

| Name | I/O Address |
|------|-------------|
| CONFIG_ADDRESS | 0xcf8 |
| CONFIG_DATA | 0xcfc |

The CONFIG_ADDRESS is a 32-bit register with the format shown in the following figure. Bit 31 is an enable flag for determining when accesses to CONFIG_DATA should be translated to the configuration cycles. Bits 23 through 16 allow the configuration software to choose a specific PCI bus in the system. Bits 15 through 11 select the specific device on the PCI Bus. Bits 10 though 8 choose a specific function in a device(if the device supports multiple functions).

The least significant byte selects the offset into the 256-byte configuration space available through this method. Since all reads and writes must be both 32-bits and aligned to work on all implementations, the two lowest buts of CONFIG_ADDRESS must be always zero, with the remaining siz bits allowing you to choose each of the 64 32-bits words. If you don't need all 32 bits, you'll have to perform the unaligned access in software by aligning the address, followed by masking and shifting the answer.

#### CONFIG_ADDRESS bit table
| bit # | Use |
|-------|-----|
| 31 | Enable Bit |
| 30-24 | Reserved |
| 23-16 | Bus Number |
| 15-11 | Device Number | 
| 10-8 | Function Number |
| 7-0 | Register Offset |

Example:
```c
//99% I screwed below up but I think its close
// Enable on
// Reserved bits
// BUS #7
// Device #1
// Function #2
// Register Offset 5
uint32_t config_selector = 0b1000000000011100011000000101;
uint32_t config_selector = 0x0801C605;
```

#### Registers
Description of some common registers

##### vendorID
16-bit register identifies a hardware manufacturer. For instance, every intel device is marked with the same vendor number, 0x8086. There is a global registry of such numbers, maintained by the PCI Special Interest Group, and manufacturers must apply to have a unique number assigned to them

##### deviceID
16-bit register, selected by the manufacturer; no official registration is required for the device ID. This ID is usually paired with the vendorID to make a unique 32-bit identifier for a hardware device. Typically drivers refer to the dw id to identify its device. You can find what value to look for in the hardware manual for a target device.

##### class
Every peripheral device belongs to a class. The class register is a 16-bit value where the top 8 bits identify the "base class"(or group). For example "ethernet" and "token ring" are two classes belong to the network group. 

# MMU
---
__Memory Management Unit__

The MMU, or Memory Management Unit, is a component of many computers that handles memory translation, memory protection, and other purposes specific to each architecture.

### Translation
The MMU's main service to the computer is memory translation. Memory translation is a process by which virtual addresses are converted to physical addresses. WE can say that the virtual addresses are mapped to the physical address. This gives us the ability to create a memory model in out own fashion. That is, we can rearrange how memory seems to be ordered.

For instance, this technique is used when creating a Higher Half Kernel.  The kernel is located at location x, but when paging is initialized the MMU is told to map location x to 0xC0000000. THis then creates the effect that the kernel is at 0xC0000000.

### Protection
Because we can make memory seem however we want, we can make each process appear that it is the only process on the machine. Moreover, because the process can only see the memory that it has, it cannot modify or copy any other application's memory. This means that if an application is to fail, it will fail, but nothing else.

# Paging
Paging is a system which allows each process to see a full virtual address space, without actually requiring the full amount of physical memory to be available or present. Paging introduces the benefit if  page-level protection. In this system, user processes can only see and modify data which is paged in on there own address space, providing hardware-based isolation. System pages are also protected from user processes. On the x86-64 architecture, page-level protection completely supersedes Segmentation as the memory protection mechanism. 

Paging is achieved through the use of the Memory Management Unit(MMU). On the x86, the MMU maps memory through a series of tables, two to be exact. They are the paging directory (PD), and the paging table (PT).

 Both tables contain 1024 4-byte entires, making them 4 KiB each. In the page directory, each entry points to a page table. In the page table, each entry points to a physical address that is then mapped to a virtual address found by calculating the offset within the directory and the offset within the table. This can be done as the entire table system represents a linear 4-GiB virtual memory map.  

### Page Directory
The topmost paging structure is the page directory. It is essentially an array of page directory entries. 

# VESA
__Video Electronics Standard Association__

