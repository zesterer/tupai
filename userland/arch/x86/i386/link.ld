/*
// file : link.ld
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
*/

ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	HEAP_SIZE = 64M; /* Maximum heap size */

	. = 1M;

	.text ALIGN(4K)
	{
		*(.init*)
		*(.fini*)

		*(.text)
		*(.gnu.linkonce.t*)
	}

	.rodata ALIGN(4K)
	{
		*(.rodata)

		start_ctors = .;
		*(SORT(.ctors*))
		end_ctors = .;

		start_dtors = .;
		*(SORT(.dtors*))
		end_dtors = .;

		*(.gnu.linkonce.r*)
	}

	.data ALIGN(4K)
	{
		*(.data)
		*(.gnu.linkonce.d*)
	}

	.bss ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		*(.gnu.linkonce.b*)
	}

	.bss.noload ALIGN(4K) (NOLOAD)
	{
		*(.bss.noload)

		. = ALIGN(4K);
		heap_start = .;
		. += HEAP_SIZE;
		heap_end = .;
	}
}
