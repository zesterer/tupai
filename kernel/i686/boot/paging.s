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

.set PAGE_TABLE_SIZE, 1024
.set KERNEL_PAGE_TABLE_NUM, 4 // 16 MB of memory

.section .bss.boot

	.align 4096
	_boot_page_directory:
		.skip 1024 * 4 // 1024 x 32-bit integers
	.align 4096
	_boot_page_table_kernel_lower:
		.skip KERNEL_PAGE_TABLE_NUM * PAGE_TABLE_SIZE * 4 // KERNEL_PAGE_NUM * PAGE_TABLE_SIZE x 32-bit integers
	.align 4096
	_boot_page_table_kernel_upper:
		.skip KERNEL_PAGE_TABLE_NUM * PAGE_TABLE_SIZE * 4 // KERNEL_PAGE_NUM * PAGE_TABLE_SIZE x 32-bit integers

.section .text.boot

	_boot_setup_paging: // Setup paging ready for main kernel

		mov $0, %ecx
		_reloop1:
			mov %ecx, %edx
			add $_boot_page_directory, %edx
			movl $0x00000002, (%edx) // Zero the page dir and mark all page tables as not present

			_set_lower_kernel: // Dummy
				movl %ecx, %ebx // Find offset
				imul $0x1000, %ebx
				and $0xFFFFF000, %ebx
				or $0x003, %ebx

				mov %ecx, %edx
				imul $4, %edx
				add $_boot_page_table_kernel_lower, %edx
				movl %ebx, (%edx) // Zero the lower kernel page table and mark them as present

			_set_upper_kernel: // Dummy
				movl %ecx, %ebx // Find offset
				imul $0x1000, %ebx
				and $0xFFFFF000, %ebx
				or $0x003, %ebx

				mov %ecx, %edx
				imul $4, %edx
				add $_boot_page_table_kernel_upper, %edx
				movl %ebx, (%edx) // Zero the lower kernel page table and mark them as present

			add $1, %ecx
			cmp $(PAGE_TABLE_SIZE * KERNEL_PAGE_TABLE_NUM), %ecx // Count through PAGE_TABLE_SIZE * KERNEL_PAGE_TABLE_NUM times
			jne _reloop1 // Loop back

		mov $0, %ecx
		_reloop2:
			// Set the appropriate page directory entries
			mov %ecx, %eax
			imul $(4 * PAGE_TABLE_SIZE), %eax
			add $_boot_page_table_kernel_lower, %eax
			and $0xFFFFF000, %eax
			or $0x003, %eax
			mov %ecx, %edx
			imul $4, %edx
			add $(_boot_page_directory + 0x0), %edx
			mov %eax, (%edx)

			mov %ecx, %eax
			imul $(4 * PAGE_TABLE_SIZE), %eax
			add $_boot_page_table_kernel_upper, %eax
			and $0xFFFFF000, %eax
			or $0x003, %eax
			mov %ecx, %edx
			imul $4, %edx
			add $(_boot_page_directory + 3072), %edx
			mov %eax, (%edx)

			add $1, %ecx
			cmp $(KERNEL_PAGE_TABLE_NUM), %ecx // Count through KERNEL_PAGE_TABLE_NUM times
			jne _reloop2 // Loop back

		call _boot_load_page_directory
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
