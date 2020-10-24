###########################################################
#------------------ LIBC/LIBK Makefile -------------------#
###########################################################

# Do not edit below
###################################################################################
include libc/arch/$(ARCH_DIR)/$(ARCH_NAME).Makefile
###################################################################################
# Do not edit above

LIBC_CFLAGS:= $(CFLAGS) $(ARCH_LIBC_CFLAGS)
LIBC_CPPFLAGS:= $(CPPFLAGS) $(ARCH_LIBC_CPPFLAGS) -D__is_libc -Iinclude

LIBK_CFLAGS:= $(LIBC_CFLAGS) $(ARCH_LIBK_CPPFLAGS)
LIBK_CPPFLAGS:= $(LIBC_CPPFLAGS) $(ARCH_LIBK_CPPFLAGS) -D__is_libk


FREESTANDING_OBJS=\
	$(ARCH_FREESTANDING_OBJS)\
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
	ctest/ctest.o


HOSTED_OBJS=\
	$(ARCH_HOSTED_OBJS)\

OBJS=\
	$(FREESTANDING_OBJS)\
	$(HOSTED_OBJS)\

# Do not edit below
###################################################################################
LIB_SRC_DIR=libc

# LIBC_OBJS=$(OBJS: .o=.libc.o)
# LIBK_OBJS=$(FREESTANDING_OBJS:.o=.libk.o)

LIBC_OBJS:=$(addprefix $(LIB_SRC_DIR)/, $(OBJS:.o=.libc.o)) 
LIBK_OBJS:=$(addprefix $(LIB_SRC_DIR)/, $(FREESTANDING_OBJS:.o=.libk.o)) 

LIBC_DEP:=$(LIBC_OBJS:.o=.d);
LIBK_DEP:=$(LIBK_OBJS:.o=.d);

-include $(LIBC_DEP)
-include $(LIBK_DEP)

libc.a: $(LIBC_OBJS)
	$(AR) rcs $(LIB_SRC_DIR)/$@ $(LIBC_OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $(LIB_SRC_DIR)/$@ $(LIBK_OBJS)

%.libc.o: %.c
	$(CC) $(LIBC_CFLAGS) $(LIBC_CPPFLAGS) -c $< -o $@

%.libk.o: %.c
	$(CC) $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -c $< -o $@

# Not needed yet
# %.libc.o: %.s
# 	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

# Not needed yet
# .s.libk.o:
# 	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

