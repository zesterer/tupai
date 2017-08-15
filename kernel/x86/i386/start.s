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

		// Set up kentry arguments
		push %esp // Stack
		mov (mb_header_ptr), %eax
		push %eax // Multiboot info
		mov (mb_header_magic), %eax
		push %eax // Multiboot magic

		call kentry // Call the kernel's entry point
		call _init // Call the global constructors
		call kmain  // Call kernel main procedure

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
