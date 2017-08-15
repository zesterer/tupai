//
// file : start.s
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

.extern kentry
.extern enable_paging

.global start

.section .text.boot

	// Entry point for the kernel.
	// r15 -> should begin execution at 0x8000
	// r0  -> 0x00000000
	// r1  -> 0x00000C42
	// r2  -> 0x00000100 - start of ATAGS
	start:
		// Setup the stack
		mov sp, #0x8000

		// Enable the FPU (in coprocessor enable register)
		push {r0, r1, r2}
		ldr r0, =(0xF << 20)
		mcr p15, 0, r0, c1, c0, 2
		mov r3, #0x40000000 // Enable FPU in FP exception reg
		//vmsr FPEXC, r3
		.long 0xEEE83A10 // Assembler bug, replace with above line when binutils is fixed!
		pop {r0, r1, r2}

		// Set up identity paging
		bl enable_paging

		// Clear the BSS
		ldr r4, =__bss_start
		ldr r9, =__bss_end
		mov r5, #0
		mov r6, #0
		mov r7, #0
		mov r8, #0
		b 2f

		1:
			// Store multiple at r4
			stmia r4!, {r5 - r8}

			// If we are still below __bss_end, loop
		2:
			cmp r4, r9
			blo 1b

		// Entry arguments
		ldr r0, =#0x00000000
		ldr r1, =#0x00000C42
		ldr r2, =#0x00000100

		// Enter kernel main
		ldr r3, =kentry // Call the kernel's entry point
		blx r3
		ldr r3, =kmain  // Call kernel main procedure
		blx r3

	hang:
		wfe
		b hang
