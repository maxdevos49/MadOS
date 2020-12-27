

NASM?=nasm

BINARIES=boot.bin

all: $(BINARIES)

%.bin: %.asm
	$(NASM) -f bin $< -o $@

%.o: %.asm
	$(NASM) -f elf64 $< -o $@

.PHONY: install install-bin clean
install: install-bin

install-bin: $(BINARIES)
	cp -L boot.bin $(SYSROOT)/boot

install-headers:
	@echo No Boot Headers

debug:

clean:
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.bin" -delete -print