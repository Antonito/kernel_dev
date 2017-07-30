OUTPUT_FORMAT("elf32-i386")
ENTRY(start)
phys = 0x00100000;
offset = 0xC0000000;
virt = phys + offset;
SECTIONS {
	. = virt;
	kernel_start = .;
	.text : AT(ADDR(.text) - offset)
	{
		code = .; _code = .; __code = .;
		*(.text)
		*(.eh_frame)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data) - offset)
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	.bss : AT(ADDR(.bss) - offset)
	{
		bss = .; _bss = .; __bss = .;
		*(COMMON)
		*(.bss)
		. = ALIGN(4096);
	}

	_end = .;
	end = .;
}
