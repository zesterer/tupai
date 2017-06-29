//
// file : mb_header.s
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

// Multiboot 2 constants
.set MB_MAGIC, 0xE85250D6
.set MB_ARCH,  0
.set MB_SIZE,  (mb_end - mb_start)
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_ARCH + MB_SIZE))

.section .rodata.multiboot
	.align 4
	mb_start:
		.long MB_MAGIC
		.long MB_ARCH
		.long MB_SIZE
		.long MB_CHECKSUM

		// Optional Multiboot tags [none yet]
		.long

		.word 0 // Type
		.word 0 // Flags
		.long 8 // Size
	mb_end:
