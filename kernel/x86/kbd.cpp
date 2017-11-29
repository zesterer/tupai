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

#include <tupai/x86/kbd.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/cpu/irq.hpp>
#include <tupai/util/boot.hpp>
#include <tupai/util/char.hpp>
#include <tupai/sys/logger.hpp>

namespace tupai::x86::kbd
{
	static const uint8_t IRQ = 1;

	// I/O ports
	static const uint16_t CMD_PORT = 0x60;
	static const uint16_t STATUS_PORT = 0x64;
	static const uint16_t DATA_PORT = 0x60;

	// State modifiers
	static const size_t MOD_SHIFT = 0;
	static const size_t MOD_CTRL = 1;
	static const size_t MOD_ALT = 2;

	// State
	static bool modifiers[3] = { false, };

	// Special keys
	static const uint8_t KEY_LSHIFT_DOWN = 0x2A;
	static const uint8_t KEY_LSHIFT_UP = 0xAA;

	// Keyboard lookup table
	static const unsigned char kbd_en_us[128] =
	{
		0, 27, '1', '2', '3', '4', '5', '6', '7', '8',	// 9
		'9', '0', '-', '=', '\b', // Backspace
		'\t', // Tab
		'q', 'w', 'e', 'r',	// 19
		't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // Enter key
		0, // 29 - Control
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 39
		'\'', '`',
		0,   // Left shift
		'\\', 'z', 'x', 'c', 'v', 'b', 'n', // 49
		'm', ',', '.', '/',
		0,   // Right shift
		'*',
		0,   // Alt
		' ', // Space bar
		0,   // Caps lock
		0,   // 59 - F1 key ... >
		0,   0,   0,   0,   0,   0,   0,   0,
		0,   // < ... F10
		0,   // 69 - Num lock
		0,   // Scroll Lock
		0,   // Home key
		0,   // Up Arrow
		0,   // Page Up
		'-',
		0,   // Left Arrow
		0,
		0,   // Right Arrow
		'+',
		0,   // 79 - End key
		0,   // Down Arrow
		0,   // Page Down
		0,   // Insert Key
		0,   // Delete Key
		0, 0, 0,
		0,   // F11 Key
		0,   // F12 Key
		0,   // All other keys are undefined
	};

	// Extern ISR stub reference
	extern "C" void _isr_kbd_stub();

	void init()
	{
		cpu::irq::set_handler(IRQ, (uintptr_t)_isr_kbd_stub);
		util::bootlog("PS2 keyboard ISR installed");

		cpu::irq::unmask_irq(IRQ);
		util::bootlog("PS2 keyboard IRQ unmasked");
	}

	extern "C" uintptr_t isr_kbd(uintptr_t stack)
	{
		while (io::in<8>(STATUS_PORT) & 0b00000001) // While there are bytes to read
		{
			uint8_t sc = io::in<8>(DATA_PORT); // Read a scancode

			if (sc & 0b10000000) // It's a key release
			{
				switch (sc)
				{
				case KEY_LSHIFT_UP:
					modifiers[MOD_SHIFT] = false;
					break;

				default:
					break;
				}
			}
			else // It's a key press
			{
				switch (sc)
				{
				case KEY_LSHIFT_DOWN:
					modifiers[MOD_SHIFT] = true;
					break;

				default:
					char c = kbd_en_us[sc];
					if (modifiers[MOD_SHIFT])
						c = util::to_uppercase(c);
					util::logc(c);
					break;
				}
			}
		}

		cpu::irq::ack(IRQ);
		return stack;
	}
}
