

NASM?=nasm

BINARIES=boot.bin extended.bin

TARGET=loader.bin

all: $(TARGET)

$(TARGET): $(BINARIES)
	cat $(BINARIES) > $(TARGET)

%.bin: %.asm
	$(NASM) -f bin $< -o $@

%.o: %.asm
	$(NASM) -f elf64 $< -o $@

.PHONY: clean run

run: $(TARGET)
	qemu-system-x86_64 -no-reboot -no-shutdown -drive format=raw,media=disk,file=loader.bin

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.bin" -delete -print