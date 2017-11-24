//
// file : higher.S
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

.global _start_virt
.global _hang

.global _stack_bottom
.global _stack_top

.set virt_offset, 0xC0000000

.section .text

	_start_virt: // Kernel virtual entry
		mov $(_stack_bottom.boot + virt_offset), %esp // Reset stack, now with virtual offset

		// Display higher boot message
		push $0x0F
		push $_start_str
		call _vga_print.boot
		add $8, %esp

		// Call early kernel setup (heap, virtual memory, etc.)
		call kearly

		// Call global constructors
		call _init

		// Call the platform-specific kentry function
		push (_mb_struct.boot)
		call kentry

		jmp _hang

	_hang:
		hlt
		jmp _hang

.section .rodata
	_start_str:
		.ascii "[ OK ] Jump successful, welcome to the virtual kernel\n\0"
