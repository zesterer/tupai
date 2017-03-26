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

; Panic
extern panic

; Checks
extern check_multiboot
extern check_cpuid
extern check_longmode

global start

section .text
	bits 32

	; Kernel entry
	start:
		; Set the stack pointer
		mov esp, stack_top

		; Run initial checks
		call run_checks

		; Setup paging
		call setup_paging

		; Enable paging
		call enable_paging

		; Print 'OK'
		mov dword [0xB8000], 0x2F4B2F4F
		hlt

	; Run initial checks
	run_checks: ; Run a set of initial checks on the system
		call check_multiboot
		call check_cpuid
		call check_longmode
		ret

	; Setup paging
	setup_paging:
		; Map first P4 entry to P3 table
		mov eax, p3_table
		or eax, 0b11 ; Present and writable
		mov [p4_table], eax

		; Map first P3 entry to P2 table
		mov eax, p2_table
		or eax, 0b11 ; Present and writable
		mov [p3_table], eax

		; Map P2 table entries
		mov ecx, 0
		map_p2_loop:
			; Map the ECX-th P2 entry to a page at address 2M * ecx
			mov eax, 0x200000             ; 2M
			imul eax, ecx                 ; Calculate 2M * ecx
			or eax, 0b10000011            ; Present, writable and 'huge' (2MB)
			mov [p2_table + ecx * 8], eax ; Map ECX-th entry (each entry is 8 bytes)

			; Iterate the loop
			inc ecx
			cmp ecx, 512
			jne map_p2_loop

		ret

	; Enable paging
	enable_paging:
		; Load P4 table to CR3 (so the CPU knows where to look for paging info)
		mov eax, p4_table
		mov cr3, eax

		; Enable PAE flag in CR4 (Physical Address Extension)
		mov eax, cr4
		or eax, (1 << 5) ; (6th bit)
		mov cr4, eax

		; Set the long-mode bit in the EFER MSR (Model-Specific Register)
		mov ecx, 0xC0000080
		rdmsr            ; Load MSR into EAX
		or eax, (1 << 8) ; (9th bit)
		wrmsr            ; Load EAX back into MSR

		; Enable paging in CR0
		mov eax, cr0
		or eax, (1 << 31) ; (32nd bit)
		mov cr0, eax

		; We've enabled paging (we're still in a 32-bit compatibility mode though)

		ret

section .bss
	; Stack
	stack_bottom:
		resb 256 ; Reserve a 256-byte stack
	stack_top:

	; Page tables 'n stuff
	alignb 4096
	p4_table:
		resb 4096
	p3_table:
		resb 4096
	p2_table:
		resb 4096
