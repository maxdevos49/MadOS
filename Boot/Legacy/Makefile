

NASM?=nasm

BINARIES=boot.bin

all: $(BINARIES)

%.bin: %.asm
	$(NASM) -f bin $< -o $@

%.o: %.asm
	$(NASM) -f elf64 $< -o $@

.PHONY: clean
clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.bin" -delete -print