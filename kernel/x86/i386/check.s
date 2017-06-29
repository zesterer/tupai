//
// file : check32.s
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

.global run_checks

.section .text.boot
	.code32

	run_checks: // Run a set of initial checks on the system
		// No checks yet
		ret

	panic: // Display an ASCII error string
		call printstr
		hlt

	printstr:
		mov $0, %ecx
		strloop:
			// Find current character, store in BL
			mov %ecx, %ebx
			imul $1, %ebx
			add %eax, %ebx
			mov (%ebx), %bl

			// Should we exit the loop?
			cmp $0, %bl
			je strend

			// Find the VGA buffer position, store in EDX
			mov %ecx, %edx
			imul $2, %edx
			add $0xB8000, %edx

			// Print the character
			movb %bl, (%edx)
			inc %edx
			// Give it color
			movb $0x4F, (%edx)

			// Iterate the loop
			add $1, %ecx
			jmp strloop
		strend:
		ret

.section .rodata.boot
	// Error messages here
