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

// Libk
#include <libk/ctype.hpp>

namespace tupai
{
	uint16 tty_vga_col;
	uint16 tty_vga_row;

	vga_info tty_vga_info;

	vga_color tty_vga_bg = (vga_color)tty_color::DEFAULT_BG;
	vga_color tty_vga_fg = (vga_color)tty_color::DEFAULT_FG;

	const uint16 TTY_TAB_SIZE = 4;

	static void tty_shift_rows(sint16 n, vga_color fg, vga_color bg);

	void tty_init()
	{
		vga_init();
		tty_vga_info = vga_get_info();
		tty_clear();
	}

	void tty_write(char c)
	{
		if (libk::isprint(c) || (ubyte)c >= 128) // Normal printable character + Extended ASCII
		{
			vga_place_entry(c, vga_make_color(tty_vga_fg, tty_vga_bg), tty_vga_col, tty_vga_row);
			tty_vga_col ++;
		}
		else if (c == '\n') // Newline character
		{
			tty_vga_col = 0;
			tty_vga_row ++;
		}
		#ifdef CFG_TTY_INTERPRET_TAB
			else if (c == '\t') // Tab character
			{
				tty_vga_col ++;
				while (tty_vga_col % TTY_TAB_SIZE != 0)
					tty_vga_col ++;
			}
		#endif

		if (tty_vga_col >= tty_vga_info.cols) // We've gone past the last column
		{
			tty_vga_col = 0;
			tty_vga_row ++;
		}

		if (tty_vga_row >= tty_vga_info.rows) // We've gone past the last row
		{
			tty_vga_row = tty_vga_info.rows - 1;
			tty_shift_rows(1, tty_vga_fg, tty_vga_bg);
		}

		vga_place_cursor(tty_vga_col, tty_vga_row); // Update the cursor
	}

	void tty_write_str(const char* str)
	{
		for (umem i = 0; str[i] != '\0'; i ++) // Write each character
			tty_write(str[i]);
	}

	void tty_set_fg_color(ubyte color)
	{
		tty_vga_fg = (vga_color)color;
	}

	void tty_set_bg_color(ubyte color)
	{
		tty_vga_bg = (vga_color)color;
	}

	void tty_place_cursor(uint16 col, uint16 row)
	{
		tty_vga_col = col;
		tty_vga_row = row;
	}

	void tty_clear()
	{
		// Clear the screen first
		for (uint16 row = 0; row < tty_vga_info.rows; row ++)
		{
			for (uint16 col = 0; col < tty_vga_info.cols; col ++)
			{
				vga_place_entry(' ', vga_make_color(tty_vga_fg, tty_vga_bg), col, row);
			}
		}

		tty_vga_col = 0;
		tty_vga_row = 0;
	}

	void tty_shift_rows(sint16 n, vga_color fg, vga_color bg)
	{
		for (umem row = 0; row < tty_vga_info.rows; row ++)
		{
			if (row + n < tty_vga_info.rows)
			{
				// Set row equal to the row n below
				for (umem col = 0; col < tty_vga_info.cols; col ++)
				{
					uint32 src_index = (row + n) * tty_vga_info.cols + col;
					uint32 tgt_index = row * tty_vga_info.cols + col;

					tty_vga_info.buffer[tgt_index] = tty_vga_info.buffer[src_index];
				}
			}
			else
			{
				// Wipe the row
				for (umem col = 0; col < tty_vga_info.cols; col ++)
				{
					vga_place_entry(' ', vga_make_color(fg, bg), col, row);
				}
			}
		}
	}
}
