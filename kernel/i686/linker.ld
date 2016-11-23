/* Entry point of the operating system. The bootloader will begin execution at this label */
ENTRY(_boot_entry)

SECTIONS
{
	/* Offset the kernel at 1MB (to avoid interfering with the bootloader or any low-level initial code */
	. = 1M;

	/* Place executable boot code within a 4KB-aligned block */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.boot_text)
	}

	/* Read-write data (uninitialized) */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.boot_stack)
	}

	/* Main kernel code */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	/* Any other sections produced by the compiler *should* be placed after this point */
}
