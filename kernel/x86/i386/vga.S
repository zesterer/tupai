//
// file : vga.S
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

.global _vga_print.boot
.global _vga_move.boot

.global _vga_col_boot
.global _vga_row_boot

// Constants
.set vga_buffer, 0xB8000
.set vga_width, 80
.set vga_height, 25

.section .text.boot

	_vga_print.boot:
		push %ebx

		mov $0, %ecx
		1:
			// Should we exit the loop?
			mov 8(%esp), %eax
			add %ecx, %eax
			cmpb $0, (%eax)
			je 4f

			cmpb $'\n', (%eax)
			je 3f

			// Find VGA buffer position, store in EDX
			mov %ecx, %edx

			// Find character
			movb (%eax), %bl

			// Find current VGA position
			mov (_vga_row_boot), %edx
			imul $vga_width, %edx
			add (_vga_col_boot), %edx
			imul $2, %edx
			add $vga_buffer, %edx

			// Write character
			movb %bl, (%edx)
			movb 12(%esp), %bl // Color
			movb %bl, 1(%edx)

			// Increment VGA position
			mov (_vga_col_boot), %edx
			inc %edx
			mov %edx, (_vga_col_boot)
		2:

			// Iterate the loop
			add $1, %ecx
			jmp 1b
		3:
			movl $0, (_vga_col_boot)
			add $1, (_vga_row_boot)
			jmp 2b
		4:
			// Set cursor position
			push (_vga_col_boot)
			push (_vga_row_boot)
			call _vga_move.boot
			add $8, %esp

			pop %ebx
			ret

	_vga_move.boot:

		// Compute position
		mov 4(%esp), %eax
		imul $vga_width, %eax
		add 8(%esp), %eax
		push %eax

		mov $0x3D4, %dx
		mov $0x0F, %al
		outb %al, %dx

		mov $0x3D5, %dx
		movb (%esp), %al
		outb %al, %dx

		mov $0x3D4, %dx
		mov $0x0E, %al
		outb %al, %dx

		mov $0x3D5, %dx
		pop %eax
		shr $8, %eax
		outb %al, %dx

		ret

.section .data.boot
	// Different names, since must be accessed from C
	_vga_col_boot:
		.long 0
	_vga_row_boot:
		.long 0
