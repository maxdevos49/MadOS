###################################################################################
#-----------------------------Kernel Makefile options-----------------------------#
###################################################################################

KERNEL=MadOS.kernel

KERNEL_OBJS= \
	kernel.o \
	kernel/heap/init_heap.o\
	kernel/heap/k_malloc.o\
	kernel/heap/k_calloc.o\
	kernel/heap/k_realloc.o\
	kernel/heap/k_free.o\

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
	$(CC) $(KERNEL_CFLAGS)  $(KERNEL_CPPFLAGS) -T"kernel/arch/$(ARCH_DIR)/linker.ld" $(KERNEL_LINKER_LIST) $(KERNEL_LIBS) -o kernel/$(KERNEL)

# Build a c file into a object file
%.k.o: %.c
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS) $(KERNEL_LIBS) -c $< -o $@ 

.PHONY: install install-bin install-headers clean
install: install-headers install-bin

install-headers:
	cp -RL include/. $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(KERNEL)
	cp -RL $(KERNEL) $(SYSROOT)$(USR_INCLUDE_DIR)

clean:
	@echo TODO clean Kernel