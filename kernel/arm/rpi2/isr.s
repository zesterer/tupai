//
//	file : isr.s
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

// Globally define the ISRs
.global isr_stub
.global isr_syscall

.section .text

	.align 4
	isr_stub: // STUB IRQ (all)

		//mov %rsp, %rdi // Pass the current stack pointer
		//call stub_isr_main
		//mov %rax, %rsp // Restore the thread stack pointer

		bx lr

	.align 4
	isr_syscall: // SYSCALL ISR (irq 0x80)

		//mov %rsp, %rdi // Pass the current stack pointer
		//call syscall_isr_main
		//mov %rax, %rsp // Restore the thread stack pointer

		bx lr
