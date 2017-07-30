OUTPUT_FORMAT("elf32-i386")
ENTRY(start)

KERNEL_PHYS = 0x00100000;
KERNEL_OFFSET = 0x00000000;
KERNEL_VIRT = KERNEL_PHYS + KERNEL_OFFSET;

SECTIONS
{
		. = KERNEL_VIRT;
		.text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_OFFSET)
		{
			*(.text)
		}

		.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_OFFSET)
		{
			*(.rodata)
		}

		.data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_OFFSET)
		{
			*(.data)
		}

		.bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_OFFSET)
		{
			*(COMMON)
			*(.bss)
		}

	KERNEL_VEND = .;
	KERNEL_END = . - KERNEL_OFFSET;
}
