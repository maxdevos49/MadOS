###################################################################################
#-----------------------Legacy Bootloader Makefile options------------------------#
###################################################################################

# Target Options
LEGACY_BOOTLOADER=$(PROJECT_OBJ)/boot.bin
LEGACY_EXT_BOOTLOADER=$(PROJECT_OBJ)/extended.bin
SECTOR_COUNT=40
LEGACY_KERNEL_LOADER=$(PROJECT_OBJ)/BOOTX64.legacy

LEGACY_TARGETS=$(LEGACY_BOOTLOADER) #$(LEGACY_KERNEL_LOADER) 

LEGACY_CFLAGS=-Ttext 0x8000 -ffreestanding -mno-red-zone --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR)
LEGACY_CPPFLAGS=
LEGACY_LIBS=-lboot -nostdlib -lgcc

# Object Files
LEGACY_OBJS=\
	legacy.o\

# Ensures if we make a change to the library it gets relinked 
# without making a random change. Only needed for statically
# linked libraries.
LEGACY_LIB_PATHS=\
	 $(PROJECT_OBJ)/../libraries/libboot/libboot.a

# Do not edit below
###################################################################################
LEGACY_CFLAGS:=$(CFLAGS) $(LEGACY_CFLAGS)
LEGACY_CPPFLAGS:=$(CFLAGS) $(LEGACY_CPPFLAGS)
LEGACY_LIBS:=$(LIBS) $(LEGACY_LIBS)
LEGACY_OBJS:=$(addprefix $(PROJECT_OBJ)/, $(LEGACY_OBJS))
LEGACY_DEPS:=$(LEGACY_OBJS:.o=.d)


$(LEGACY_KERNEL_LOADER): $(LEGACY_OBJS) $(LEGACY_LIB_PATHS)
	$(CC) $(LEGACY_CFLAGS) $(LEGACY_CPPFLAGS) -T"linker.ld" $(LEGACY_OBJS) $(LEGACY_LIBS) -o $(LEGACY_KERNEL_LOADER).elf
	$(OBJCOPY) -O binary $(LEGACY_KERNEL_LOADER).elf $(LEGACY_KERNEL_LOADER)

$(LEGACY_BOOTLOADER): $(LEGACY_EXT_BOOTLOADER)
	$(NASM) -f bin -d SECTOR_COUNT=$(SECTOR_COUNT) $(PROJECT_PATH)/boot.asm -o $(LEGACY_BOOTLOADER)


# Individual Rules

$(PROJECT_OBJ)/%.bin: $(PROJECT_PATH)/%.asm
	$(NASM) -f bin $< -o $@


$(PROJECT_OBJ)/%.o: $(PROJECT_PATH)/%.c
	$(CC) $(LEGACY_CFLAGS) $(LEGACY_CPPFLAGS) $(LEGACY_LIBS) -c $< -o $@


# Dependencies
-include $(LEGACY_DEPS)

.PHONY: install-bin clean
install-bin: $(LEGACY_TARGETS)
	dd if=$(LEGACY_BOOTLOADER) of=$(IMG_PATH) conv=notrunc bs=1 count=3
	dd if=$(LEGACY_BOOTLOADER) of=$(IMG_PATH) conv=notrunc bs=1 count=350 skip=90 seek=90
	dd if=$(LEGACY_EXT_BOOTLOADER) of=$(IMG_PATH) conv=notrunc bs=512 count=$(SECTOR_COUNT) seek=7
	mcopy -s -i $(IMG_PATH) $(SYSROOT)/usr ::

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.bin" -delete -print
	@find . -type f -name "*.elf" -delete -print
	@find . -type f -name "*.img" -delete -print