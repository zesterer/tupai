//
// file : crt.s
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

.section .init
	.global _init
	.type _init, @function
	_init:
		push %ebp
		movl %esp, %ebp
		// GCC will nicely put the contents of crtbegin.o's init section here.

.section .fini
	.global _fini
	.type _fini, @function
	_fini:
		push %ebp
		movl %esp, %ebp
		// GCC will nicely put the contents of crtbegin.o's fini section here.

.section .init
	// GCC should place crtend.o init code here
	popl %ebp
	ret

.section .fini
	// GCC should place crtend.o fini code here
	popl %ebp
	ret
