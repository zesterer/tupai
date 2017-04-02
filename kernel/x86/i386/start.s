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

.extern _init
.extern kentry
.extern run_checks

.global start

.section .text.boot
	// Kernel entry
	start:
		// Set the stack pointer
		mov $stack_top, %esp

		// Run initial checks
		call run_checks

		// Call global constructor code
		call _init

		// Call the kernel's entry point
		call kentry

		// Hang the kernel
		hang:
			hlt
			jmp hang

.section .bss.boot
	// Stack
	.align 64
	stack_bottom:
		.skip 256 // Reserve a 256-byte stack
	stack_top:
