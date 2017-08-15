//
// file : kbd.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
