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
.global isr_0
.global isr_1
.global isr_2
.global isr_3
.global isr_4
.global isr_5
.global isr_6
.global isr_7
.global isr_8
.global isr_9
.global isr_10
.global isr_11
.global isr_12
.global isr_13
.global isr_14
.global isr_15
.global isr_16
.global isr_17
.global isr_18
.global isr_19
.global isr_20
.global isr_21
.global isr_22
.global isr_23
.global isr_24
.global isr_25
.global isr_26
.global isr_27
.global isr_28
.global isr_29
.global isr_30
.global isr_31

.set ISR_DUMMY_ERROR, 0

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

	.align 4
	isr_0: // Division By Zero Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $0 // ISR identifer
		jmp isr_common

	.align 4
	isr_1: // Debug Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $1 // ISR identifer
		jmp isr_common

	.align 4
	isr_2: // Non-Maskable Interrupt Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $2 // ISR identifer
		jmp isr_common

	.align 4
	isr_3: // Breakpoint Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $3 // ISR identifer
		jmp isr_common

	.align 4
	isr_4: // Into Detected Overflow Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $4 // ISR identifer
		jmp isr_common

	.align 4
	isr_5: // Out Of Bounds Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $5 // ISR identifer
		jmp isr_common

	.align 4
	isr_6: // Invalid Opcode Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $6 // ISR identifer
		jmp isr_common

	.align 4
	isr_7: // No Coprocessor Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $7 // ISR identifer
		jmp isr_common

	.align 4
	isr_8: // Double Fault Exception
		push $8 // ISR identifer
		jmp isr_common

	.align 4
	isr_9: // Coprocessor Segment Overrun Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $9 // ISR identifer
		jmp isr_common

	.align 4
	isr_10: // Bad TSS Exception
		push $10 // ISR identifer
		jmp isr_common

	.align 4
	isr_11: // Segment Not Present Exception
		push $11 // ISR identifer
		jmp isr_common

	.align 4
	isr_12: // Stack Fault Exception
		push $12 // ISR identifer
		jmp isr_common

	.align 4
	isr_13: // General Protection Fault Exception
		push $13 // ISR identifer
		jmp isr_common

	.align 4
	isr_14: // Page Fault Exception
		push $14 // ISR identifer
		jmp isr_common

	.align 4
	isr_15: // Unknown Interrupt Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $15 // ISR identifer
		jmp isr_common

	.align 4
	isr_16: // Coprocessor Fault Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $16 // ISR identifer
		jmp isr_common

	.align 4
	isr_17: // Alignment Check Exception (486+)
		push $ISR_DUMMY_ERROR // Dummy error
		push $17 // ISR identifer
		jmp isr_common

	.align 4
	isr_18: // Machine Check Exception (Pentium/586+)
		push $ISR_DUMMY_ERROR // Dummy error
		push $18 // ISR identifer
		jmp isr_common

	/* Please note: ISRs 19-31 are reserved and will be implemented later! */

	isr_common: // Common ISR routine (must be jumped to by an ISR defined above)
		push %rax // Preserve RAX - we need it to move things around
		mov 16(%rsp), %rax
		mov %rax, (tmp_exception_err) // Preserve exception error
		mov 8(%rsp), %rax
		mov %rax, (tmp_exception_code) // Preserve exception code
		pop %rax // Restore RAX

		add $16, %rsp
		PUSH_REGS // Preserve registers
		movq $1, (in_irq) // We're now in an IRQ!

		mov (tmp_exception_err), %rdx // Pass the exception error
		mov (tmp_exception_code), %rsi // Pass the exception code
		mov %rsp, %rdi // Pass the current stack pointer
		cld
		call exception_handle
		mov %rax, %rsp // Restore the thread stack pointer

		movq $0, (in_irq) // No longer in an IRQ
		POP_REGS // Restore registers
		iretq

.section .bss
	tmp_exception_err:
		.quad
	tmp_exception_code:
		.quad

.section .rodata
	exception_error:
		.ascii "Exception occured!"
