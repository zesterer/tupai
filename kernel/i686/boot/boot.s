//
//	file : boot.s
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

// A temporary 16 KB bootstrap stack
.section .boot_stack, "aw", @nobits
	_boot_stack_bottom:
	.skip 0x4000 // 16 KB
	_boot_stack_top:

// Now for some actual code
.section .boot_text
	.global _boot_entry
	.type _boot_entry, @function
	_boot_entry:
		// Tell the stack pointer where the bootstrap stack is
		movl $_boot_stack_top, %esp

		// We now have a C-worthy (get it?) environment
		// Time to jump into kernel early C
		call kearly

		// C++ constructor code
		call _init

		// Main kernel code
		call kmain

		// C++ destructor code
		call _fini

		// If the kernel call returns, halt the system
		jmp _khalt

	// A halt procedure
	.global _khalt
	_khalt:
		hlt
		jmp _khalt

	// A hang procedure
	.global _khang
	_khang:
		cli
		hlt
		jmp _khang

	// Set the size of the _boot_entry label to the current location minus its beginning position
	.size _boot_entry, . - _boot_entry
