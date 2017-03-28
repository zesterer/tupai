//
// file : check32.s
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

.global run_checks32

.section .text.boot
	.code32

	run_checks32: // Run a set of initial checks on the system
		call check_multiboot
		call check_cpuid
		call check_longmode
		ret

	panic: // Display an ASCII error string
		call printstr
		hlt

	printstr:
		mov $0, %ecx
		strloop:
			// Find current character, store in BL
			mov %ecx, %ebx
			imul $1, %ebx
			add %eax, %ebx
			mov (%ebx), %bl

			// Should we exit the loop?
			cmp $0, %bl
			je strend

			// Find the VGA buffer position, store in EDX
			mov %ecx, %edx
			imul $2, %edx
			add $0xB8000, %edx

			// Print the character
			movb %bl, (%edx)
			inc %edx
			// Give it color
			movb $0x4F, (%edx)

			// Iterate the loop
			add $1, %ecx
			jmp strloop
		strend:
		ret

	check_multiboot:
		cmp $0x36D76289, %eax
		jne error_no_multiboot
		ret

	error_no_multiboot:
		mov $error_no_multiboot_str, %eax
		jmp panic

	check_cpuid:
		// Check if CPUID is supported by attempting to flip the ID bit (bit 21) in FLAGS

		// Copy EFLAGS into EAX (via stack)
		pushf
		pop %eax

		// Copy to ECX to compare later
		mov %eax, %ecx

		// Flip the ID bit
		xor $(1 << 21), %eax

		// Copy EAX to EFLAGS via the stack
		push %eax
		popf

		// Copy EFLAGS back to EAX (via the stack) to check if the bit has flipped
		pushf
		pop %eax

		// Restore EFLAGS from original copy stored in ECX
		push %ecx
		popf

		// The value of EFLAGS is now as it originally was

		// Compare the original EFLAGS with the (attempted) modified EFLAGS to detect a change
		// If no change has occured, CPUID is not supported!
		cmp %eax, %ecx
		je error_no_cpuid

		ret

	error_no_cpuid:
		mov $error_no_cpuid_str, %eax
		jmp panic

	check_longmode:
		// Test if extended processor info is available
		mov $0x80000000, %eax // EAX is an implicit argument for cpuid
		cpuid                 // Get highest supported argument
		cmp $0x80000001, %eax // It should be AT LEAST 0x80000001
		jb error_no_longmode  // If it's less, we don't have long mode support

		// Use extended processor info to test if long mode is available
		mov $0x80000001, %eax // EAX is an implicit argument for cpuid
		cpuid                 // Returns feature bits in ecx and edx
		test $(1 << 29), %edx // Test if the LongMode bit is set in EDX
		jz error_no_longmode

		ret

	error_no_longmode:
		mov $error_no_longmode_str, %eax
		jmp panic

.section .rodata.boot
	error_no_multiboot_str:
		.ascii "Panic: Kernel not booted by a multiboot bootloader"
	error_no_cpuid_str:
		.ascii "Panic: CPU does not support the CPUID instruction"
	error_no_longmode_str:
		.ascii "Panic: CPU is not 64-bit compatible"
