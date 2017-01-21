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
#include <tupai/util/mutex.hpp>
#include <tupai/util/char.hpp>
#include <tupai/util/conv.hpp>

#include <tupai/early/out.hpp>

// Libk
#include <libk/ctype.hpp>

namespace tupai
{
	util::mutex tty_mutex;

	void tty_init()
	{
		tty_clear();
	}

	void tty_write(char c)
	{
		tty_mutex.lock();

		early::printchar(c);

		tty_mutex.unlock();
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

	void tty_clear()
	{
		// ANSI
		tty_write(0x1B); // Escape
		tty_write(util::num_to_digit(2).val());
		tty_write('J'); // 'J' clear screen parameter
	}
}
