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
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

.extern run_checks
.extern start64
.extern setup_paging
.extern enable_paging

.global start
.global mb_header_magic
.global mb_header_ptr

.section .text.boot
	.code32

	// Kernel entry
	start:
		// Set the stack pointer
		mov $boot_stack_top, %esp

		// Preserve Multiboot things
		mov %eax, (mb_header_magic)
		mov %ebx, (mb_header_ptr)

		// Run initial checks
		call run_checks

		// Paging-related things
		call setup_paging
		call enable_paging

		// Enable GDT64
		call enable_gdt64

		// Jump to the long-mode entry (code segment at offset 8 into the GDT)
		jmp $8, $start64

	// Enable GDT64
	enable_gdt64:
		lgdt (gdt64_ptr)
		ret

.section .bss.boot
	// Multiboot things
	mb_header_magic:
		.long 0
	mb_header_ptr:
		.long 0

	// Stack
	.align 64
	boot_stack_bottom:
		.skip 1024 // Reserve a 1K stack
	boot_stack_top:

.section .rodata.boot
	// GDT
	gdt64:
		gdt64_start:
			.quad 0 // Zero entry
			.quad (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) // 64-bit Code segment (readable, code descriptor, present, 64-bit)
		gdt64_end:
		gdt64_ptr:
			.word (gdt64_end - gdt64_start - 1)
			.quad gdt64_start
