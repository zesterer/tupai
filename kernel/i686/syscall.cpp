/*
* 	file : syscall.cpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

// Tupai
#include <tupai/syscall.hpp>
#include <tupai/kpanic.hpp>

#include <tupai/i686/interrupt.hpp>
#include <tupai/i686/idt.hpp>
#include <tupai/i686/gdt.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	extern "C" void syscall_irq_handler_main();

	// Syscall interrupt handler
	extern "C" void syscall_irq_handler();
	//asm volatile ("pit_irq_handler: \n call pit_irq_handler_main \n iret");
	asm volatile (
					".section .text \n"
					"	.align 4 \n"
	 				"	syscall_irq_handler: \n"
					"		pushal \n"
					"		call syscall_irq_handler_main \n"
					"		popal \n"
					"		iret \n"
					);

	void syscall_init()
	{
		idt_set_entry(0x80 - IDT_REMAP_OFFSET, (uint32)syscall_irq_handler, sizeof(gdt_entry) * 1);
	}

	void syscall_irq_handler_main()
	{
		interrupt_ack(0x80);

		libk::printf("SYSCALL!\n");
	}
}
