###########################################################
#------------------ LIBC/LIBK Makefile -------------------#
###########################################################

# Defaults
CC?=x86_64-elf-gcc
AR?=x86_64-elf-ar
CFLAGS?=-O0 -MD -Wall -Wextra -ffreestanding -m64 --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR) # likely will not work stand alone unless explicit sysroot exist and is defined
CPPFLAGS?=

# Flags
LIBC_CFLAGS:= $(CFLAGS) 
LIBC_CPPFLAGS:= $(CPPFLAGS)  -D__is_libc -Iinclude

LIBK_CFLAGS:= $(LIBC_CFLAGS)
LIBK_CPPFLAGS:= $(LIBC_CPPFLAGS) -D__is_libk

# Output Binaries
BINARIES=libk.a #libc.a

# Objects
OBJS=\
	stdio/printf.o \
	stdio/putchar.o \
	stdio/puts.o \
	string/strlen.o\
	string/strcmp.o\
	string/memcmp.o\
	string/memcpy.o\
	string/memmove.o\
	string/memset.o\
	stdlib/abort.o\
	stdlib/rand.o\
	stdlib/abs.o\
	stdlib/malloc.o\
	stdlib/realloc.o\
	stdlib/calloc.o\
	stdlib/free.o\
	time/mktime.o\

# Do not edit below
###################################################################################

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(LIBC_CFLAGS) $(LIBC_CPPFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -c $< -o $@

.PHONY: install install-headers install-bin clean
install: all install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' .  $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(BINARIES)
	cp -RL $(BINARIES) $(SYSROOT)$(USR_LIB_DIR)

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
