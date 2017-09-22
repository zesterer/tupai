//
// file : check.S
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

.global _check.boot

.section .text.boot

.set multiboot_magic, 0x36D76289

	_check.boot: // Check CPU compatibility

		call _check_mb
		call _check_cpuid

		push $0x0F
		push $_check_success_str
		call _print.boot
		add $8, %esp

		ret

	_check_mb:

		mov (_mb_magic.boot), %eax
		cmp $multiboot_magic, %eax
		je 1f

		// Error occured
		push $0x4F
		push $_check_nomb_str
		call _print.boot
		add $8, %esp

		jmp _hang.boot

		1:
			// No error occured
			push $0x0F
			push $_check_mb_str
			call _print.boot
			add $8, %esp

			ret

	_check_cpuid: // Check that the CPUID instruction exists

		pushf // Copy EFLAGS into EAX (via stack)
		pop %eax
		mov %eax, %ecx // Copy to ECX to compare later
		xor $(1 << 21), %eax // Flip the ID bit
		push %eax // Copy EAX to EFLAGS via the stack
		popf
		pushf // Copy EFLAGS back to EAX (via the stack) to check if the bit has flipped
		pop %eax
		push %ecx // Restore EFLAGS from original copy stored in ECX
		popf

		// The value of EFLAGS is now as it originally was

		// Compare the original EFLAGS with the (attempted) modified EFLAGS to detect a change
		// If no change has occured, CPUID is not supported!
		cmp %eax, %ecx
		jne 1f

		// Error occured
		push $0x4F
		push $_check_nocpuid_str
		call _print.boot
		add $8, %esp

		// Hang
		jmp _hang.boot

		1:
			// No error occured
			push $0x0F
			push $_check_cpuid_str
			call _print.boot
			add $8, %esp

			ret

.section .rodata.boot
	_check_success_str:
		.ascii "[DONE] CPU compatibility checks passed\n\0"
	_check_mb_str:
		.ascii "[ OK ] Bootloader is Multiboot-compliant\n\0"
	_check_nomb_str:
		.ascii "[FAIL] Bootloader is not Multiboot-compliant\n\0"
	_check_cpuid_str:
		.ascii "[ OK ] CPU supports cpuid\n\0"
	_check_nocpuid_str:
		.ascii "[FAIL] CPU does not support cpuid\n\0"
