###################################################################################
#-----------------------------Kernel Makefile options-----------------------------#
###################################################################################

#Target
KERNEL=$(BIN)/$(OS_NAME).elf

# Objects
KERNEL_OBJS=\
	kernel.o

# timer.o\
# debug.o\
# interrupts/idt.o\
# interrupts/isr.o\
# interrupts/irq.o\
# interrupts/pic.o\
# devices/keyboard.o\
# devices/mouse.o\
# devices/tty.o\
# devices/vesa.o\
# devices/vga.o\
# memory/init_heap.o\
# memory/k_malloc.o\
# memory/k_calloc.o\
# memory/k_realloc.o\
# memory/k_free.o\
# memory/memory.o\
# time/cmos.o\
# time/rtc.o\
# time/pit.o\
# pci/core.o\
# filesystems/fs.o\
# stack_smash.o\
# pacman/pacman.o\
# pacman/ai.o\
# pacman/render.o\
# pacman/input.o\

# filesystems/initrd.o\



# Ensures if we make a change to the library it gets relinked to the kernel 
# without making a random change to the kernel. Only needed for statically linked 
# libraries.
KERNEL_LIB_PATHS=
#  ../libraries/libc/libk.a\
#  ../libraries/libm/libm.a\
#  ../libraries/libgraphics/libgraphics.a

KERNEL_CFLAGS:=\
	-MD -Wall -Wextra -m64\
	-ffreestanding\
	--sysroot=$(SYSROOT)\
	-isystem=$(SYSROOT)$(USR_INCLUDE_DIR)\
	-fno-stack-protector \
	-fno-pic -fpie       \
	-mgeneral-regs-only  \
	-mno-red-zone\
	-D__is_kernel\
	-Iinclude

KERNEL_LDFLAGS:=\
	-Wl,-static,-pie,--no-dynamic-linker,-ztext\
	-static-pie\
	-z max-page-size=0x1000

KERNEL_LIBS:=\
	$(LIBS)\
	-nostdlib\
	-lgcc
# -lk\
# -lgraphics\
# -lm

# Do not edit below
###################################################################################

KERNEL_OBJS:=$(addprefix $(PROJECT_OBJ)/, $(KERNEL_OBJS))
KERNEL_DEPS:=$(KERNEL_OBJS:.o=.d)

# Links MadOS.kernel
$(KERNEL): $(KERNEL_OBJS) $(KERNEL_LIB_PATHS)
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_LDFLAGS) -T"linker.ld" $(KERNEL_OBJS) $(KERNEL_LIBS) -o $(KERNEL)

$(PROJECT_OBJ)/%.o: $(PROJECT_PATH)/%.c
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_LIBS) -c $< -o $@ 

-include $(KERNEL_DEPS)

.PHONY: install install-bin install-headers clean
install: install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' . $(SYSROOT)$(USR_INCLUDE_DIR)/kernel

install-bin: $(KERNEL)
	cp -RL $(KERNEL) $(SYSROOT)$(KERNEL_DIR)

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.s" -delete -print
	@find . -type f -name "*.kernel" -delete -print
	@find . -type f -name "*.elf" -delete -print