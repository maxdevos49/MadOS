/* https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware  */
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
	. = 0x8000;

	.text : ALIGN(0x1000)
	{
        _s_text = .;
		*(.text)
        _e_text = .;
	}

	.rodata : ALIGN(0x1000)
	{
        _s_rodata = .;
		*(.rodata)
        _e_rodata = .;
	}

	.data : ALIGN(0x1000)
	{
        _s_data = .;
		*(.data)
        _e_data = .;
	}

	.bss : ALIGN(0x1000){
        _s_bss = .;
		*(COMMON)
		*(.bss)
        _e_bss = .;
    }
}