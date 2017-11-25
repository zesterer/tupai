//
// file : paging.S
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

.global _paging_init.boot
.global _paging_enable.boot

.section .text.boot

	_paging_init.boot: // Initiate paging

		// Clear page tables first
		mov $_p2_table.boot, %ecx
		1:
			movl $0, (%ecx)
			add $4, %ecx
			cmp $_table_end.boot, %ecx
			jne 1b

		// Map P2 table entries
		mov $0, %ecx
		1:
			// Map the ECX-th P1 entry to a page at address 4K * ecx
			mov $(1024 * 4), %eax    // For each P1 table (size = 1024 * 4)
			imul %ecx, %eax         // Calculate 4K * ecx
			or $0b11, %eax          // Present and writable
			add $_p1_table.boot, %eax // Present, writable

			// Lower identity
			mov $4, %edx
			imul %ecx, %edx     // Calculate (p2_table + ecx * 8)...
			add $_p2_table.boot, %edx
			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Higher virtual
			mov $4, %edx
			imul %ecx, %edx     // Calculate (p2_table + ecx * 8)...
			add $_p2_table.boot, %edx
			add $((1024 - 256) * 4), %edx // 3G offset
			mov %eax, (%edx)    // Map ECX-th entry (each entry is 8 bytes)...

			// Iterate the loop
			inc %ecx
			cmp $128, %ecx
			jne 1b

		// Map P1 table entries
		mov $0, %ecx
		2:
			// Map the ECX-th P1 entry to a page at address 4K * ecx
			mov $4096, %eax     // 4K
			imul %ecx, %eax     // Calculate 4K * ecx
			or $0b11, %eax      // Present, writable

			mov $4, %edx
			imul %ecx, %edx     // Calculate (p1_table + ecx * 4)...
			add $_p1_table.boot, %edx
			mov %eax, (%edx)    // Map ECX-th entry (each entry is 4 bytes)...

			// Iterate the loop
			inc %ecx
			cmp $512 * 128, %ecx
			jne 2b

		push $0x0F
		push $_paging_init_str
		call _vga_print.boot
		add $8, %esp

		ret

	_paging_enable.boot:
		push %ebp // Save the state of EBP
		mov %esp, %ebp // Save the state of ESP in EBP

		mov $_p2_table.boot, %eax // Find the function argument on the stack
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

		push $0x0F
		push $_paging_enabled_str
		call _vga_print.boot
		add $8, %esp

		ret

.section .bss.boot
	.align 4096
	_p2_table.boot:
		.skip 4 * 1024
	_p1_table.boot:
		.skip 4 * 1024 * 128 // 4K * 1024 * 128 = 512M of memory
	_table_end.boot:

.section .rodata.boot
	_paging_init_str:
		.ascii "[ OK ] Early page maps constructed\n\0"
	_paging_enabled_str:
		.ascii "[ OK ] Paging enabled, kernel mapped to high memory\n\0"
