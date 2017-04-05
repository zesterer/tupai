ENTRY(start)

SECTIONS
{
	. = 1M;
	KERNEL_VIRTUAL = 0xFFFF800000000000;

	/* Place the Multiboot header at the beginning */
	.rodata.multiboot BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata.multiboot)
	}

	.text.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.text.boot)
	}

	.data.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata.boot)
		*(.data.boot)
		*(.bss.boot)
	}

	. += KERNEL_VIRTUAL;

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
		*(.bss)
	}
}
