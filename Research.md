# OS DEV Research

Notes, examples, and links of any useful resources I find interesting or useful. Most notes /examples will be transformed from original source into how I think I understand it with links provided. 


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [OS DEV Research](#os-dev-research)
  - [GDT](#gdt)
  - [IDT](#idt)
  - [PIT](#pit)
      - [Channel 0](#channel-0)
  - [PIC](#pic)

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

TODO

Sources:
http://www.osdever.net/bkerndev/Docs/idt.htm


PIT
---
__Programmable Interval Timer__

The PIT or also called the System Clock, is a very useful chip for accurately generating interrupts at regular intervals. 

The chip has 3 channels(indexed 0 - 2):
    - 0: Tied to IRQ0 to interrupt the CPU at predictable and regular times
    - 1: System specific(depends).
    - 2: Connected to the system speaker.(Beep Boop speaker)

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


PIC
---
__Programmable Interrupt Controller__

TODO

Sources: