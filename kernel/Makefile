###################################################################################
#-----------------------------Kernel Makefile options-----------------------------#
###################################################################################

NASM?=nasm

KERNEL=MadOS.kernel

KERNEL_OBJS= \
	Early_Kernel/extended_boot.o\
	kernel.o \
	tty.o\
	io.o\
	debug.o\
	Interrupts/idt.o\
	Interrupts/isr.o\
	Interrupts/irq.o\
	Devices/keyboard.o\
	Heap/init_heap.o\
	Heap/k_malloc.o\
	Heap/k_calloc.o\
	Heap/k_realloc.o\
	Heap/k_free.o\
	Heap/memory.o\
	timer.o\

KERNEL_CFLAGS:= -Ttext 0x8000 -mno-red-zone $(CFLAGS)  
KERNEL_CPPFLAGS:=$(CPPFLAGS) -D__$(ARCH_NAME) -D__is_kernel -Iinclude  
KERNEL_LIBS:=$(LIBS) -nostdlib -lk -lgcc

# Do not edit below
###################################################################################

KERNEL_LINKER_LIST+=$(KERNEL_OBJS)

KERNEL_LINKER_LIST:=$(KERNEL_LINKER_LIST:.o=.k.o)
KERNEL_DEPS:=$(KERNEL_LINKER_LIST:.o=.d)

-include $(KERNEL_DEPS)
-include echo $(KERNEL_DEPS)

# Links MadOS.kernel
$(KERNEL): $(KERNEL_LINKER_LIST)
	$(CC) $(KERNEL_CFLAGS)  $(KERNEL_CPPFLAGS) -T"linker.ld" $(KERNEL_LINKER_LIST) $(KERNEL_LIBS) -o $(KERNEL)

# Build a c file into a object file
%.k.o: %.c
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS) $(KERNEL_LIBS) -c $< -o $@ 

%.k.o: %.asm	
	$(NASM) -f elf64 $^ -o $@

.PHONY: install install-bin install-headers clean
install: install-headers install-bin

install-headers:
	cp -RL include/. $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(KERNEL)
	cp -RL $(KERNEL) $(SYSROOT)$(USR_INCLUDE_DIR)

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.kernel" -delete -print