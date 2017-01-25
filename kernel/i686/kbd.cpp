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

#include <tupai/early/out.hpp>
#include <tupai/util/conv.hpp>
#include <tupai/util/char.hpp>

namespace tupai
{
	static const uint16 KBD_COMMAND_PORT = 0x60;
	static const uint16 KBD_STATUS_PORT = 0x64;
	static const uint16 KBD_DATA_PORT = 0x60;

	static const uint16 KBD_MAX_COLS = 16;
	static const uint16 KBD_MAX_ROWS = 8;

	byte key_state[KBD_MAX_ROWS][KBD_MAX_COLS]; // 0 = released, 1 = pressed

	bool shift_down = false;
	static const char scancode_2_uk[] =
	{
	//   0    1    2    3    4    5    6    7
	//   8    9    A    B    C    D    E    F
		'?', '!', '?', '!', '!', '!', '!', '!', // 0x00 - 0x07
		'?', '!', '!', '!', '!', '!', '!', '?', // 0x08 - 0x0F

		'?', '!', '!', '?', '!', 'q', '1', '?', // 0x10 - 0x17
		'?', '?', 'z', 's', 'a', 'w', '2', '?', // 0x18 - 0x1F

		'?', 'c', 'x', 'd', 'e', '4', '3', '?', // 0x20 - 0x27
		'?', ' ', 'v', 'f', 't', 'r', '5', '?', // 0x28 - 0x2F

		'?', 'n', 'b', 'h', 'g', 'y', '6', '?', // 0x30 - 0x37
		'?', '?', 'm', 'j', 'u', '7', '8', '?', // 0x38 - 0x3F

		'?', ',', 'k', 'i', 'o', '0', '9', '?', // 0x40 - 0x47
		'?', '.', '/', 'l', ';', 'p', '-', '?', // 0x48 - 0x4F

		'?', '?','\'', '?', '[', '=', '?', '?', // 0x50 - 0x57
		'!', '!','\n', ']', '?','\\', '?', '?', // 0x58 - 0x5F

		'?', '?', '?', '?', '?', '?','\b', '?', // 0x60 - 0x67
		'?', '1', '?', '4', '7', '?', '?', '?', // 0x68 - 0x6F

		'0', '.', '2', '5', '6', '8', '!', '!', // 0x70 - 0x77
		'!', '+', '3', '-', '*', '9', '!', '?', // 0x78 - 0x7F

		'?', '?', '?', '!', '?', '?', '?', '?', // 0x80 - 0x87
	};

	static void kbd_handle_scancode(byte* scancode, umem n);
	static void kbd_send_command(byte* command, umem n);

	const char* scancode_table = "!!1234567890-=\b\tqwertyuiop[]\n!asdfghjkl;'#!\\zxcvbnm,./!!! !FFFFFFFFFF!";

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

	// A 256-character keyboard ring buffer
	generic::ringbuff<char> kbd_ringbuffer;

	void kbd_init()
	{
		// Set the keyboard IRQ handler
		idt_set_entry(IDT_REMAP_OFFSET + 0x1, (uint32)kbd_irq_handler, sizeof(gdt_entry) * 1);

		// Allocate space for the ring buffer
		kbd_ringbuffer.init(256);
		kbd_ringbuffer.set_push_locked(false); // Disable push-locking to prevent keyboard buffer lock-ups

		// Clear the key state buffer
		for (umem row = 0; row < KBD_MAX_ROWS; row ++)
		{
			for (umem col = 0; col < KBD_MAX_COLS; col ++)
				key_state[row][col] = 0x0; // Zero it
		}

		// Self test
		//byte cmd_selftest[1] = {0xFF};
		//kbd_send_command(cmd_selftest, sizeof(cmd_selftest) / sizeof(byte));

		// Set the keyboard scancode set to 2
		//byte cmd_scancode2[2] = {0xF0, 0x02};
		//kbd_send_command(cmd_scancode2, sizeof(cmd_scancode2) / sizeof(byte));

		// Disable first PS/2 port translation TODO : Clean tbis up, place into generic PS/2 driver
		/*port_out8(0x64, 0x20);
		port_wait();
		byte val = port_in8(0x60);
		port_wait();
		val = val & 0xBF;
		port_out8(0x64, 0x60);
		port_wait();
		port_out8(0x60, val);*/
	}

	void kbd_enable()
	{
		// Unmask IRQ1 (keyboard)
		pic_set_mask(1, true);
	}

	extern "C" void kbd_irq_handler_main()
	{
		interrupt_ack(IDT_REMAP_OFFSET + 0x1);

		byte scancode[6];
		umem index = 0;

		while (true)
		{
			/* Lowest bit of status will be set if buffer is not empty */
			byte status = port_in8(KBD_STATUS_PORT);
			if ((status & 0x01) != 0x01) // If the buffer is empty, stop reading scancode bytes
				break;

			if (true)
			{
				char keycode = port_in8(KBD_DATA_PORT);
				if (keycode < 0)
					return;

				char character = scancode_table[(umem)keycode];

				kbd_ringbuffer.push(character);
				return;
			}
			else
			{
				byte scanbyte = port_in8(KBD_DATA_PORT);

				// Append to the scancode stack
				if (index < 6)
				{
					scancode[index] = scanbyte;
					index ++;
				}
			}
		}

		// Now handle the scancode
		if (false) kbd_handle_scancode(scancode, index);
	}

	static void kbd_handle_scancode(byte* scancode, umem n)
	{
		if (n == 1 && scancode[0] <= 0x8F)
		{
			char character = scancode_2_uk[scancode[0]];

			if (shift_down)
				character = util::to_uppercase(character);

			if (character != '!')
				kbd_ringbuffer.push(character);
		}

		if (scancode[0] == 0x12)
			shift_down = true;
		else if (scancode[0] == 0xF0 && scancode[1] == 0x12)
			shift_down = false;

		/*
		for (umem i = 0; i < n; i ++)
		{
			early::print("part = 0x");
			early::print(util::compose<uint8>(scancode[i], 16).val().raw());
			early::print("\n");
		}
		*/
	}

	static void kbd_send_command(byte* command, umem n)
	{
		while ((port_in8(KBD_STATUS_PORT) & 0x02) > 0);
		port_out8(KBD_COMMAND_PORT, command[0]);
		port_wait();

		for (umem i = 1; i < n; i ++)
		{
			while ((port_in8(KBD_STATUS_PORT) & 0x02) > 0);
			port_out8(KBD_COMMAND_PORT, command[i]);
		}

		port_wait();
	}
}
