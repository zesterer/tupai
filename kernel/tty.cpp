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

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/vga.hpp>
#endif

namespace tupai
{
	#if defined(SYSTEM_ARCH_i686)
		uint16 tty_vga_col;
		uint16 tty_vga_row;

		i686::vga_info tty_vga_info;

		i686::vga_color tty_vga_fg = i686::vga_color::WHITE;
		i686::vga_color tty_vga_bg = i686::vga_color::BLACK;
	#endif

	void tty_init()
	{
		#if defined(SYSTEM_ARCH_i686)
			i686::vga_init();
			tty_vga_info = i686::vga_get_info();
		#endif

		tty_clear();
	}

	void tty_write(char c)
	{
		#if defined(SYSTEM_ARCH_i686)

			if (c == '\n') // Newline character
			{
				tty_vga_col = 0;
				tty_vga_row ++;
			}
			else // Normal printable character
			{
				i686::vga_place_entry(c, i686::vga_make_color(tty_vga_fg, tty_vga_bg), tty_vga_col, tty_vga_row);
				tty_vga_col ++;
			}

			if (tty_vga_col >= tty_vga_info.cols) // We've gone past the last column
			{
				tty_vga_col = 0;
				tty_vga_row ++;
			}

			if (tty_vga_row >= tty_vga_info.rows) // We've gone past the last row
			{
				tty_vga_row = 0;
			}

			i686::vga_place_cursor(tty_vga_col, tty_vga_row); // Update the cursor
		#endif
	}

	void tty_write_str(const char* str)
	{
		for (umem i = 0; str[i] != '\0'; i ++) // Write each character
			tty_write(str[i]);
	}

	void tty_set_fg_color(ubyte color)
	{
		#if defined(SYSTEM_ARCH_i686)
			tty_vga_fg = (i686::vga_color)color;
		#endif
	}

	void tty_set_bg_color(ubyte color)
	{
		#if defined(SYSTEM_ARCH_i686)
			tty_vga_bg = (i686::vga_color)color;
		#endif
	}

	void tty_clear()
	{
		#if defined(SYSTEM_ARCH_i686)
			// Clear the screen first
			for (uint16 row = 0; row < tty_vga_info.rows; row ++)
			{
				for (uint16 col = 0; col < tty_vga_info.cols; col ++)
				{
					i686::vga_place_entry(' ', vga_make_color(tty_vga_fg, tty_vga_bg), col, row);
				}
			}

			tty_vga_col = 0;
			tty_vga_row = 0;
		#endif
	}
}
