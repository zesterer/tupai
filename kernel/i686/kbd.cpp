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
#include <tupai/i686/gdt.hpp>
#include <tupai/i686/port.hpp>
#include <tupai/kpanic.hpp>
#include <tupai/tty.hpp>
#include <tupai/generic/ringbuff.hpp>

namespace tupai
{
	namespace i686
	{
		// Default interrupt handler
		extern "C" void kbd_irq_handler();
		//asm volatile ("kbd_irq_handler: \n call kbd_irq_handler_main \n iret");
		asm volatile (
						".section .text \n"
						"	.align 4 \n"
		 				"	kbd_irq_handler: \n"
						"		pushal \n"
						"		call kbd_irq_handler_main \n"
						"		popal \n"
						"		iret \n"
						);

		const char* scancode_table = "!!1234567890-=\b!qwertyuiop[]\n!asdfghjkl;'#!\\zxcvbnm,./!!! !FFFFFFFFFF!";

		// A 256-character keyboard ring buffer
		//generic::ringbuff<char> kbd_ringbuff;
		volatile char key_char = '\0';

		void kbd_init()
		{
			// Set the keyboard IRQ handler
			idt_set_entry(1, (uint32)kbd_irq_handler, sizeof(gdt_entry) * 1);

			/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
			port_out8(0x21 , 0xFD);

			// Allocate space for the ring buffer
			//kbd_ringbuff.init(10);
			 key_char = '\0';
		}

		extern "C" void kbd_irq_handler_main()
		{
			/* write EOI */
			port_out8(0x20, 0x20);

			ubyte status = port_in8(0x60);//KEYBOARD_STATUS_PORT);
			/* Lowest bit of status will be set if buffer is not empty */
			if (status > 0)
			{
				char keycode = port_in8(0x60);//KEYBOARD_DATA_PORT);
				if (keycode < 0)
					return;

				char character = scancode_table[(umem)keycode];

				//tty_write(character);
				//kbd_ringbuff.push(character);
				key_char = character;
			}
		}
	}
}
