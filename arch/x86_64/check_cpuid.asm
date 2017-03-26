;
; file : check_cpuid.asm
;
; This file is part of Tupai.
;
; Tupai is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; Tupai is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
;

extern panic

global check_cpuid

section .text
	bits 32

	check_cpuid:
		; Check if CPUID is supported by attempting to flip the ID bit (bit 21) in FLAGS

		; Copy EFLAGS into EAX (via stack)
		pushfd
		pop eax

		; Copy to ECX to compare later
		mov ecx, eax

		; Flip the ID bit
		xor eax, (1 << 21)

		; Copy EAX to EFLAGS via the stack
		push eax
		popfd

		; Copy EFLAGS back to EAX (via the stack) to check if the bit has flipped
		pushfd
		pop eax

		; Restore EFLAGS from original copy stored in ECX
		push ecx
		popfd

		; The value of EFLAGS is now as it originally was

		; Compare the original EFLAGS with the (attempted) modified EFLAGS to detect a change
		; If no change has occured, CPUID is not supported!
		cmp eax, ecx
		je error_no_cpuid

		ret
	error_no_cpuid:
		mov eax, error_no_cpuid_str
		jmp panic

section .data
	error_no_cpuid_str:
		dd "Panic: CPU does not support the CPUID instruction"
		dw 0
