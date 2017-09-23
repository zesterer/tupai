//
//	file : isr.S
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
.global _isr_default_stub
.global _isr_pit_stub
.global _isr_kbd_stub
.global _isr_syscall_stub
.global _isr_spurious_stub
.global _in_irq

.section .text

	.macro BEGIN_IRQ // Begin IRQ
		push %eax
		push %ebx
		push %ecx
		push %edx
		push %esp
		push %ebp
		push %esi
		push %edi
		movl $1, (_in_irq)
		cld
	.endm

	.macro END_IRQ // End IRQ
		movl $0, (_in_irq)
		pop %edi
		pop %esi
		pop %ebp
		pop %esp
		pop %edx
		pop %ecx
		pop %ebx
		pop %eax
	.endm

	.macro END_IRQ_CALL // End IRQ (special syscall variant that preserves eax)
		movl $0, (_in_irq)
		pop %edi
		pop %esi
		pop %ebp
		pop %esp
		pop %edx
		pop %ecx
		pop %ebx
		add $4, %esp  // Pretend we popped eax
	.endm

	.align 4
	_isr_default_stub: // STUB IRQ (all)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		call isr_spurious
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	_isr_pit_stub: // PIT ISR (irq 0)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		//call pit_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	_isr_kbd_stub: // KBD ISR (irq 1)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		//call kbd_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	_isr_spurious_stub: // SPURIOUS ISR (irq 1)
		BEGIN_IRQ

		push %esp // Pass the current stack pointer
		call isr_spurious
		mov %eax, %esp // Restore the thread stack pointer

		END_IRQ
		iret

	.align 4
	_isr_syscall_stub: // SYSCALL ISR (irq 0x80)
		BEGIN_IRQ

		push $0 // Return status
		push %esp // Pointer to return status

		push %edi // Arg 3
		push %edx // Arg 2
		push %ecx // Arg 1
		push %ebx // Arg 0
		push %eax // Call ID

		// Find old stack pointer
		mov %esp, %eax
		add $(6 * 4), %eax // number of args * 4 bytes

		push %eax // Pass the old stack pointer
		//call syscall_isr_main
		mov %eax, %esp // Restore the thread stack pointer

		pop %eax // Pop the return status

		END_IRQ_CALL
		iret

.section .data
	.align 4
	_in_irq:
		.long 0
