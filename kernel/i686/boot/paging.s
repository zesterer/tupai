//
//	file : paging.s
//
// 	This file is part of Tupai.
//
//	Tupai is free software: you can redistribute it and/or modify
// 	it under the terms of the GNU General Public License as published by
// 	the Free Software Foundation, either version 3 of the License, or
// 	(at your option) any later version.
//
// 	Tupai is distributed in the hope that it will be useful,
// 	but WITHOUT ANY WARRANTY; without even the implied warranty of
// 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// 	GNU General Public License for more details.
//
// 	You should have received a copy of the GNU General Public License
// 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

.global _boot_setup_paging
.global _boot_load_page_directory
.global _boot_enable_paging
.global _boot_page_directory

.set PAGE_DIR_SIZE, 1024
.set PAGE_TABLE_SIZE, 1024
.set KERNEL_PAGE_TABLE_NUM, 256 // 16 MB of memory

.section .bss.boot

	.align 4096
	_boot_page_directory:
		.skip 1024 * 4 // 1024 x 32-bit integers
	.align 4096
	_boot_page_table_kernel:
		.skip KERNEL_PAGE_TABLE_NUM * PAGE_TABLE_SIZE * 4 // KERNEL_PAGE_NUM * PAGE_TABLE_SIZE x 32-bit integers

.section .text.boot

	_boot_setup_paging: // Setup paging ready for main kernel

		// Fill page directory with "not present"
		mov $0, %ecx
		_reloop0:
			mov %ecx, %edx
			imul $4, %edx
			add $_boot_page_directory, %edx

			movl $0x00000002, (%edx) // Zero the page dir and mark all page tables as not present

			add $1, %ecx
			cmp $PAGE_DIR_SIZE, %ecx // Count through PAGE_DIR_SIZE times
			jne _reloop0 // Loop back

		// Fill page tables with entries pointing to lower memory
		mov $0, %ecx
		_reloop1:
			mov %ecx, %eax
			imul $0x1000, %eax
			or $0x00000003, %eax // Present, R/W

			mov %ecx, %edx
			imul $4, %edx
			add $_boot_page_table_kernel, %edx

			movl %eax, (%edx)

			add $1, %ecx
			cmp $(PAGE_TABLE_SIZE * KERNEL_PAGE_TABLE_NUM), %ecx // Count through PAGE_DIR_SIZE * KERNEL_PAGE_TABLE_NUM times
			jne _reloop1 // Loop back

		// Fill higher and lower portions of page directory with page tables
		mov $0, %ecx
		_reloop2:
			mov %ecx, %eax
			imul $(4 * 1024), %eax
			add $_boot_page_table_kernel, %eax
			or $0x00000003, %eax // Present, R/W

			// Lower memory
			mov %ecx, %edx
			imul $4, %edx
			add $_boot_page_directory, %edx

			movl %eax, (%edx)

			// Upper memory
			mov %ecx, %edx
			add $768, %edx
			imul $4, %edx
			add $_boot_page_directory, %edx

			movl %eax, (%edx)

			add $1, %ecx
			cmp $(KERNEL_PAGE_TABLE_NUM), %ecx // Count through PAGE_DIR_SIZE * KERNEL_PAGE_TABLE_NUM times
			jne _reloop2 // Loop back

		call _boot_load_page_directory
		xchg %bx, %bx
		call _boot_enable_paging

		ret

	_boot_load_page_directory: // Load a page directory into the CR3 register
		push %ebp // Save the state of EBP
		mov %esp, %ebp // Save the state of ESP in EBP

		mov $_boot_page_directory, %eax // Find the function argument on the stack
		mov %eax, %cr3 // Move it into the CR3 register

		mov %ebp, %esp // Restore the state of ESP from EBP
		pop %ebp // Restore the state of EBP
		ret

	_boot_enable_paging: // Enable paging with the CR0 register
		push %ebp // Save the state of EBP
		mov %esp, %ebp // Save the state of ESP in EBP

		mov %cr0, %eax // Place the value of the CR0 register in EAX
		or $0x80000000, %eax // Enable the paging bit
		mov %eax, %cr0 // Place the new CR0 value back into CR0

		mov %ebp, %esp // Restore the state of ESP from EBP
		pop %ebp // Restore the state of EBP
		ret
