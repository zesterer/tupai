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

// Explanation:
// Some ISRs push error bytes onto the stack. So normalise all ISRs, we push a
// fake error byte onto the stack to maintain a uniform stack. We can then act
// as if all ISRs push error bytes

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
	.align 4
	isr_0: // Division By Zero Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $0 // ISR identifer
		jmp isr_common

	.align 4
	isr_1: // Debug Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $1 // ISR identifer
		jmp isr_common

	.align 4
	isr_2: // Non-Maskable Interrupt Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $2 // ISR identifer
		jmp isr_common

	.align 4
	isr_3: // Breakpoint Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $3 // ISR identifer
		jmp isr_common

	.align 4
	isr_4: // Into Detected Overflow Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $4 // ISR identifer
		jmp isr_common

	.align 4
	isr_5: // Out Of Bounds Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $5 // ISR identifer
		jmp isr_common

	.align 4
	isr_6: // Invalid Opcode Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $6 // ISR identifer
		jmp isr_common

	.align 4
	isr_7: // No Coprocessor Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $7 // ISR identifer
		jmp isr_common

	.align 4
	isr_8: // Double Fault Exception
		cli
		push $8 // ISR identifer
		jmp isr_common

	.align 4
	isr_9: // Coprocessor Segment Overrun Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $9 // ISR identifer
		jmp isr_common

	.align 4
	isr_10: // Bad TSS Exception
		cli
		push $10 // ISR identifer
		jmp isr_common

	.align 4
	isr_11: // Segment Not Present Exception
		cli
		push $11 // ISR identifer
		jmp isr_common

	.align 4
	isr_12: // Stack Fault Exception
		cli
		push $12 // ISR identifer
		jmp isr_common

	.align 4
	isr_13: // General Protection Fault Exception
		cli
		push $13 // ISR identifer
		jmp isr_common

	.align 4
	isr_14: // Page Fault Exception
		cli
		push $14 // ISR identifer
		jmp isr_common

	.align 4
	isr_15: // Unknown Interrupt Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $15 // ISR identifer
		jmp isr_common

	.align 4
	isr_16: // Coprocessor Fault Exception
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $16 // ISR identifer
		jmp isr_common

	.align 4
	isr_17: // Alignment Check Exception (486+)
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $17 // ISR identifer
		jmp isr_common

	.align 4
	isr_18: // Machine Check Exception (Pentium/586+)
		cli
		push $ISR_DUMMY_ERROR // Dummy error
		push $18 // ISR identifer
		jmp isr_common

	/* Please note: ISRs 19-31 are reserved and will be implemented later! */

	isr_common: // Common ISR routine (must be jumped to by an ISR defined above)
		//pushal

		//push %ds
		//push %es
		//push %fs
		//push %gs

		// Load the kernel data segment GDT descriptor
		//mov $0x10, %ax
		//mov %ax, %ds
		//mov %ax, %es
		//mov %ax, %fs
		//mov %ax, %gs

		//pushl 0x52(%esp) // Push ISR error as second parameter
		//pushl 0x44(%esp) // Push ISR identifer as first parameter
		push $exception_error
		call panic

		//pop %gs
		//pop %fs
		//pop %es
		//pop %ds

		//popal
		//add $8, %esp
		sti
		iret // Standard interrupt return

.section .rodata
	exception_error:
		.ascii "Exception occured!"
