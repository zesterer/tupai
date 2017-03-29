//
// file : start64.s
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

.extern kmain

.global start64

.section .text.boot
	.code64

	// Kernel entry
	start64:
		// Clear the data segment registers to the null segment descriptor
		mov $0, %ax
		mov %ax, %ss
		mov %ax, %ds
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs

		// Call the kernel's entry point
		call x86_amd64_kentry

	// Hang the kernel
	hang:
		hlt
		jmp hang
