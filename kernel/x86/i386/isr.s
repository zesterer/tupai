//
//	file : isr.s
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
