

NASM?=nasm
CC?=x86_64-elf-gcc
OBJCOPY?=x86_64-elf-objcopy
CFLAGS?=-Ttext 0x8000 -O1 -MD -Wall -Wextra -ffreestanding -m64
CPPFLAGS?=
LIBS?=-nostdlib


LOADERSIZE=

TARGET=loader

OBJS=\
	extended.o\
	loader.o\
	string.o\
	Drivers/serial.c\
	Drivers/print.c\


all: $(TARGET)

$(TARGET): $(OBJS)
	x86_64-elf-gcc $(CFLAGS)  $(CPPFLAGS) -T"linker.ld" $(OBJS) $(LIBS) -o $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	set -e;\
	SECTORCOUNT=$$(expr $$(wc -c $(TARGET).bin | awk '{print $$1}') / 512);\
	nasm -f bin -d SECTOR_COUNT=$$SECTORCOUNT boot.asm -o boot.bin;
	if [ $$SECTORCOUNT -gt 63 ]; then\
		echo "Loader sector count is too large";\
	fi

	cat boot.bin $(TARGET).bin > $(TARGET)

%.bin: %.asm
	$(NASM) -f bin $< -o $@

%.o: %.asm
	$(NASM) -f elf64 $< -o $@

%.o: %.c
	x86_64-elf-gcc $(CFLAGS) $(CPPFLAGS) $(LIBS) -c $< -o $@


.PHONY: clean run
run: $(TARGET)
	qemu-system-x86_64 -no-reboot -no-shutdown -drive format=raw,media=disk,file=$(TARGET) -serial stdio

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.bin" -delete -print
	@find . -type f -name "*.elf" -delete -print
	@find . -type f -name "$(TARGET)" -delete -print