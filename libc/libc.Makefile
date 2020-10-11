
LIBC_OBS=
LIBK_OBJS=

ARCH_FREESTANDING_OBJS=
FREESTANDING_OBJS=

# Do not edit below
###################################################################################

REQUIRED_DIR+=

# Include architecture info
include libc/arch/$(ARCH_DIR)/$(ARCH_NAME).Makefile

# REQUIRED_DIR+=$(BUILD_DIR)/libc/
LINKER_LIST+=$(addprefix $(BUILD_DIR)/libc/, $(LIBC_OBS))

# Build a c file into a object file
# $(BUILD_DIR)/kernel/kernel/%.o: ./src/kernel/kernel/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)