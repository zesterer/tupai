//
//	file : multiboot.s
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

// Multiboot constants
.set MB_ALIGN,    1 << 0
.set MB_MEMINFO,  1 << 1
.set MB_FLAGS,    MB_ALIGN | MB_MEMINFO
.set MB_MAGIC,    0x1BADB002
.set MB_CHECKSUM, -(MB_MAGIC + MB_FLAGS)

// The Multiboot-compliant header
.section .rodata.multiboot
	.global _multiboot
	_multiboot:
		.align 4
		.long MB_MAGIC
		.long MB_FLAGS
		.long MB_CHECKSUM

		.long _multiboot
		.long 0
		.long 0
		.long 0
