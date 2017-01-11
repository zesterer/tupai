/* Entry point of the operating system. The bootloader will begin execution at this label */
ENTRY(_boot_entry)

SECTIONS
{
	/* Offset the kernel at 1MB (to avoid interfering with the bootloader or any low-level initial code */
	. = 1M;

	/* Place executable boot code within a 4KB-aligned block */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
	}

	/* Main kernel code */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text_begin)
		*(.boot_text)
		*(.text)
		*(.init)
		*(.fini)
		*(.text_end)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata_begin)
		*(.rodata)
		*(.rodata_end)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data_begin)
		*(.data)
		*(.data_end)
	}

	/* Read-write data (uninitialized) */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.bss_begin)
		*(.boot_stack)
		*(COMMON)
		*(.bss)
		*(.bss_end)
	}

	/* Any other sections produced by the compiler *should* be placed after this point */
}
