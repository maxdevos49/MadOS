###################################################################################
#-----------------------------Kernel Makefile options-----------------------------#
###################################################################################

# Do not edit below
###################################################################################
include src/kernel/arch/$(ARCH_DIR)/$(ARCH_NAME).Makefile
###################################################################################
# Do not edit above


KERNEL_OBJS= \
	kernel/kernel.o 

KERNEL_CFLAGS:= $(CFLAGS) -ffreestanding  -Ttext 0x8000


# Do not edit below
###################################################################################

KERNEL_SRC_DIR=src/kernel
LINKER_LIST+=$(addprefix $(KERNEL_SRC_DIR)/, $(KERNEL_OBJS))

# Build a c file into a object file
%.o: %.c
	$(CC) $(KERNEL_CFLAGS) -c $< -o $@