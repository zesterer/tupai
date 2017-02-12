/*
* 	file : tty.cpp
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
#include <tupai/tty.hpp>
#include <tupai/console.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/util/char.hpp>
#include <tupai/util/conv.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/serial.hpp>
#endif

// Libk
#include <libk/ctype.hpp>

namespace tupai
{
	safeptr<console> g_console;
	util::mutex tty_mutex;

	void tty_init()
	{
		// Find the global console
		g_console = console_get_global();

		tty_clear();
	}

	void tty_write(char c)
	{
		// If it's an invalid pointer, don't do anything
		if (g_console.is_valid())
			g_console.val()->write_char(c);

		#if defined(SYSTEM_ARCH_i686)
			#if defined(CFG_ENABLE_SERIAL_DEBUG)
				serial_write(1, c); // Write to COM1 debug
			#endif
		#endif
	}

	void tty_write_str(const char* str)
	{
		for (umem i = 0; str[i] != '\0'; i ++) // Write each character
			tty_write(str[i]);
	}

	void tty_set_fg_color(ubyte color)
	{
		// ANSI
		tty_write(0x1B); // Escape
		if (color >= 8)
			tty_write('9');
		else
			tty_write('3');
		tty_write(util::num_to_hex(color % 8).val());
		tty_write('m'); // 'm' SGR parameter
	}

	void tty_set_bg_color(ubyte color)
	{
		// ANSI
		tty_write(0x1B); // Escape
		if (color >= 8)
		{
			tty_write('1');
			tty_write('0');
		}
		else
			tty_write('4');
		tty_write(util::num_to_hex(color % 8).val());
		tty_write('m'); // 'm' SGR parameter
	}

	void tty_place_cursor(uint16 col, uint16 row)
	{
		// ANSI
		tty_write(0x1B); // Escape
		tty_write_str(util::compose<uint16>(col).val().raw());
		tty_write(';');
		tty_write_str(util::compose<uint16>(row).val().raw());
		tty_write('f'); // 'f' move cursor parameter
	}

	void tty_show_cursor()
	{
		// ANSI
		tty_write(0x1B); // Escape
		tty_write('?');
		tty_write('2');
		tty_write('5');
		tty_write('h'); // 'h' show cursor parameter
	}

	void tty_hide_cursor()
	{
		// ANSI
		tty_write(0x1B); // Escape
		tty_write('?');
		tty_write('2');
		tty_write('5');
		tty_write('l'); // 'h' hide cursor parameter
	}

	void tty_reset()
	{
		// ANSI
		tty_write(0x1B); // Escape
		tty_write('0');
		tty_write('m'); // 'm' SGR parameter
	}

	void tty_clear()
	{
		// ANSI
		tty_write(0x1B); // Escape
		tty_write(util::num_to_digit(2).val());
		tty_write('J'); // 'J' clear screen parameter
	}
}
