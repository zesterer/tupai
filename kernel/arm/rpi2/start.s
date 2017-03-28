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

.extern kmain

.global start

.section .text.boot

	// Entry point for the kernel.
	// r15 -> should begin execution at 0x8000
	// r0  -> 0x00000000
	// r1  -> 0x00000C42
	// r2  -> 0x00000100 - start of ATAGS
	start:
		// Setup the stack
		mov sp, #0x8000

		// Clear the BSS
		ldr r4, =__bss_start
		ldr r9, =__bss_end
		mov r5, #0
		mov r6, #0
		mov r7, #0
		mov r8, #0
		b 2f

		1:
			// Store multiple at r4
			stmia r4!, {r5 - r8}

			// If we are still below __bss_end, loop
		2:
			cmp r4, r9
			blo 1b

		// Enter kernel main
		ldr r3, =rpi2_kmain
		blx r3

	hang:
		wfe
		b hang
