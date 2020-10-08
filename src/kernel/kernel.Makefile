#Alias's
ECHO?=echo
CC?=x86_64-elf-gcc

BUILD_DIR?=build 
DIST_DIR?=dist
ARCH_DIR?=x86_64

REQUIRED_DIR+=$(BUILD_DIR)/kernel/kernel/
KERNEL_OBJS= kernel.o


# Do not edit below
###################################################################################

# Include architecture info
include src/kernel/arch/$(ARCH_DIR)/$(ARCH_DIR).Makefile
LINKER_LIST+=$(addprefix $(BUILD_DIR)/kernel/kernel/, $(KERNEL_OBJS))

# Build a c file into a 
$(BUILD_DIR)/kernel/kernel/%.o: ./src/kernel/kernel/%.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@