//
// file : mb.S
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

// Multiboot 2 constants
.set MB_MAGIC, 0xE85250D6
.set MB_ARCH,  0
.set MB_SIZE,  (mb_end - mb_start)
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_ARCH + MB_SIZE))

.section .rodata.multiboot
	.align 4
	mb_start:
		.long MB_MAGIC
		.long MB_ARCH
		.long MB_SIZE
		.long MB_CHECKSUM

		// Optional Multiboot tags [none yet]
		.long

		.word 0 // Type
		.word 0 // Flags
		.long 8 // Size
	mb_end:
