//
// file : start64.s
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

.extern _init
.extern kentry
.extern mb_header_magic
.extern mb_header_ptr
.extern setup_paging64
.extern enable_paging64

.global start64

.section .text.boot
	.code64

	// 64-bit kernel entry
	start64:
		// Clear the data segment registers to the null segment descriptor
		mov $0, %ax
		mov %ax, %ss
		mov %ax, %ds
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs

		// Setup 64-bit paging
		call setup_paging64
		call enable_paging64

		// Jump to the higher-half entry
		movabs $start64_higher, %rax
		jmp *%rax

.section .text
	.code64

	// 64-bit higher-half kernel entry
	start64_higher:
		// Set the stack pointer
		movabs $stack_top, %rsp

		// Call global constructor code
		call _init

		// Set up kentry arguments
		mov %rsp, %rdx // Stack
		mov (mb_header_ptr), %esi // Multiboot info
		mov (mb_header_magic), %edi // Multiboot magic

		// Call the kernel's entry point
		call kentry

	// Hang the kernel
	hang:
		hlt
		jmp hang

.section .bss
	// Stack
	.align 64
	stack_bottom:
		.skip 4096 // Reserve a 4K stack
	stack_top:
