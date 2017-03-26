;
; file : multiboot.asm
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

section .multiboot_header
	header_start:
		dd 0xE85250D6                ; Multiboot 2 magic number
		dd 0                         ; Architecture 0 (protected mode i386)
		dd header_end - header_start ; Header length

		; Checksum
		dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))

		; Optional Multiboot tags [none yet]

		dw 0 ; Type
		dw 0 ; Flags
		dd 8 ; Size
	header_end:
