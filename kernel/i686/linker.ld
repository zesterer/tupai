/* Entry point of the operating system. The bootloader will begin execution at this label */
ENTRY(_boot_entry)

SECTIONS
{
	/* Offset the kernel at 1MB (to avoid interfering with the bootloader or any low-level initial code */
	. = 1M;

	/* Place executable boot code within a 4KB-aligned block */
	.rodata.multiboot BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
	}

	/* Kernel boot code */
	.text.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.text.boot)
	}

	/* Kernel boot data */
	.data.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.data.boot)
		*(.rodata.boot)
		*(.bss.boot)
	}

	. += 0xC0000000;

	/* Main kernel code */
	.text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
		*(.init)
		*(.fini)
	}

	/* Read-only data */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) */
	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.boot_stack)
		*(COMMON)
		*(.bss)
	}

	/* Any other sections produced by the compiler *should* be placed after this point */
}
