# Plan Of Action

What needs to be done, in order of priority:

- Page frame allocator
- Virtual memory manager
- Virtual memory context switcher
- Improve syscall API
- Create an elf loader for userland programs & modules
- Fix x86 keyboard driver
- Fix TTY handling things
- Create a TSS and handle use-space code

## Page Frame Allocator

### Necessary Functions

page_flags
{
	is_static
}

page_entry
{
	owner
	flags
}

find_page()
test_page(address)
reserve_page(address)
