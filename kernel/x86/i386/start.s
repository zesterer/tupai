//
// file : start.s
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

.extern _init
.extern kentry
.extern run_checks
.extern setup_paging
.extern enable_paging

.global start

.section .text.boot
	// Kernel entry
	start:
		// Set the stack pointer
		mov $boot_stack_top, %esp

		// Preserve Multiboot things
		mov %eax, (mb_header_magic)
		mov %ebx, (mb_header_ptr)

		// Run initial checks
		call run_checks

		// Paging
		call setup_paging
		call enable_paging

		jmp start_higher

.section .text
	start_higher:
		// Set the stack pointer
		mov $stack_top, %esp

		// Call global constructor code
		call _init

		// Set up kentry arguments
		push %esp // Stack
		mov (mb_header_ptr), %eax
		push %eax // Multiboot info
		mov (mb_header_magic), %eax
		push %eax // Multiboot magic

		// Call the kernel's entry point
		call kentry

		// Hang the kernel
		hang:
			hlt
			jmp hang

.section .bss.boot
	// Multiboot things
	mb_header_magic:
		.long 0
	mb_header_ptr:
		.long 0

	// Boot stack
	.align 64
	boot_stack_bottom:
		.skip 256 // A 256-byte boot stack
	boot_stack_top:

.section .bss
	// Stack
	stack_bottom:
		.skip 1024 * 64 // A 64K stack
	stack_top:
