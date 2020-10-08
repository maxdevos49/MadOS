###########################################################
################# MadOS Make file system ##################
###########################################################

TARGET=MadOS_$(ARCH_DIR).img
BIN=MadOS_$(ARCH_DIR).bin
BOOTLOADER=bootloader_$(ARCH_DIR).bin
BOOTLOADER_DEP=$(BUILD_DIR)/kernel/arch/$(ARCH_DIR)/$(BOOTLOADER)

BUILD_DIR=build
DIST_DIR=dist
ARCH_DIR=x86_64

REQUIRED_DIR = $(BUILD_DIR) $(DIST_DIR) $(BUILD_DIR)/kernel/arch/$(ARCH_DIR)/

# List of objects to link. Populated in sub makefiles
CFLAGS = -g -Wall -Wextra -Ttext 0x8000 -ffreestanding -mno-red-zone -m64
LINKER_LIST=

# ALIASES
ECHO=echo
CC=x86_64-elf-gcc
LD=x86_64-elf-ld
RM=rm
MKDIR=mkdir
NASM=nasm
QEMU = qemu-system-x86_64

# Do not edit below
###################################################################################

include src/kernel/kernel.Makefile
include src/libc/libc.Makefile

all: $(BIN)

# Builds MadOS_arch.kernel
$(BIN): $(REQUIRED_DIR) $(BOOTLOADER_DEP) $(LINKER_LIST)
	@$(ECHO) Linking $(BIN) $(LINKER_LIST)
	@$(LD) -T"./src/kernel/arch/$(ARCH_DIR)/linker.ld" $(LINKER_LIST) -o $(DIST_DIR)/kernel_$(ARCH_DIR).tmp
	@cat $(DIST_DIR)/$(BOOTLOADER) $(DIST_DIR)/kernel_$(ARCH_DIR).tmp > $(DIST_DIR)/$(BIN)
	@$(RM) -f $(DIST_DIR)/kernel_$(ARCH_DIR).tmp

# Create any necessary folders
$(REQUIRED_DIR):
	@$(ECHO) Creating directory $@
	@$(MKDIR) -p $@

.PHONY: clean image run
.SILENT:image

image: $(BIN)
	@$(ECHO) Creating Floppy image
	@dd if=/dev/zero of=./dist/$(TARGET) bs=512 count=200
	@dd if=./dist/$(BIN) of=./dist/$(TARGET) conv=notrunc

run: $(BIN)
	@$(ECHO) Running kernel in qemu.
	@$(QEMU) -fda dist/$(TARGET)


clean: 
	@$(ECHO) Cleaning project
	@$(RM) -rf $(BUILD_DIR) $(DIST_DIR)

