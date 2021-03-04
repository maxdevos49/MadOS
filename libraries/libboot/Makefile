
BOOT_SYSROOT?=$(shell pwd)/boot_sysroot
USR_INCLUDE_DIR?=/usr/include
USR_LIB_DIR?=/usr/lib

CC?=x86_64-elf-gcc
AR?=x86_64-elf-ar
CFLAGS?=-O1 -MD -Wall -Wextra -ffreestanding -m64 --sysroot=$(BOOT_SYSROOT) -isystem=$(USR_INCLUDE_DIR)
CPPFLAGS?=

BINARIES=libboot.a

OBJS=boot/print.c\
	boot/string.c\
	boot/drivers/serial.c\
	boot/drivers/tty.c\
	boot/drivers/vga.c\

all: $(BINARIES)

libboot.a: $(OBJS)
	$(AR) rcs $@ $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -O3 $(CPPFLAGS) -c $< -o $@

.PHONY: install install-headers install-bin clean
install: install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' .  $(BOOT_SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(BINARIES)
	cp -RL $(BINARIES) $(BOOT_SYSROOT)$(USR_LIB_DIR)

clean: 
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
	@find . -type f -name "*.s" -delete -print