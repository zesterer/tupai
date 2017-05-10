//
// file : paging.s
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

.global enable_paging

.section .text.boot

	enable_paging:
		// Save return address
		push {lr}

		// Restore return address
		pop {lr}
		bx lr

.section .bss.boot:

	.align 12 // 2 ^ 12 = 4096
	// L1 page table
	p1_table:
		.skip 4096 * 4
	// L2 page table
	p2_table:
		.skip 1024 * 4
