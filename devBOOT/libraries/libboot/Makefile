###################################################################################
#--------------------------Boot Library Makefile options--------------------------#
###################################################################################

# Defaults
CC?=x86_64-elf-gcc
AR?=x86_64-elf-ar
CFLAGS?=-O1 -MD -Wall -Wextra -m64 # -ffreestanding -mno-red-zone --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR)
CPPFLAGS?=

# Target Lib
LIB_TARGET=libboot.a
LIBBOOT_CFLAGS=-ffreestanding -mno-red-zone --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR)
LIBBOOT_CPPFLAGS=

# Object Files
OBJS=boot/print.c\
	boot/string.c\
	boot/drivers/serial.c\
	boot/drivers/tty.c\
	boot/drivers/vga.c\


# Do not edit below
###################################################################################
LIBBOOT_CFLAGS:=$(CFLAGS) $(LIBBOOT_CFLAGS)
LIBBOOT_CPPFLAGS:=$(CPPFLAGS) $(LIBBOOT_CPPFLAGS)

all: $(LIB_TARGET)

$(LIB_TARGET): $(OBJS)
	$(AR) rcs $@ $(OBJS) 

%.o: %.c
	$(CC) $(LIBBOOT_CFLAGS) -O3 $(LIBBOOT_CPPFLAGS) -c $< -o $@

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