	//
// file : kbd.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/x86/kbd.hpp>
#include <tupai/x86/pic.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/interrupt.hpp>
#include <tupai/util/out.hpp>
#include <tupai/dev/tty.hpp>
#include <tupai/debug.hpp>

#include <tupai/x86/textmode.hpp>

namespace tupai
{
	namespace x86
	{
		static const uint16_t KBD_CMD_PORT    = 0x60;
		static const uint16_t KBD_STATUS_PORT = 0x64;
		static const uint16_t KBD_DATA_PORT   = 0x60;

		const char* scancode_table = "!!1234567890-=\b\tqwertyuiop[]\n!asdfghjkl;'#!\\zxcvbnm,./!!! !FFFFFFFFFF!";

		extern "C" void isr_kbd();
		extern "C" void kbd_isr_main();

		void kbd_bind()
		{
			// Bind the interrupt
			//interrupt_bind(1, (void*)isr_kbd, true);
		}

		void kbd_init()
		{
			// Temporary
			{
				wait();
				uint8_t val = inb(KBD_DATA_PORT);
				wait();
				val = val & 0xBF;
				outb(KBD_STATUS_PORT, 0x60);
				wait();
				outb(KBD_CMD_PORT, val);
			}

			// Unmask the interrupt
			interrupt_mask(1, true);
		}

		void kbd_isr_main()
		{
			// Acknowledge the interrupt
			interrupt_ack(1);

			// TEMPORARY
			/*{
				// Lowest bit of status will be set if buffer is not empty
				uint8_t status = inb(KBD_STATUS_PORT);
				if ((status & 0x01) != 0x01) // If the buffer is empty, stop reading scancode bytes
					return;

				char keycode = inb(KBD_DATA_PORT);
				if (keycode < 0)
					return;
				char character = scancode_table[(size_t)keycode];

				dev::tty_write_in(character);
				util::println("Hi!");
			}*/
		}
	}
}
