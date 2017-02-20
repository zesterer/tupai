/* Entry point of the operating system. The bootloader will begin execution at this label */
ENTRY(_boot_entry)

SECTIONS
{
	/* Offset the kernel at 1MB (to avoid interfering with the bootloader or any low-level initial code */
	. = 1M;

	/* Kernel virtual offset */
	KERNEL_VIRTUAL_OFFSET = 0xC0000000;

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

	. += KERNEL_VIRTUAL_OFFSET;

	kernel_start = .;

	/* Main kernel code */
	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL_OFFSET)
	{
		*(.text)
		*(.init)
		*(.fini)
	}

	/* Read-only data */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL_OFFSET)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL_OFFSET)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) */
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL_OFFSET)
	{
		*(.boot_stack)
		*(COMMON)
		*(.bss)
	}

	kernel_end = .;

	/* Any other sections produced by the compiler *should* be placed after this point */
}
