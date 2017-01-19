/*
* 	file : ansi.cpp
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
#include <tupai/type.hpp>
#include <tupai/util/char.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/vga.hpp>
#endif

namespace tupai
{
	namespace early
	{
		int ansi_escape_state = 0;
		char ansi_escape_color = 0;

		// Implementation
		static void ansi_impl_init();
		static void ansi_impl_handle(char c);
		static void ansi_impl_handle_escaped(char c);
		static void ansi_impl_clear();

		#if defined(SYSTEM_ARCH_i686)
			vga_info ansi_vga_info;
			uint ansi_vga_pos = 0;

			const vga_color ansi_vga_fg_default = (vga_color)0xF;
			const vga_color ansi_vga_bg_default = (vga_color)0x0;
			vga_color ansi_vga_fg = ansi_vga_fg_default;
			vga_color ansi_vga_bg = ansi_vga_bg_default;

			static uint16 ansi_vga_get_row(uint pos);
			static uint16 ansi_vga_get_col(uint pos);
			static void ansi_vga_init();
			static void ansi_vga_handle(char c);
			static void ansi_vga_handle_newline(char c);
			static void ansi_vga_handle_backspace(char c);
			static void ansi_vga_afterhandle();
			static void ansi_vga_clear();
			static void ansi_vga_set_fg(ubyte color);
			static void ansi_vga_set_bg(ubyte color);
		#endif

		void ansi_init() // stub
		{
			ansi_impl_init();
			ansi_impl_clear();
		}

		void ansi_handle(char c) // stub
		{
			ansi_impl_handle(c);
		}

		// Implementation

		static void ansi_impl_init()
		{
			ansi_vga_init();
		}

		static void ansi_impl_handle(char c)
		{
			if (ansi_escape_state != 0) // Escaped character
				ansi_impl_handle_escaped(c);
			else if (util::is_printable(c) || (ubyte)c >= 128) // Printable + Extended ASCII
				ansi_vga_handle(c);
			else if (util::is_newline(c)) // Newlines - \n and \r
				ansi_vga_handle_newline(c);
			else if (util::is_backspace(c)) // Backspace - \r
				ansi_vga_handle_backspace(c);
			else if (util::is_escape(c)) // Escape - 0x1B
				ansi_escape_state = 1;

			ansi_vga_afterhandle();
		}

		static void ansi_impl_handle_escaped(char c)
		{
			if (ansi_escape_state == 1)
			{
				if (util::is_hex(c))
				{
					ansi_escape_color = c;
					ansi_escape_state = 2;
				}
				else
					ansi_escape_state = 0;
			}
			else if (ansi_escape_state == 2)
			{
				if (c == 'm')
				{
					ansi_vga_set_fg(util::hex_to_num(ansi_escape_color));
					ansi_escape_state = 0;
				}
				else
					ansi_escape_state = 0;
			}
			else
				ansi_escape_state = 0;
		}

		static void ansi_impl_clear()
		{
			ansi_vga_clear();
		}

		#if defined(SYSTEM_ARCH_i686)
			static uint16 ansi_vga_get_row(uint pos) { return pos / ansi_vga_info.cols; }
			static uint16 ansi_vga_get_col(uint pos) { return pos % ansi_vga_info.cols; }

			static void ansi_vga_init()
			{
				vga_init();
				ansi_vga_info = vga_get_info();
			}

			static void ansi_vga_handle(char c)
			{
				// Place the character
				ubyte color = vga_make_color(ansi_vga_fg, ansi_vga_bg);
				vga_place_entry(c, color,  ansi_vga_get_col(ansi_vga_pos), ansi_vga_get_row(ansi_vga_pos));
				// Increment cursor position
				ansi_vga_pos ++;
			}

			static void ansi_vga_handle_newline(char c)
			{
				// Skip to the next line
				ansi_vga_pos = (ansi_vga_pos / ansi_vga_info.cols + 1) * ansi_vga_info.cols;
			}

			static void ansi_vga_handle_backspace(char c)
			{
				// Decrement cursor position
				ansi_vga_pos --;
				// Place a blank character
				ubyte color = vga_make_color(ansi_vga_fg_default, ansi_vga_bg_default);
				vga_place_entry(c, color,  ansi_vga_get_col(ansi_vga_pos), ansi_vga_get_row(ansi_vga_pos));
			}

			static void ansi_vga_afterhandle()
			{
				while (ansi_vga_pos >= ansi_vga_info.cols * ansi_vga_info.rows)
				{
					vga_shift_rows(1, ansi_vga_fg_default, ansi_vga_bg_default);
					ansi_vga_pos -= ansi_vga_info.cols;
				}

				// Reposition cursor
				vga_place_cursor(ansi_vga_get_col(ansi_vga_pos), ansi_vga_get_row(ansi_vga_pos));
			}

			static void ansi_vga_clear()
			{
				// Clear the screen
				for (uint16 row = 0; row < ansi_vga_info.rows; row ++)
				{
					for (uint16 col = 0; col < ansi_vga_info.cols; col ++)
					{
						ubyte color = vga_make_color(ansi_vga_fg_default, ansi_vga_bg_default);
						vga_place_entry(' ', color, col, row);
					}
				}

				// Reset cursor position
				ansi_vga_pos = 0;
			}

			static void ansi_vga_set_fg(ubyte color)
			{
				ansi_vga_fg = (vga_color)color;
			}

			static void ansi_vga_set_bg(ubyte color)
			{
				ansi_vga_bg = (vga_color)color;
			}
		#endif
	}
}
