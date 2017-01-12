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
#include <tupai/i686/interrupt.hpp>
#include <tupai/i686/pic.hpp>
#include <tupai/tty.hpp>

namespace tupai
{
	// Kbd interrupt handler
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

	const char* scancode_table = "!!1234567890-=\b\tqwertyuiop[]\n!asdfghjkl;'#!\\zxcvbnm,./!!! !FFFFFFFFFF!";

	// A 256-character keyboard ring buffer
	generic::ringbuff<char> kbd_ringbuffer;

	void kbd_init()
	{
		// Set the keyboard IRQ handler
		idt_set_entry(IDT_REMAP_OFFSET + 0x1, (uint32)kbd_irq_handler, sizeof(gdt_entry) * 1);

		// Enable IRQ1 (keyboard)
		pic_set_mask(1, true);

		// Allocate space for the ring buffer
		kbd_ringbuffer.init(256);
	}

	extern "C" void kbd_irq_handler_main()
	{
		interrupt_ack(IDT_REMAP_OFFSET + 0x1);

		ubyte status = port_in8(0x60);//KEYBOARD_STATUS_PORT);
		/* Lowest bit of status will be set if buffer is not empty */
		if (status > 0)
		{
			char keycode = port_in8(0x60);//KEYBOARD_DATA_PORT);
			if (keycode < 0)
				return;

			char character = scancode_table[(umem)keycode];

			kbd_ringbuffer.push(character);
		}
	}
}
