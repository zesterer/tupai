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
#include <tupai/x86/ps2_8042.hpp>
#include <tupai/dev/tty.hpp>
#include <tupai/irq.hpp>

#include <tupai/x86/textmode.hpp>

namespace tupai
{
	namespace x86
	{
		static const uint16_t KBD_CMD_PORT    = 0x60;
		static const uint16_t KBD_STATUS_PORT = 0x64;
		static const uint16_t KBD_DATA_PORT   = 0x60;

		const char scancode_table[] = "!!1234567890-=\b\tqwertyuiop[]\n!asdfghjkl;'#!\\zxcvbnm,./!!! !FFFFFFFFFF!";

		extern "C" void isr_kbd();
		extern "C" size_t kbd_isr_main(size_t stack);

		void kbd_bind()
		{
			// Bind the interrupt
			irq::bind(1, (void*)isr_kbd, true);
		}

		void kbd_init()
		{
			ps2_8042_open_port(0);

			// Temporary
			/*{
				wait();
				uint8_t val = inb(KBD_DATA_PORT);
				wait();
				val = val & 0xBF;
				outb(KBD_STATUS_PORT, 0x60);
				wait();
				outb(KBD_CMD_PORT, val);
			}*/

			// Unmask the interrupt
			irq::mask(1, true);
		}

		size_t kbd_isr_main(size_t stack)
		{
			// Acknowledge the interrupt
			irq::ack(1);

			// TEMPORARY
			while (true)
			{
				// Lowest bit of status will be set if buffer is not empty
				uint8_t status = inb(KBD_STATUS_PORT);
				if ((status & 0x01) != 0x01) // If the buffer is empty, stop reading scancode bytes
					break;

				uint8_t keycode = inb(KBD_DATA_PORT);
				if (keycode >= sizeof(scancode_table) / sizeof(char))
					break;

				char c = scancode_table[(size_t)keycode];

				dev::tty_write_in(c);
			}

			return stack;
		}
	}
}
