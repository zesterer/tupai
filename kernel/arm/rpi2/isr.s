//
// file : isr.s
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
