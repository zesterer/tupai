//
//	file : gdt.s
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

.section .data
	_gdt_ptr:
		.word // Limit
		.long // Base

.section .text
	.global gdt_install
	//gdt_install:
		mov 4(%esp), %eax
		mov %eax, (_gdt_ptr + 2)
		mov 8(%esp), %ax
		mov %ax, (_gdt_ptr + 0)
		lgdt _gdt_ptr
		ret
