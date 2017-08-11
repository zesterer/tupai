ENTRY(start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	. = 1M;
	KERNEL_VIRTUAL = 0xC0000000;

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
		*(.gnu.linkonce.b*)
	}

	. -= KERNEL_VIRTUAL;
	kernel_end = .;
}
