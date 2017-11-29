//
// file : fault.S
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
// Some ISRs push error bytes onto the stack. To normalise all ISRs, we push a
// fake error byte onto the stack to maintain a uniform stack. We can then act
// as if all ISRs push error bytes

// Globally define the ISRs
.global _fault_0
.global _fault_1
.global _fault_2
.global _fault_3
.global _fault_4
.global _fault_5
.global _fault_6
.global _fault_7
.global _fault_8
.global _fault_9
.global _fault_10
.global _fault_11
.global _fault_12
.global _fault_13
.global _fault_14
.global _fault_15
.global _fault_16
.global _fault_17
.global _fault_18
.global _fault_19
.global _fault_20
.global _fault_21
.global _fault_22
.global _fault_23
.global _fault_24
.global _fault_25
.global _fault_26
.global _fault_27
.global _fault_28
.global _fault_29
.global _fault_30
.global _fault_31

.set ISR_DUMMY_ERROR, 0

.section .text
	.align 4
	_fault_0: // Division By Zero Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $0 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_1: // Debug Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $1 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_2: // Non-Maskable Interrupt Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $2 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_3: // Breakpoint Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $3 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_4: // Into Detected Overflow Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $4 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_5: // Out Of Bounds Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $5 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_6: // Invalid Opcode Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $6 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_7: // No Coprocessor Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $7 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_8: // Double Fault Fault
		push $8 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_9: // Coprocessor Segment Overrun Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $9 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_10: // Bad TSS Fault
		push $10 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_11: // Segment Not Present Fault
		push $11 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_12: // Stack Fault Fault
		push $12 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_13: // General Protection Fault Fault
		push $13 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_14: // Page Fault Fault
		push $14 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_15: // Unknown Interrupt Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $15 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_16: // Coprocessor Fault Fault
		push $ISR_DUMMY_ERROR // Dummy error
		push $16 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_17: // Alignment Check Fault (486+)
		push $ISR_DUMMY_ERROR // Dummy error
		push $17 // ISR identifer
		jmp _fault_common

	.align 4
	_fault_18: // Machine Check Fault (Pentium/586+)
		push $ISR_DUMMY_ERROR // Dummy error
		push $18 // ISR identifer
		jmp _fault_common

	/* Please note: ISRs 19-31 are reserved and will be implemented later! */

	_fault_common: // Common ISR routine (must be jumped to by an ISR defined above)
		push %eax // Preserve EAX - we need it to move things around
		mov 8(%esp), %eax
		mov %eax, (tmp_fault_err) // Preserve fault error
		mov 4(%esp), %eax
		mov %eax, (tmp_fault_code) // Preserve fault code
		pop %eax // Restore EAX

		add $8, %esp
		pushal // Preserve registers
		movl $1, (_in_irq) // We're now in an IRQ!

		mov %esp, %eax // Preserve ESP
		push (tmp_fault_err) // Pass the fault error
		push (tmp_fault_code) // Pass the fault code
		push %eax // Pass the stack pointer (see above)
		cld
		call fault_handler
		mov %eax, %esp // Restore the thread stack pointer

		movl $0, (_in_irq) // No longer in an IRQ
		popal // Restore registers
		iret

.section .bss
	tmp_fault_err:
		.long 0
	tmp_fault_code:
		.long 0

.section .rodata
	fault_error:
		.ascii "Fault occured!"
