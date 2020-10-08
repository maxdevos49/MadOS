# Alias's
ECHO?=echo
MKDIR?=mkdir
NASM?=nasm

LINKER_LIST+=$(addprefix $(BUILD_DIR)/kernel/arch/x86_64/, extended_boot.o)

# Do not edit below
###################################################################################

# Assembles a asm file to a elf64 binary file
$(BUILD_DIR)/kernel/arch/x86_64/%.o: ./src/kernel/arch/x86_64/%.asm
	@$(ECHO) Assembling $<
	@$(NASM) -isrc/kernel/arch/x86_64/ -f elf64 $< -o $@

# Assembles a asm file into a raw binary file
$(BUILD_DIR)/kernel/arch/x86_64/%.bin: ./src/kernel/arch/x86_64/%.asm
	@$(ECHO) Assembling $<
	@$(NASM) -isrc/kernel/arch/x86_64/ -f bin $< -o $(DIST_DIR)/$(BOOTLOADER)