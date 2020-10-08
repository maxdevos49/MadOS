###########################################################
################# MadOS Make file system ##################
###########################################################

ARCH_NAME=x86_64
ARCH_DIR=x86_64

BUILD_DIR:=build_$(ARCH_NAME)
DIST_DIR:=dist_$(ARCH_NAME)

TARGET=MadOS_$(ARCH_NAME).img
BIN=MadOS_$(ARCH_NAME).kernel

CFLAGS=-g -Wall -Wextra -Ttext 0x8000 -ffreestanding -mno-red-zone -m64

ECHO=echo
CC=x86_64-elf-gcc
LD=x86_64-elf-ld
RM=rm
MKDIR=mkdir
NASM=nasm
QEMU=qemu-system-x86_64
DD=dd
CAT=cat

# Do not edit below
###################################################################################

LINKER_LIST= 
BOOTLOADER= 
REQUIRED_DIR= $(BUILD_DIR) $(DIST_DIR) $(BUILD_DIR)/kernel/arch/$(ARCH_DIR)/ 
BIN_REQUIREMENTS= $(REQUIRED_DIR) $(BOOTLOADER) $(LINKER_LIST)

include src/kernel/kernel.Makefile
include src/libc/libc.Makefile

all: $(TARGET)

# Builds MadOS_<arch>.bin
$(BIN): $(BIN_REQUIREMENTS)
	@$(ECHO) Linking $(BIN)
	@$(LD) -T"./src/kernel/arch/$(ARCH_DIR)/linker.ld" $(LINKER_LIST) -o $(DIST_DIR)/kernel_$(ARCH_DIR).tmp
	@$(CAT) $(BOOTLOADER) $(DIST_DIR)/kernel_$(ARCH_DIR).tmp > $(DIST_DIR)/$(BIN)
	@$(RM) -f $(DIST_DIR)/kernel_$(ARCH_DIR).tmp

# Builds MadOS_<arch>.img
$(TARGET): $(BIN)
	@$(ECHO) Creating floppy image $@
	@$(DD) if=/dev/zero of=$(DIST_DIR)/$(TARGET) bs=512 count=200 &> /dev/null
	@$(DD) if=$(DIST_DIR)/$(BIN) of=$(DIST_DIR)/$(TARGET) conv=notrunc &> /dev/null

# Create any necessary folders
$(REQUIRED_DIR):
	@$(ECHO) Creating directory $@
	@$(MKDIR) -p $@

.PHONY: clean run

run: $(TARGET)
	@$(ECHO) Running kernel in qemu.
	@$(QEMU) -fda $(DIST_DIR)/$(TARGET)

clean: 
	@$(ECHO) Cleaning project
	@$(RM) -rf $(BUILD_DIR) $(DIST_DIR)

