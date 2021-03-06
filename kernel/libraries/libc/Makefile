###########################################################
#------------------ LIBC/LIBK Makefile -------------------#
###########################################################

# Defaults
CC?=x86_64-elf-gcc
AR?=x86_64-elf-ar
CFLAGS?=-O1 -MD -Wall -Wextra -ffreestanding -m64 --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR) # likely will not work stand alone unless explicit sysroot exist and is defined
CPPFLAGS?=

# Flags
LIBC_CFLAGS:= $(CFLAGS) -O1
LIBC_CPPFLAGS:= $(CPPFLAGS)  -D__is_libc -Iinclude

LIBK_CFLAGS:= $(LIBC_CFLAGS) -O1
LIBK_CPPFLAGS:= $(LIBC_CPPFLAGS) -D__is_libk

# Output Binaries
BINARIES=libk.a #libc.a

# Objects
OBJS=\
	stdio/printf.o \
	stdio/putchar.o \
	stdio/puts.o \
	stdio/sprintf.o \
	string/strlen.o\
	string/strcmp.o\
	string/strcpy.o\
	string/memcmp.o\
	string/memcpy.o\
	string/memmove.o\
	string/memset.o\
	string/memset64.o\
	stdlib/abort.o\
	stdlib/rand.o\
	stdlib/abs.o\
	stdlib/malloc.o\
	stdlib/realloc.o\
	stdlib/calloc.o\
	stdlib/free.o\
	time/mktime.o\
	time/clock.o\
	time/ctime.o\
	time/time.o


# Do not edit below
###################################################################################
LIBC_DEPS:=$(OBJS:.o=.d)

$(BINARIES): $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(LIBC_CFLAGS) $(LIBC_CPPFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -c $< -o $@

%.s: %.c
	$(CC) $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -S -masm=intel -fverbose-asm $< -o $@


# Not sure if this is doing anything or not
-include $(LIBC_DEPS)

.PHONY: install install-headers install-bin clean
install: install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' .  $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(BINARIES)
	cp -RL $(BINARIES) $(SYSROOT)$(USR_LIB_DIR)

debug: $(OBJS:.o=.s)

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
	@find . -type f -name "*.s" -delete -print
