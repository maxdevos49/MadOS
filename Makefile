###########################################################
################# MadOS Make file system ##################
###########################################################

# Chnage this to target another platform
ARCH_NAME=x86_64
ARCH_DIR=x86_64

TARGET=MadOS_$(ARCH_NAME).img
KERNEL=MadOS.kernel

SYS_ROOT=$(shell pwd)/sysroot
USR_INCLUDE_DIR=/usr/include
USR_LIB_DIR=/usr/lib

SYS_ROOT_PATHS=\
	$(SYS_ROOT)/boot \
	$(SYS_ROOT)$(USR_INCLUDE_DIR)\
	$(SYS_ROOT)$(USR_LIB_DIR)

CFLAGS=-MD -Wall -Wextra -mno-red-zone -m64 --sysroot=$(SYS_ROOT) -isystem=$(USR_INCLUDE_DIR)

CC=x86_64-elf-gcc
LD=x86_64-elf-ld
QEMU=qemu-system-x86_64

# Do not edit below
###################################################################################

LINKER_LIST= 
BOOTLOADER= 

ALL_REQUIREMENTS= \
	install-headers \
	install-libraries \
	$(LINKER_LIST)

include src/kernel/kernel.Makefile
# include src/libc/libc.Makefile

all: $(TARGET)

# Builds MadOS.kernel
$(KERNEL): $(ALL_REQUIREMENTS)
	$(LD) -T"src/kernel/arch/$(ARCH_DIR)/linker.ld" $(LINKER_LIST) -o src/kernel/$(KERNEL)
	cp src/kernel/$(KERNEL) $(SYS_ROOT)/boot

# Builds MadOS_<arch>.img
$(TARGET): $(BOOTLOADER) $(KERNEL)
	dd if=/dev/zero of=$(TARGET) bs=512 count=200 &> /dev/null
	cat $(BOOTLOADER) src/kernel/$(KERNEL) | dd of=$(TARGET) conv=notrunc &> /dev/null


.PHONY: clean run install-headers install-libraries sysroot

install-headers: $(SYS_ROOT_PATHS)
	cp -Rv src/kernel/include/. src/libc/include/. $(SYS_ROOT)$(USR_INCLUDE_DIR)

install-libraries: $(SYS_ROOT_PATHS) # libk.a libc.a
	@echo todo
	
$(SYS_ROOT_PATHS):
	mkdir -p $@

run: $(TARGET)
	$(QEMU) -fda $(TARGET)

clean: 
	@echo Cleaning Project
	@rm -rfv sysroot
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.kernel" -delete -print
	@find . -type f -name "*.bin" -delete -print
	@find . -type f -name "*.img" -delete -print

