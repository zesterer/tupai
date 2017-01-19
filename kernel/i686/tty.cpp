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
#include <tupai/i686/vga.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/util/char.hpp>

#include <tupai/early/out.hpp>

// Libk
#include <libk/ctype.hpp>

namespace tupai
{
	vga_color tty_vga_bg = (vga_color)tty_color::DEFAULT_BG;
	vga_color tty_vga_fg = (vga_color)tty_color::DEFAULT_FG;

	mutex tty_mutex;

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
		tty_write(util::num_to_hex(color)); // Convert to color to hex TODO : change this
		tty_write('m'); // 'm' SGR parameter
	}

	void tty_set_bg_color(ubyte color)
	{
		tty_mutex.lock();

		// TODO : Background color

		tty_mutex.unlock();
	}

	void tty_place_cursor(uint16 col, uint16 row)
	{
		tty_mutex.lock();

		// TODO : Place

		tty_mutex.unlock();
	}

	void tty_clear()
	{
		tty_mutex.lock();

		// TODO : Clear

		tty_mutex.unlock();
	}
}
