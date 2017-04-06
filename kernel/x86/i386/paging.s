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
// but WITHOUT ANY WARRANTY// without even the implied warranty of
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

	setup_paging:
		// Map P2 table entries
		mov $0, %ecx
		map_p2_loop:

			// Map the ECX-th P1 entry to a page at address 4K * ecx
			mov $(1024 * 4), %eax    // For each P1 table (size = 1024 * 4)
			imul %ecx, %eax         // Calculate 4K * ecx
			or $0b11, %eax          // Present and writable
			add $p1_table, %eax // Present, writable

			// Lower identity
			mov $4, %edx
			imul %ecx, %edx     // Calculate (p2_table + ecx * 8)...
			add $p2_table, %edx
			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Higher virtual
			mov $4, %edx
			imul %ecx, %edx     // Calculate (p2_table + ecx * 8)...
			add $p2_table, %edx
			add $(768 * 4), %edx
			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %ecx
			cmp $128, %ecx
			jne map_p2_loop

		// Map P1 table entries
		mov $0, %ecx
		map_p1_loop:
			// Map the ECX-th P1 entry to a page at address 4K * ecx
			mov $4096, %eax     // 4K
			imul %ecx, %eax     // Calculate 4K * ecx
			or $0b11, %eax      // Present, writable

			mov $4, %edx
			imul %ecx, %edx     // Calculate (p1_table + ecx * 8)...
			add $p1_table, %edx
			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %ecx
			cmp $512 * 128, %ecx
			jne map_p1_loop

		ret

	enable_paging:
		push %ebp // Save the state of EBP
		mov %esp, %ebp // Save the state of ESP in EBP

		mov $p2_table, %eax // Find the function argument on the stack
		mov %eax, %cr3 // Move it into the CR3 register

		mov %ebp, %esp // Restore the state of ESP from EBP
		pop %ebp // Restore the state of EBP

		push %ebp // Save the state of EBP
		mov %esp, %ebp // Save the state of ESP in EBP

		mov %cr0, %eax // Place the value of the CR0 register in EAX
		or $0x80000000, %eax // Enable the paging bit
		mov %eax, %cr0 // Place the new CR0 value back into CR0

		mov %ebp, %esp // Restore the state of ESP from EBP
		pop %ebp // Restore the state of EBP

		ret

.section .bss.boot
	.align 4096
	p2_table:
		.skip 1024 * 4
	p1_table:
		.skip 1024 * 4 * 128
