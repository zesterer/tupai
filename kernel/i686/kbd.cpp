/*
* 	file : kbd.cpp
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
#include <tupai/i686/kbd.hpp>
#include <tupai/i686/idt.hpp>
#include <tupai/i686/port.hpp>
#include <tupai/tty.hpp>

namespace tupai
{
	namespace i686
	{
		// Default interrupt handler
		extern "C" void kbd_irq_handler();
		asm volatile ("kbd_irq_handler: \n cli \n call kbd_irq_handler_main \n sti \n iret");

		void kbd_init()
		{
			// Set the keyboard IRQ handler
			//idt_set_entry(1, (uint32)kbd_irq_handler, 0x08);

			/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
			port_out8(0x21 , 0xFD);
		}

		extern "C" void kbd_irq_handler_main()
		{
			tty_write_str("Interrupt!\n");
		}
	}
}
