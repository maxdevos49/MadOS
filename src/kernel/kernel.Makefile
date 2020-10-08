
KERNEL_OBJS= kernel.o

# Do not edit below
###################################################################################

# Include architecture info
include src/kernel/arch/$(ARCH_DIR)/$(ARCH_NAME).Makefile

REQUIRED_DIR+=$(BUILD_DIR)/kernel/kernel/
LINKER_LIST+=$(addprefix $(BUILD_DIR)/kernel/kernel/, $(KERNEL_OBJS))

# Build a c file into a object file
$(BUILD_DIR)/kernel/kernel/%.o: ./src/kernel/kernel/%.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@