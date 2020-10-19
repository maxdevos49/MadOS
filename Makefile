###########################################################
#---------------- MadOS Make file system -----------------#
###########################################################

# Chnage this to target another platform
ARCH_NAME=x86_64
ARCH_DIR=x86_64

AR=$(ARCH_NAME)-elf-ar
CC=$(ARCH_NAME)-elf-gcc
LD=$(ARCH_NAME)-elf-ld
QEMU=qemu-system-$(ARCH_NAME)

TARGET=MadOS_$(ARCH_NAME).img
KERNEL=MadOS.kernel

CFLAGS= -O0 -MD -Wall -Wextra -ffreestanding -m64 --sysroot=$(SYS_ROOT) -isystem=$(USR_INCLUDE_DIR)
CPPFLAGS=  #-D__test

BINARIES=libk.a # libc.a

# Do not edit below
###################################################################################

SYS_ROOT=$(shell pwd)/sysroot
USR_INCLUDE_DIR=/usr/include
USR_LIB_DIR=/usr/lib

SYS_ROOT_PATHS=\
	$(SYS_ROOT)/boot \
	$(SYS_ROOT)$(USR_INCLUDE_DIR)\
	$(SYS_ROOT)$(USR_LIB_DIR)

all: $(TARGET)

include libc/libc.Makefile
include kernel/kernel.Makefile

# Builds MadOS_<arch>.img
$(TARGET): install-headers install-libraries $(BOOTLOADER) $(KERNEL)
	dd if=/dev/zero of=$(TARGET) bs=512 count=200 &> /dev/null
	cat $(BOOTLOADER) kernel/$(KERNEL) | dd of=$(TARGET) conv=notrunc &> /dev/null

$(SYS_ROOT_PATHS):
	mkdir -p $@


.PHONY: clean run install-headers install-libraries

install-headers: $(SYS_ROOT_PATHS)
	rsync -r -u kernel/include/* libc/include/* $(SYS_ROOT)$(USR_INCLUDE_DIR)

install-libraries: $(BINARIES) $(SYS_ROOT_PATHS)
	rsync -r -u libc/libk.a $(SYS_ROOT)$(USR_LIB_DIR)

run: $(TARGET)
	$(QEMU) -drive format=raw,media=disk,file=$(TARGET)

clean: 
	@echo Cleaning Project
	@rm -rf sysroot
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
	@find . -type f -name "*.kernel" -delete -print
	@find . -type f -name "*.bin" -delete -print
	@find . -type f -name "*.img" -delete -print


