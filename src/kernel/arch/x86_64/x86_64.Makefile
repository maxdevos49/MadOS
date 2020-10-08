
X86_64_BOOTLOADER=bootloader.bin
X86_64_OBS=extended_boot.o

# Do not edit below
###################################################################################
LINKER_LIST+=$(addprefix $(BUILD_DIR)/kernel/arch/x86_64/, $(X86_64_OBS))
BOOTLOADER=$(DIST_DIR)/$(X86_64_BOOTLOADER)

REL_SRC_DIR=src/kernel/arch/x86_64/
REL_BUILD_DIR=$(BUILD_DIR)/kernel/arch/x86_64/

# Assembles a asm file to a elf64 binary file
$(REL_BUILD_DIR)%.o: ./$(REL_SRC_DIR)%.asm
	@$(ECHO) Assembling $<
	@$(NASM) -i$(REL_SRC_DIR) -f elf64 $< -o $@

# Assembles a asm file into a raw binary file
$(DIST_DIR)/%.bin: $(REL_SRC_DIR)bootloader.asm
	@$(ECHO) Assembling $<
	@$(NASM) -i$(REL_SRC_DIR) -f bin $< -o $(DIST_DIR)/bootloader.bin