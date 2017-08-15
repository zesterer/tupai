//
// file : start64.s
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

		call kentry // Call the kernel's entry point
		call _init // Call the global constructors
		call kmain  // Call kernel main procedure

	// Hang the kernel
	hang:
		hlt
		jmp hang

.section .bss
	// Stack
	.align 64
	stack_bottom:
		.skip 1024 * 64 // Reserve a 64K stack
	stack_top:
