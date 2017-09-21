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

ENTRY(_start.boot)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	. = 1M;
	KERNEL_VIRTUAL = 0xC0000000;
	MMAP_SIZE = 0x1000000;

	kernel_start = .;

	/* Place the Multiboot header at the beginning */
	.rodata.multiboot BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata.multiboot)
	}

	.text.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.text.boot)
	}

	.data.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata.boot)
		*(.data.boot)
	}

	.bss.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.bss.boot)
	}

	. += KERNEL_VIRTUAL;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL)
	{
		*(.init*)
		*(.fini*)

		*(.text)
		*(.gnu.linkonce.t*)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL)
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

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL)
	{
		*(.data)
		*(.gnu.linkonce.d*)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL)
	{
		*(COMMON)
		*(.bss)
		*(.gnu.linkonce.b*)

		. = ALIGN(4K);
		start_mmap = .;
		. += MMAP_SIZE;
		end_mmap = .;
	}

	.bss.noload ALIGN(4K) (NOLOAD) : AT(ADDR(.bss.noload) - KERNEL_VIRTUAL)
	{
		*(.bss.noload)

		. = ALIGN(4K);
		start_mmap = .;
		. += MMAP_SIZE;
		end_mmap = .;
	}

	. -= KERNEL_VIRTUAL;
	kernel_end = .;
}
