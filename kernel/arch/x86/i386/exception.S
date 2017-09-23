//
// file : exception.S
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

// Explanation:
// Some ISRs push error bytes onto the stack. So normalise all ISRs, we push a
// fake error byte onto the stack to maintain a uniform stack. We can then act
// as if all ISRs push error bytes

// Globally define the ISRs
.global _exception_0
.global _exception_1
.global _exception_2
.global _exception_3
.global _exception_4
.global _exception_5
.global _exception_6
.global _exception_7
.global _exception_8
.global _exception_9
.global _exception_10
.global _exception_11
.global _exception_12
.global _exception_13
.global _exception_14
.global _exception_15
.global _exception_16
.global _exception_17
.global _exception_18
.global _exception_19
.global _exception_20
.global _exception_21
.global _exception_22
.global _exception_23
.global _exception_24
.global _exception_25
.global _exception_26
.global _exception_27
.global _exception_28
.global _exception_29
.global _exception_30
.global _exception_31

.set ISR_DUMMY_ERROR, 0

.section .text
	.align 4
	_exception_0: // Division By Zero Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $0 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_1: // Debug Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $1 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_2: // Non-Maskable Interrupt Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $2 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_3: // Breakpoint Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $3 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_4: // Into Detected Overflow Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $4 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_5: // Out Of Bounds Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $5 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_6: // Invalid Opcode Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $6 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_7: // No Coprocessor Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $7 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_8: // Double Fault Exception
		push $8 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_9: // Coprocessor Segment Overrun Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $9 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_10: // Bad TSS Exception
		push $10 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_11: // Segment Not Present Exception
		push $11 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_12: // Stack Fault Exception
		push $12 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_13: // General Protection Fault Exception
		push $13 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_14: // Page Fault Exception
		push $14 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_15: // Unknown Interrupt Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $15 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_16: // Coprocessor Fault Exception
		push $ISR_DUMMY_ERROR // Dummy error
		push $16 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_17: // Alignment Check Exception (486+)
		push $ISR_DUMMY_ERROR // Dummy error
		push $17 // ISR identifer
		jmp _exception_common

	.align 4
	_exception_18: // Machine Check Exception (Pentium/586+)
		push $ISR_DUMMY_ERROR // Dummy error
		push $18 // ISR identifer
		jmp _exception_common

	/* Please note: ISRs 19-31 are reserved and will be implemented later! */

	_exception_common: // Common ISR routine (must be jumped to by an ISR defined above)
		push %eax // Preserve EAX - we need it to move things around
		mov 8(%esp), %eax
		mov %eax, (tmp_exception_err) // Preserve exception error
		mov 4(%esp), %eax
		mov %eax, (tmp_exception_code) // Preserve exception code
		pop %eax // Restore EAX

		add $8, %esp
		pushal // Preserve registers
		movl $1, (_in_irq) // We're now in an IRQ!

		mov %esp, %eax // Preserve ESP
		push (tmp_exception_err) // Pass the exception error
		push (tmp_exception_code) // Pass the exception code
		push %eax // Pass the stack pointer (see above)
		cld
		call exception_handler
		mov %eax, %esp // Restore the thread stack pointer

		movl $0, (_in_irq) // No longer in an IRQ
		popal // Restore registers
		iret

.section .bss
	tmp_exception_err:
		.long 0
	tmp_exception_code:
		.long 0

.section .rodata
	exception_error:
		.ascii "Exception occured!"
