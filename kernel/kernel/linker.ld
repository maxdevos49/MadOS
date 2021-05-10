OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start)

SECTIONS
{
	. = 0xffffffff80200000;

	.stivale2hdr : {
        KEEP(*(.stivale2hdr))
    }

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