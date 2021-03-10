###################################################################################
#-----------------------Legacy Bootloader Makefile options------------------------#
###################################################################################

# Defaults
NASM?=nasm
CC?=x86_64-elf-gcc
OBJCOPY?=x86_64-elf-objcopy
CFLAGS?=-MD -Wall -Wextra -m64 
CPPFLAGS?=
LIBS?=-nostdlib

# Target Options
LEGACY_TARGET=loader
LEGACY_CFLAGS=-Ttext 0x8000 -ffreestanding -mno-red-zone --sysroot=$(SYSROOT) -isystem=$(USR_INCLUDE_DIR)
LEGACY_CPPFLAGS=
LEGACY_LIBS=-lboot

# Object Files
LEGACY_OBJS=\
	extended.o\
	legacy.o\

# Ensures if we make a change to the library it gets relinked 
# without making a random change. Only needed for statically
# linked libraries.
LEGACY_LIB_PATHS=\
	 ../libraries/libboot/libboot.a

# Do not edit below
###################################################################################
LEGACY_CFLAGS:=$(CFLAGS) $(LEGACY_CFLAGS)
LEGACY_CPPFLAGS:=$(CFLAGS) $(LEGACY_CPPFLAGS)
LEGACY_LIBS:=$(LIBS) $(LEGACY_LIBS)
LEGACY_DEPS:=$(LEGACY_OBJS:.o=.d)

all: $(LEGACY_TARGET)

$(LEGACY_TARGET): $(LEGACY_OBJS) $(LEGACY_LIB_PATHS)
	x86_64-elf-gcc $(LEGACY_CFLAGS) $(LEGACY_CPPFLAGS) -T"linker.ld" $(LEGACY_OBJS) $(LEGACY_LIBS) -o $(LEGACY_TARGET).elf
	$(OBJCOPY) -O binary $(LEGACY_TARGET).elf $(LEGACY_TARGET).bin
	set -e;\
	SECTORCOUNT=$$(expr $$(wc -c $(LEGACY_TARGET).bin | awk '{print $$1}') / 512);\
	nasm -f bin -d SECTOR_COUNT=$$SECTORCOUNT boot.asm -o boot.bin;\
	echo "SECTOR_COUNT=$$SECTORCOUNT"

	cat boot.bin $(LEGACY_TARGET).bin > $(LEGACY_TARGET)

%.bin: %.asm
	$(NASM) -f bin $< -o $@

%.o: %.asm
	$(NASM) -f elf64 $< -o $@

%.o: %.c
	x86_64-elf-gcc $(LEGACY_CFLAGS) $(LEGACY_CPPFLAGS) $(LEGACY_LIBS) -c $< -o $@

-include $(LEGACY_DEPS)

.PHONY: install install-headers install-bin clean
install: install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' .  $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(LEGACY_TARGET)
	cp -RL $(LEGACY_TARGET) $(SYSROOT)$(USR_LIB_DIR)

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.bin" -delete -print
	@find . -type f -name "*.elf" -delete -print
	@find . -type f -name "$(LEGACY_TARGET)" -delete -print