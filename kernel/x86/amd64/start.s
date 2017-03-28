//
// file : start.s
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

.extern panic32
.extern run_checks32
.extern start64

.global start

.section .text.boot
	.code32

	// Kernel entry
	start:
		// Set the stack pointer
		mov $stack_top, %esp

		// Run initial checks
		call run_checks32

		// Setup paging
		call setup_paging

		// Enable paging
		call enable_paging

		// Enable GDT64
		call enable_gdt64

		// Jump to the long-mode entry (code segment at offset 8 into the GDT)
		jmp $8, $start64

	// Setup paging
	setup_paging:
		// Map first P4 entry to P3 table
		mov $p3_table, %eax
		or $0b11, %eax // Present and writable
		mov %eax, (p4_table)

		// Map first P3 entry to P2 table
		mov $p2_table, %eax
		or $0b11, %eax // Present and writable
		mov %eax, (p3_table)

		// Map first P2 entry to P1 table
		mov $p1_table, %eax
		or $0b11, %eax // Present and writable
		mov %eax, (p2_table)

		// Map P1 table entries
		mov $0, %ecx
		map_p2_loop:
			// Map the ECX-th P2 entry to a page at address 4K * ecx
			mov $4096, %eax     // 4K
			imul %ecx, %eax     // Calculate 4K * ecx
			or $0b11, %eax      // Present, writable
			mov $8, %edx
			imul %ecx, %edx     // Calculate (p1_table + ecx * 8)...
			add $p1_table, %edx
			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %ecx
			cmp $512, %ecx
			jne map_p2_loop
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

	// Enable GDT64
	enable_gdt64:
		lgdt (gdt64_ptr)
		ret

.section .bss.boot
	// Stack
	.align 64
	stack_bottom:
		.skip 4096 // Reserve a 4K stack
	stack_top:

	// Page tables 'n stuff
	.align 4096
	p4_table:
		.skip 4096
	p3_table:
		.skip 4096
	p2_table:
		.skip 4096
	p1_table:
		.skip 4096

.section .rodata.boot
	// GDT
	gdt64:
		gdt64_start:
			.quad 0 // Zero entry
			.quad (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) // 64-bit Code segment (executable, code descriptor, present, 64-bit)
		gdt64_end:
		gdt64_ptr:
			.word (gdt64_end - gdt64_start - 1)
			.quad gdt64_start
