//
// file : paging.s
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

.global setup_paging
.global enable_paging

.section .text.boot
	.code32

	// Setup paging
	setup_paging:

		// Map 1st P4 entry to P3 table
		mov $p3_lo_table, %eax
		or $0b11, %eax // Present and writable
		mov %eax, (p4_table)

		// Map 1st P3 entry to P2 table
		mov $p2_lo_table, %eax
		or $0b11, %eax // Present and writable
		mov %eax, (p3_lo_table)

		// Map P2 table entries
		mov $0, %ecx
		map_p2_lo_loop:
			// Map the ECX-th P1 entry to a page at address 4K * ecx
			mov $(512 * 8), %eax    // For each P1 table (size = 512 * 8)
			imul %ecx, %eax         // Calculate 4K * ecx
			or $0b11, %eax          // Present and writable
			add $p1_lo_tables, %eax // Present, writable

			mov $8, %edx
			imul %ecx, %edx     // Calculate (p2_table + ecx * 8)...
			add $p2_lo_table, %edx

			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %ecx
			cmp $(128), %ecx
			jne map_p2_lo_loop

		// Map P1 table entries
		mov $0, %ecx
		map_p1_lo_loop:
			// Map the ECX-th P1 entry to a page at address 4K * ecx
			mov $4096, %eax     // 4K
			imul %ecx, %eax     // Calculate 4K * ecx
			or $0b11, %eax      // Present, writable

			mov $8, %edx
			imul %ecx, %edx     // Calculate (p1_table + ecx * 8)...
			add $p1_lo_tables, %edx

			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %ecx
			cmp $(512 * 128), %ecx
			jne map_p1_lo_loop

		ret

	// Enable paging
	enable_paging:
		// Load P4 table to CR3 (so the CPU knows where to look for paging info)
		mov $p4_table, %eax
		mov %eax, %cr3

		// Enable PAE flag in CR4 (Physical Address Extension)
		mov %cr4, %eax
		or $(1 << 5), %eax // (6th bit)
		mov %eax, %cr4

		// Set the long-mode bit in the EFER MSR (Model-Specific Register)
		mov $0xC0000080, %ecx
		rdmsr              // Load MSR into EAX
		or $(1 << 8), %eax // (9th bit)
		wrmsr              // Load EAX back into MSR

		// Enable paging in CR0
		mov %cr0, %eax
		or $(1 << 31), %eax // (32nd bit)
		mov %eax, %cr0

		// We've enabled paging (we're still in a 32-bit compatibility mode though)
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
