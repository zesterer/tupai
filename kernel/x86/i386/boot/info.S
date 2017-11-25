//
// file : info.S
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

.global _info.boot

.section .text.boot

	_info.boot: // Check CPU compatibility

		call _info_vendor

		ret

	_info_vendor:

		push $0x0F
		push $_info_vendor_start
		call _vga_print.boot
		add $8, %esp

		mov $0, %eax
		cpuid

		push $0
		push %ecx
		push %edx
		push %ebx

		mov %esp, %eax

		push $0x0F
		push %eax
		call _vga_print.boot
		add $8, %esp

		add $16, %esp

		push $0x0F
		push $_info_vendor_end
		call _vga_print.boot
		add $8, %esp

		ret


.section .rodata.boot
	_info_vendor_start:
		.ascii "[ OK ] CPU vendor is '\0"
	_info_vendor_end:
		.ascii "'\n\0"
