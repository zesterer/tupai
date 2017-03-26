;
; file : boot.asm
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

extern stack_top
extern stack_bottom

global start

section .text
	bits 32

	start:
		; Set the stack pointer
		mov esp, stack_top

		; Print 'OK'
		mov dword [0xB8000], 0x2F4B2F4F
		hlt

	error: ; Display an ASCII error code
		mov dword [0xB8000], 0x4F524F45
		mov dword [0xB8004], 0x4F3a4F52
		mov dword [0xB8008], 0x4F204F20
		mov byte  [0xB800A], al
		hlt
