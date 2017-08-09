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

	.macro PUSH_REGS // Preserve register states
		push %rax
		push %rbx
		push %rcx
		push %rdx
		push %rsi
		push %rdi
		push %rbp
		push %r8
		push %r9
		push %r10
		push %r11
		push %r12
		push %r13
		push %r14
		push %r15
	.endm

	.macro POP_REGS // Restore register states
		pop %r15
		pop %r14
		pop %r13
		pop %r12
		pop %r11
		pop %r10
		pop %r9
		pop %r8
		pop %rbp
		pop %rdi
		pop %rsi
		pop %rdx
		pop %rcx
		pop %rbx
		pop %rax
	.endm

	.macro BEGIN_IRQ // Begin IRQ
		PUSH_REGS
		movq $1, (in_irq)
		cld
	.endm

	.macro END_IRQ // End IRQ
		movq $0, (in_irq)
		POP_REGS
	.endm

	.align 4
	isr_stub: // STUB IRQ (all)
		BEGIN_IRQ

		mov %rsp, %rdi // Pass the current stack pointer
		call stub_isr_main
		mov %rax, %rsp // Restore the thread stack pointer

		END_IRQ
		iretq

	.align 4
	isr_pit: // PIT ISR (irq 0)
		BEGIN_IRQ

		mov %rsp, %rdi // Pass the current stack pointer
		call pit_isr_main
		mov %rax, %rsp // Restore the thread stack pointer

		END_IRQ
		iretq

	.align 4
	isr_kbd: // KBD ISR (irq 1)
		BEGIN_IRQ

		mov %rsp, %rdi // Pass the current stack pointer
		call kbd_isr_main
		mov %rax, %rsp // Restore the thread stack pointer

		END_IRQ
		iretq

	.align 4
	isr_spurious: // SPURIOUS ISR (irq 7)
		BEGIN_IRQ

		mov %rsp, %rdi // Pass the current stack pointer
		call spurious_isr_main
		mov %rax, %rsp // Restore the thread stack pointer

		END_IRQ
		iretq

	.align 4
	isr_syscall: // SYSCALL ISR (irq 0x80)
		BEGIN_IRQ

		push %rax // Preserve call ID
		push %rbx // Preserve arg 0
		push %rcx // Preserve arg 1
		push %rdx // Preserve arg 2

		pop %r8  // Arg 2
		pop %rcx // Arg 1
		pop %rdx // Arg 0
		pop %rsi // Call ID

		mov %rsp, %rdi // Pass the current stack pointer
		call syscall_isr_main
		mov %rax, %rsp // Restore the thread stack pointer

		END_IRQ
		iretq

.section .data
	.align 4
	in_irq:
		.quad 0
