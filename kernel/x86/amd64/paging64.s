//
// file : paging64.s
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

.global setup_paging64
.global enable_paging64

.section .text.boot
	.code64

	// Setup paging
	setup_paging64:

		// Map 1st P4 entry to P3 table
		mov $p3_lo_table, %rax
		or $0b11, %rax // Present and writable
		mov %rax, (p4_table)

		// Map 256th P4 entry to P3 table (virtual higher mapping)
		mov $p3_lo_table, %rax
		or $0b11, %rax // Present and writable
		mov %rax, (p4_table + 511 * 8)

		// Map 1st P3 entry to P2 table
		mov $p2_lo_table, %rax
		or $0b11, %rax // Present and writable
		mov %rax, (p3_lo_table)

		// Map 511th P3 entry to P2 table (virtual higher mapping)
		mov $p2_lo_table, %rax
		or $0b11, %rax // Present and writable
		mov %rax, (p3_lo_table + 510 * 8)

		// Map P2 table entries
		mov $0, %rcx
		map_p2_lo_loop:
			// Map the rcx-th P1 entry to a page at address 4K * rcx
			mov $(512 * 8), %rax    // For each P1 table (size = 512 * 8)
			imul %rcx, %rax         // Calculate 4K * rcx
			or $0b11, %rax          // Present and writable
			add $p1_lo_tables, %rax // Present, writable

			mov $8, %rdx
			imul %rcx, %rdx     // Calculate (p2_table + rcx * 8)...
			add $p2_lo_table, %rdx

			mov %rax, (%rdx)    // Map rcx-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %rcx
			cmp $256, %rcx
			jne map_p2_lo_loop

		// Map P1 table entries
		mov $0, %rcx
		map_p1_lo_loop:
			// Map the rcx-th P1 entry to a page at address 4K * rcx
			mov $4096, %rax     // 4K
			imul %rcx, %rax     // Calculate 4K * rcx
			or $0b11, %rax      // Present, writable

			mov $8, %rdx
			imul %rcx, %rdx     // Calculate (p1_table + rcx * 8)...
			add $p1_lo_tables, %rdx

			mov %rax, (%rdx)    // Map rcx-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %rcx
			cmp $(512 * 256), %rcx
			jne map_p1_lo_loop

		ret

	// Enable paging
	enable_paging64:
		// Load P4 table to CR3 (so the CPU knows where to look for paging info)
		mov $p4_table, %rax
		mov %rax, %cr3

		// We've enabled 64-bit paging
		ret

.section .bss.boot
	// Page tables 'n stuff
	.align 4096
	p4_table:
		.skip 512 * 8
	p3_lo_table:
		.skip 512 * 8
	p2_lo_table:
		.skip 512 * 8
	p1_lo_tables:
		.skip 512 * 8 * 256 // Enough to cover 4K * 512 * 256 = 512M of memory
