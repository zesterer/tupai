;
; file : check_longmode.asm
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

global check_longmode

section .text
	bits 32

	check_longmode:
		; Test if extended processor info is available
		mov eax, 0x80000000  ; EAX is an implicit argument for cpuid
		cpuid                ; Get highest supported argument
		cmp eax, 0x80000001  ; It should be AT LEAST 0x80000001
		jb error_no_longmode ; If it's less, we don't have long mode support

		; Use extended processor info to test if long mode is available
		mov eax, 0x80000001 ; EAX is an implicit argument for cpuid
		cpuid               ; Returns feature bits in ecx and edx
		test edx, (1 << 29) ; Test if the LongMode bit is set in EDX
		jz error_no_longmode

		ret
	error_no_longmode:
		mov eax, error_no_longmode_str
		jmp panic

section .data
	error_no_longmode_str:
		dd "Panic: CPU is not 64-bit compatible"
		dw 0
