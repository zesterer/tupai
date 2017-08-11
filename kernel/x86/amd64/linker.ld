ENTRY(start)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
	. = 1M;
	KERNEL_VIRTUAL = 0xFFFFFFFF80000000;
	MMAP_SIZE = 0x1000000;

	kernel_start = .;

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
		*(.init*)
		*(.fini*)

		*(.text)
		*(.gnu.linkonce.t*)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL)
	{
		*(.rodata)

		start_ctors = .;
		*(SORT(.ctors*))
		end_ctors = .;

		start_dtors = .;
		*(SORT(.dtors*))
		end_dtors = .;

		*(.gnu.linkonce.r*)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL)
	{
		*(.data)
		*(.gnu.linkonce.d*)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL)
	{
		*(COMMON)
		*(.bss)

		. = ALIGN(4K);
		start_mmap = .;
		. += MMAP_SIZE;
		end_mmap = .;

		*(.gnu.linkonce.b*)
	}

	. -= KERNEL_VIRTUAL;
	kernel_end = .;
}
