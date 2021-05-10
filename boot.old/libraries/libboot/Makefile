###################################################################################
#--------------------------Boot Library Makefile options--------------------------#
###################################################################################

# Target Lib
LIB_TARGET=$(PROJECT_OBJ)/libboot.a
LIBBOOT_CFLAGS=-O1 -ffreestanding -mno-red-zone --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR)
LIBBOOT_LIBS=-nostdlib -lgcc

# Object Files
LIBBOOT_OBJS=\
	boot/cpu.o\
	boot/drivers/serial.o\
	boot/drivers/tty.o\
	boot/drivers/vga.o\
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
LIBBOOT_OBJS:=$(addprefix $(PROJECT_OBJ)/, $(LIBBOOT_OBJS))
LIBBOOT_DEPS:=$(LIBBOOT_OBJS:.o=.d)

$(LIB_TARGET): $(LIBBOOT_OBJS)
	$(AR) rcs $@ $(LIBBOOT_OBJS) 

# Individual Rules

$(PROJECT_OBJ)/%.o: $(PROJECT_PATH)/%.c
	$(CC) $(LIBBOOT_CFLAGS) $(LIBBOOT_CPPFLAGS) -c $< -o $@

# Dependencies
-include $(LIBBOOT_DEPS)

.PHONY: install-bin clean
install-bin: $(LIB_TARGET)
	cp -RL $(LIB_TARGET) $(SYSROOT)/$(USR_LIB_DIR)

clean: 
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
	@find . -type f -name "*.s" -delete -print