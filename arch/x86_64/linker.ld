ENTRY(start)

SECTIONS
{
	. = 1M;

	.boot :
	{
		/* Place the Multiboot header at the beginning */
		*(.multiboot_header)
	}

	.text :
	{
		/* Executable code */
		*(.text)
	}

	.data :
	{
		/* Data */
		*(.data)
	}

	.bss :
	{
		/* BSS */
		*(.bss)
	}
}
