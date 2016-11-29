/*
* 	file : pit.cpp
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
#include <tupai/i686/pit.hpp>
#include <tupai/i686/idt.hpp>
#include <tupai/i686/gdt.hpp>
#include <tupai/i686/port.hpp>
#include <tupai/kpanic.hpp>
#include <tupai/tty.hpp>

namespace tupai
{
// Default interrupt handler
	extern "C" void pit_irq_handler();
	//asm volatile ("pit_irq_handler: \n call pit_irq_handler_main \n iret");
	asm volatile (
					".section .text \n"
					"	.align 4 \n"
	 				"	pit_irq_handler: \n"
					"		pushal \n"
					"		call pit_irq_handler_main \n"
					"		popal \n"
					"		iret \n"
					);

	void pit_init()
	{
		// Set the PIT IRQ handler
		idt_set_entry(0, (uint32)pit_irq_handler, sizeof(gdt_entry) * 1);

		/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
		port_out8(0x21 , 0xFD);


	}

	extern "C" void pit_irq_handler_main()
	{
		/* write EOI */
		port_out8(0x20, 0x20);

		tty_write_str("PIT!");
	}
}
