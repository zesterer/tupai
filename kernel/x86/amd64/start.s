//
// file : start.s
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
