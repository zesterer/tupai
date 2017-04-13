//
// file : paging64.s
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
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
		mov %rax, (p4_table + 256 * 8)

		// Map 1st P3 entry to P2 table
		mov $p2_lo_table, %rax
		or $0b11, %rax // Present and writable
		mov %rax, (p3_lo_table)

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
			cmp $(128), %rcx
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
			cmp $(512 * 128), %rcx
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
		.skip 512 * 8 * 128 // Enough to cover 4K * 512 * 128 = 256M of memory
