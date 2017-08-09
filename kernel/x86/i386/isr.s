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
.global isr_pit
.global isr_kbd
.global isr_syscall
.global isr_spurious
.global in_irq

.section .text

	.macro BEGIN_IRQ // Begin IRQ
		pushal
		movl $1, (in_irq)
		cld
	.endm

	.macro END_IRQ // End IRQ
		movl $0, (in_irq)
		popal
	.endm

	.align 4
	isr_stub: // STUB IRQ (all)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		call stub_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	isr_pit: // PIT ISR (irq 0)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		call pit_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	isr_kbd: // KBD ISR (irq 1)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		call kbd_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	isr_spurious: // SPURIOUS ISR (irq 1)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		call spurious_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	isr_syscall: // SYSCALL ISR (irq 0x80)
		BEGIN_IRQ

		push %edx // Arg 2
		push %ecx // Arg 1
		push %ebx // Arg 0
		push %eax // Call ID

		// Find old stack pointer
		mov %esp, %eax
		add $16, %eax

		push %eax // Pass the old stack pointer
		call syscall_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

.section .data
	.align 4
	in_irq:
		.long 0
