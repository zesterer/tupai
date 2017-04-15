ENTRY(start)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
	. = 1M;
	KERNEL_VIRTUAL = 0xFFFFFFFF80000000;

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
	}

	.bss.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.bss.boot)
	}

	. += KERNEL_VIRTUAL;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL)
	{
		*(.text)
		*(.init)
		*(.fini)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL)
	{
		*(COMMON)
		*(.bss)
	}
}
