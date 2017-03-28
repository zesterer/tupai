ENTRY(start)

SECTIONS
{
	/* Start at LOADER_ADDR */
	. = 0x8000;

	.text.boot BLOCK(4K) : ALIGN(4K)
	{
		KEEP(*(.text.boot))
	}

	.data.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata.boot)
		*(.data.boot)
		*(.bss.boot)
	}

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
		*(.init)
		*(.fini)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		__bss_start = .;
		bss = .;
		*(.bss)
		__bss_end = .;
	}
}
