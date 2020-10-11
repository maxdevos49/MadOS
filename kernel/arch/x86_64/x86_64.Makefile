###################################################################################
#-----------------------------x86_64 architecture options-------------------------#
###################################################################################


X86_64_BOOTLOADER=bootloader.bin
X86_64_OBS=\
	extended_boot.o \
	tty.o \
	io.o


# Do not edit below
###################################################################################

X86_64_SRC_DIR=kernel/arch/x86_64
KERNEL_LINKER_LIST+=$(addprefix $(X86_64_SRC_DIR)/, $(X86_64_OBS))
BOOTLOADER=$(X86_64_SRC_DIR)/$(X86_64_BOOTLOADER)

# Assembles a asm file to a elf64 binary file
%.o: %.asm
	nasm -i$(X86_64_SRC_DIR) -f elf64 $< -o $@

# Assembles a asm file into a raw binary file
%.bin: %.asm
	nasm -i$(X86_64_SRC_DIR) -f bin $< -o $@