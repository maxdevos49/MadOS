###################################################################################
#--------------------------Boot Library Makefile options--------------------------#
###################################################################################

# Defaults
CC?=x86_64-elf-gcc
AR?=x86_64-elf-ar
CFLAGS?=-MD -Wall -Wextra -m64
CPPFLAGS?=-nostdlib

# Target Lib
LIB_TARGET=libboot.a
LIBBOOT_CFLAGS=-O1 -ffreestanding -mno-red-zone --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR)
LIBBOOT_CPPFLAGS=

# Object Files
LIBBOOT_OBJS=\
	boot/cpu.o\
	boot/drivers/serial.o\
	boot/drivers/tty.o\
	boot/drivers/vga.o\
	boot/drivers/pci.o\
	boot/stdio/printf.o\
	boot/stdio/serial_printf.o\
	boot/stdio/sprintf.o\
	boot/stdio/vsprintf.o\
	boot/string/string.o\
	boot/stdlib/malloc.o\


# Do not edit below
###################################################################################
LIBBOOT_CFLAGS:=$(CFLAGS) $(LIBBOOT_CFLAGS)
LIBBOOT_CPPFLAGS:=$(CPPFLAGS) $(LIBBOOT_CPPFLAGS)
LIBBOOT_DEPS:=$(LIBBOOT_OBJS:.o=.d)

all: $(LIB_TARGET)

$(LIB_TARGET): $(LIBBOOT_OBJS)
	$(AR) rcs $@ $(LIBBOOT_OBJS) 

%.o: %.c
	$(CC) $(LIBBOOT_CFLAGS) $(LIBBOOT_CPPFLAGS) -c $< -o $@

-include $(LIBBOOT_DEPS)

.PHONY: install install-headers install-bin clean
install: install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' .  $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(LIB_TARGET)
	cp -RL $(LIB_TARGET) $(SYSROOT)$(USR_LIB_DIR)

clean: 
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
	@find . -type f -name "*.s" -delete -print