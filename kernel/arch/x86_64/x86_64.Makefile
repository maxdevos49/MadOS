###################################################################################
#-----------------------------x86_64 architecture options-------------------------#
###################################################################################


X86_64_BOOTLOADER=bootloader/bootloader.k.bin
X86_64_OBS=\
	extended_boot/extended_boot.o \
	tty.o \
	io.o \
	debug.o\
	binaries.o\
	IDT.o\
	keyboard.o\
	memory.o\
	heap/init_heap.o\
	heap/k_malloc.o\
	heap/k_calloc.o\
	heap/k_realloc.o\
	heap/k_free.o\
	


# Do not edit below
###################################################################################

X86_64_SRC_DIR=kernel/arch/x86_64
KERNEL_LINKER_LIST+=$(addprefix $(X86_64_SRC_DIR)/, $(X86_64_OBS))
BOOTLOADER=$(X86_64_SRC_DIR)/$(X86_64_BOOTLOADER)

# Assembles a asm file to a elf64 binary file
%.k.o: %.asm
	nasm -i$(X86_64_SRC_DIR)/extended_boot -f elf64 $< -o $@

# Assembles a asm file into a raw binary file
%.k.bin: %.asm
	nasm -i$(X86_64_SRC_DIR)/bootloader -f bin $< -o $@