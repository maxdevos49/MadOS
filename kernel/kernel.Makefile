###################################################################################
#-----------------------------Kernel Makefile options-----------------------------#
###################################################################################

# Do not edit below
###################################################################################
KERNEL_LINKER_LIST=
include kernel/arch/$(ARCH_DIR)/$(ARCH_NAME).Makefile
###################################################################################
# Do not edit above


KERNEL_OBJS= \
	kernel/kernel.o 

KERNEL_CFLAGS:= $(CFLAGS) -ffreestanding  -Ttext 0x8000
KERNEL_CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
KERNEL_LIBS:=$(LIBS) -nostdlib -lk -lgcc


# Do not edit below
###################################################################################

KERNEL_SRC_DIR=kernel
KERNEL_LINKER_LIST+=$(addprefix $(KERNEL_SRC_DIR)/, $(KERNEL_OBJS)) 

# Builds MadOS.kernel
$(KERNEL): install-headers $(KERNEL_LINKER_LIST)
	$(LD) -T"kernel/arch/$(ARCH_DIR)/linker.ld" $(KERNEL_LINKER_LIST) -o kernel/$(KERNEL)

# Build a c file into a object file
%.o: %.c
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_LIBS) -c $< -o $@ 