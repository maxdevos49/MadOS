###################################################################################
#-----------------------------Kernel Makefile options-----------------------------#
###################################################################################

#Defaults
NASM?=nasm
CC?=x86_64-elf-gcc
OBJCOPY?=x86_64-elf-objcopy
CFLAGS?=-O1 -MD -Wall -Wextra -ffreestanding -m64 --sysroot=../sysroot -isystem=../sysroot/usr/include
CPPFLAGS?=
LIBS?=

#Target
KERNEL=MadOS.kernel


# Objects
KERNEL_OBJS=\
	boot/extended_boot.o\
	boot/binaries.o\
	kernel.o \
	tty.o\
	timer.o\
	debug.o\
	interrupts/idt.o\
	interrupts/isr.o\
	interrupts/irq.o\
	interrupts/pic.o\
	devices/keyboard.o\
	devices/mouse.o\
	devices/vesa.o\
	memory/init_heap.o\
	memory/k_malloc.o\
	memory/k_calloc.o\
	memory/k_realloc.o\
	memory/k_free.o\
	memory/memory.o\
	time/cmos.o\
	time/rtc.o\
	time/pit.o\
	pci/core.o\
	filesystems/fs.o\
	stack_smash.o\
	pacman/pacman.o\
	pacman/ai.o\
	pacman/render.o\
	pacman/input.o\
	
	# filesystems/initrd.o\



# Ensures if we make a change to the library it gets relinked to the kernel 
# without making a random change to the kernel. Only needed for statically linked 
# libraries.
KERNEL_LIB_PATHS=\
	 ../libraries/libc/libk.a\
	 ../libraries/libgraphics/libgraphics.a

KERNEL_CFLAGS:=-Ttext 0x8000 $(CFLAGS)  
KERNEL_CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude  
KERNEL_LIBS:=$(LIBS) -nostdlib -lk -lgcc -ltest -lgraphics

# Do not edit below
###################################################################################

KERNEL_LINKER_LIST+=$(KERNEL_OBJS)
KERNEL_DEPS:=$(KERNEL_LINKER_LIST:.o=.d)

TMP:=$(filter-out 	boot/extended_boot.o boot/binaries.o, $(KERNEL_OBJS))
KERNEL_DEBUG:=$(TMP:.o=.s)

# Links MadOS.kernel
$(KERNEL): $(KERNEL_LINKER_LIST) $(KERNEL_LIB_PATHS)
	$(CC) $(KERNEL_CFLAGS)  $(KERNEL_CPPFLAGS) -T"linker.ld" $(KERNEL_LINKER_LIST) $(KERNEL_LIBS) -o $(KERNEL).elf
	$(OBJCOPY) -O binary $(KERNEL).elf $(KERNEL)


%.o: %.c
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS) $(KERNEL_LIBS) -c $< -o $@ 

%.o: %.asm
	$(NASM) -f elf64 $^ -o $@ -MD $@.d

%.s: %.c
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS) $(KERNEL_LIBS) -S -masm=intel -fverbose-asm $< -o $@

-include $(KERNEL_DEPS)

.PHONY: install install-bin install-headers clean
install: install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' . $(SYSROOT)$(USR_INCLUDE_DIR)/kernel

install-bin: $(KERNEL)
	cp -RL $(KERNEL) $(SYSROOT)$(KERNEL_DIR)

debug: $(KERNEL_DEBUG)

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.s" -delete -print
	@find . -type f -name "*.kernel" -delete -print
	@find . -type f -name "*.elf" -delete -print