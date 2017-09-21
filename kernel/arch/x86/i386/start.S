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

.section .text.boot

	_start.boot: // Kernel first entry point

		mov $_stack_top.boot, %esp // Set up initial stack

		// TODO : Preserve Multiboot things

		// TODO : Run CPU compatibility checks

		// TODO : Enable paging

		mov $_hello.boot, %eax
		call _print.boot

		_hang.boot:
			hlt
			jmp _hang.boot

	_print.boot:
		mov $0, %ecx
		strloop:
			// Find current character, store in BL
			mov %ecx, %ebx
			imul $1, %ebx
			add %eax, %ebx
			mov (%ebx), %bl

			// Should we exit the loop?
			cmp $0, %bl
			je strend

			// Find the VGA buffer position, store in EDX
			mov %ecx, %edx
			imul $2, %edx
			add $0xB8000, %edx

			// Print the character
			movb %bl, (%edx)
			inc %edx
			// Give it color
			movb $0x4F, (%edx)

			// Iterate the loop
			add $1, %ecx
			jmp strloop
		strend:
		ret

.section .bss.boot
	.align 64
	_stack_bottom.boot:
		.skip 1024
	_stack_top.boot:

.section .rodata.boot
	_hello.boot:
		.ascii "Hello, World!\0"
