;
; file : check_multiboot.asm
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

global check_multiboot

section .text
	bits 32

	check_multiboot:
		cmp eax, 0x36D76289
		jne error_no_multiboot
		ret
	error_no_multiboot:
		mov eax, error_no_multiboot_str
		jmp panic

section .data
	error_no_multiboot_str:
		dd "Panic: Kernel not booted by a multiboot bootloader"
		dw 0
