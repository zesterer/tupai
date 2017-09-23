//
// file : start.S
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

.global _start.boot
.global _hang.boot

.global _stack_bottom.boot
.global _stack_top.boot

.global _mb_magic.boot
.global _mb_struct.boot

.section .text.boot

	_start.boot: // Kernel first entry point

		mov $_stack_top.boot, %esp // Set up initial stack

		mov %eax, (_mb_magic.boot) // Preserve Multiboot magic value
		mov %ebx, (_mb_struct.boot) // Preserve Multiboot structure

		// Display early boot message
		push $0x0F
		push $_start_str
		call _vga_print.boot
		add $8, %esp

		call _check.boot // Perform initial CPU compatibility checks
		call _info.boot // Display CPU info
		call _paging_init.boot // Construct page tables
		call _paging_enable.boot

		// Display message before jumping to virtual kernel
		push $0x0F
		push $_virt_jump_str
		call _vga_print.boot
		add $8, %esp

		// Jump to the higher half
		jmp _start_virt

		// If, by some ungodly miracle, the previous code returns, then hang the CPU
		jmp _hang.boot

	_hang.boot:
		hlt
		jmp _hang.boot

.section .bss.boot
	.align 16
	_stack_bottom.boot:
		.skip 1024
	_stack_top.boot:

	_mb_magic.boot:
		.skip 4
	_mb_struct.boot:
		.skip 4

.section .rodata.boot
	_start_str:
		.ascii "*** Tupai boot started ***\n\0"
	_virt_jump_str:
		.ascii "[ OK ] Reached virtual kernel jump\n\0"
