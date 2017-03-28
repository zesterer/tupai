//
// file : multiboot.s
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

.section .rodata.multiboot
	.align 4
	mb_start:
		.long 0xE85250D6                  // Multiboot 2 magic number
		.long 0                           // Architecture 0 (protected mode i386)
		.long (mb_end - mb_start) // Header length

		// Checksum
		.long (0x100000000 - (0xE85250D6 + 0 + (mb_end - mb_start)))

		// Optional Multiboot tags [none yet]
		.long

		.word 0 // Type
		.word 0 // Flags
		.long 8 // Size
	mb_end:
