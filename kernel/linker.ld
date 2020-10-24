/* OUTPUT_FORMAT(elf64-x86-64) */
OUTPUT_FORMAT(binary)
/* https://stackoverflow.com/questions/34129461/should-i-make-my-own-os-kernel-elf-or-raw-binary */

ENTRY(kernel_main)

SECTIONS
{
	. = 0x8000;

	.text : ALIGN(0x1000)
	{
		*(.text)
	}

	.idt BLOCK(0x1000) : ALIGN(0x1000)
	{
		_idt = .;
		. = . + 0x1000;
	}

	.data : ALIGN(0x1000)
	{
		*(.data)
	}

	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
	}

	.bss : ALIGN(0x1000){
		*(COMMON)
		*(.bss)
    }
}