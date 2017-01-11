//
//	file : kpanic.s
//
// 	This file is part of Tupai.
//
//	Tupai is free software: you can redistribute it and/or modify
// 	it under the terms of the GNU General Public License as published by
// 	the Free Software Foundation, either version 3 of the License, or
// 	(at your option) any later version.
//
// 	Tupai is distributed in the hope that it will be useful,
// 	but WITHOUT ANY WARRANTY; without even the implied warranty of
// 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// 	GNU General Public License for more details.
//
// 	You should have received a copy of the GNU General Public License
// 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Constants
.set KPANIC_BACK_FORMAT,  ((0x1 << 12) | (0x4 << 8))
.set KPANIC_TEXT_FORMAT,  ((0x1 << 12) | (0xF << 8))
.set KPANIC_TITLE_FORMAT, ((0x4 << 12) | (0xF << 8))
.set KPANIC_ERROR_FORMAT, ((0x1 << 12) | (0xC << 8))
.set KPANIC_EMAIL_FORMAT, ((0x1 << 12) | (0xA << 8))
.set VGA_BUFFER,          0xB8000
.set VGA_W,               80
.set VGA_H,               25

.section .bss
	.global _kpanic_errormsg
	_kpanic_errormsg:
		.skip 4
	.global _kpanic_errorcode
	_kpanic_errorcode:
		.skip 4

.section .rodata
	.skip 16
	_kpanic_message0:
		.ascii " Kernel Panic! \0"
	_kpanic_message1:
		.ascii "Tupai's kernel has experienced a critical error and cannot recover.\0"
	_kpanic_message2:
		.ascii "Please email this error message and any extra information about the crash to:\0"
	_kpanic_message3:
		.ascii "joshua.s.barretto@gmail.com\0"
	_kpanic_message_noerror:
		.ascii "[NO ERROR MESSAGE]\0"
	_kpanic_hexcodes:
		.ascii "0123456789ABCDEF"

.section .boot_text
	.global _kpanic
	_kpanic:
		cli

		movl $_kpanic_errormsg, %edx
		movl 4(%esp), %eax
		movl %eax, (%edx)

		movl $_kpanic_errorcode, %edx
		movl 8(%esp), %eax
		movl %eax, (%edx)

		// Clear the screen
		mov $0, %ecx
		_kpanic_clearloop:
			mov $(VGA_BUFFER), %ebx
			add %ecx, %ebx
			movw $KPANIC_BACK_FORMAT, (%ebx)
			add  $2, %ecx
			cmp  $(2 * VGA_W * VGA_H), %ecx
			jne  _kpanic_clearloop

		// Show an error message
		mov $(VGA_BUFFER + 2 * (32 + VGA_W * 1)), %ebx
		mov $_kpanic_message0, %ecx
		// Loop through characters
		_kpanic_msgloop0:
			movl $0, %eax
			movb (%ecx), %al
			or   $KPANIC_TITLE_FORMAT, %eax
			movw %ax, (%ebx)
			add  $2, %ebx
			add  $1, %ecx
			cmpb $0, (%ecx)
			jne  _kpanic_msgloop0

		// Show an error message
		mov $(VGA_BUFFER + 2 * (1 + VGA_W * 3)), %ebx
		mov $_kpanic_message1, %ecx
		// Loop through characters
		_kpanic_msgloop1:
			movl $0, %eax
			movb (%ecx), %al
			or   $KPANIC_TEXT_FORMAT, %eax
			movw %ax, (%ebx)
			add  $2, %ebx
			add  $1, %ecx
			cmpb $0, (%ecx)
			jne  _kpanic_msgloop1

		// Show an error message
		mov $(VGA_BUFFER + 2 * (1 + VGA_W * 5)), %ebx
		mov $_kpanic_errormsg, %ecx
		//call kbreak
		mov (%ecx), %ecx
		//call kbreak
		// TODO
		cmp $0, %ecx
		jne  _kpanic_errormsg_continue
		mov $_kpanic_message_noerror, %ecx
		_kpanic_errormsg_continue:
		// Loop through characters
		_kpanic_errormsg_loop:
			movl $0, %eax
			movb (%ecx), %al
			or   $KPANIC_ERROR_FORMAT, %eax
			movw %ax, (%ebx)
			add  $2, %ebx
			add  $1, %ecx
			cmpb $0, (%ecx)
			jne  _kpanic_errormsg_loop

		// Show a 32-bit error code in hexadecimal form
		mov $(VGA_BUFFER + 2 * (1 + VGA_W * 7)), %ebx
		mov $0, %ecx
		// Display '0x' prefix
		movw $(KPANIC_ERROR_FORMAT | '0'), (%ebx)
		add $2, %ebx
		movw $(KPANIC_ERROR_FORMAT | 'x'), (%ebx)
		add $2, %ebx
		// Get the error code
		mov $_kpanic_errorcode, %edx
		mov (%edx), %edx
		// Loop in order to display the error code
		_kpanic_errloop:
			mov %edx, %eax // Load the error code into %eax (the working calculation register)
			// Computer hexadecimal value
			and $(0xF << 28), %eax // Use a bitmask to find the next hexadecimal value
			shr $28, %eax // Shift the error code along to find the nth hexadecimal value
			// Compute hexadecimal character
			add  $_kpanic_hexcodes, %eax // Add the current hex value to the hexcode pointer to find our hex character
			movb (%eax), %al // Actually put the hex character into the %eax register
			and  $0xFF, %eax
			// Write to VGA buffer
			or   $KPANIC_ERROR_FORMAT, %eax // Add standard text colouring
			movw %ax, (%ebx) // Move the VGA word into the correct location
			// Counters and incrementing
			add  $2, %ebx // Increment the VGA buffer position
			shl  $4, %edx // Shift the error code ready for the next hex value
			add  $4, %ecx // Increment the bit counter
			// Comparison statement
			cmp  $32, %ecx // Compare the bit counter with the error code length (32 bits)
			jne  _kpanic_errloop

		// Show an error message
		mov $(VGA_BUFFER + 2 * (1 + VGA_W * 9)), %ebx
		mov $_kpanic_message2, %ecx
		// Loop through characters
		_kpanic_msgloop2:
			movl $0, %eax
			movb (%ecx), %al
			or   $KPANIC_TEXT_FORMAT, %eax
			movw %ax, (%ebx)
			add  $2, %ebx
			add  $1, %ecx
			cmpb $0, (%ecx)
			jne  _kpanic_msgloop2

		// Show an error message
		mov $(VGA_BUFFER + 2 * (1 + VGA_W * 11)), %ebx
		mov $_kpanic_message3, %ecx
		// Loop through characters
		_kpanic_msgloop3:
			movl $0, %eax
			movb (%ecx), %al
			or   $KPANIC_EMAIL_FORMAT, %eax
			movw %ax, (%ebx)
			add  $2, %ebx
			add  $1, %ecx
			cmpb $0, (%ecx)
			jne  _kpanic_msgloop3

		jmp _khalt
