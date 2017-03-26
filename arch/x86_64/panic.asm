;
; file : panic.asm
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

global panic

section .text
	bits 32

	panic: ; Display an ASCII error string
		mov ecx, 0
		strloop:
			; Find current character, store in BL
			mov ebx, ecx
			imul ebx, 1
			add ebx, eax
			mov bx, [ebx]

			; Should we exit the loop?
			cmp bx, 0
			je strloop

			; Find the VGA buffer position, store in EDX
			mov edx, ecx
			imul edx, 2
			add edx, 0xB8000

			; Print the character
			mov word [edx], 0x4F00
			mov [edx], bl

			; Iterate the loop
			add ecx, 1
			jmp strloop
		strend:

		hlt
