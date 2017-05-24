ENTRY(start)
OUTPUT_FORMAT(arm-none-eabi)

SECTIONS
{
	/* Start at LOADER_ADDR */
	. = 0x8000;
	KERNEL_VIRTUAL = 0x0;

	kernel_start = .;

	.text.boot BLOCK(4K) : ALIGN(4K)
	{
		KEEP(*(.text.boot))
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
		__bss_start = .;
		bss = .;
		*(.bss)
		__bss_end = .;
	}

	. -= KERNEL_VIRTUAL;
	kernel_end = .;
}
