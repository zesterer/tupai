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
#include <tupai/util/math.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/vga.hpp>
	#include <tupai/i686/serial.hpp>
#endif

namespace tupai
{
	namespace early
	{
		int ansi_escape_state = 0;
		int ansi_escape_number[2] = {1, 1};

		// Implementation
		static void ansi_impl_init();
		static void ansi_impl_handle(char c);
		static void ansi_impl_handle_escaped(char c);
		static void ansi_impl_clear();
		static void ansi_impl_set_sgr(int code);
		static void ansi_impl_set_pos(int h, int v);

		#if defined(SYSTEM_ARCH_i686)
			vga_info ansi_vga_info;
			uint ansi_vga_pos = 0;

			const vga_color ansi_vga_fg_default = (vga_color)0x7;
			const vga_color ansi_vga_bg_default = (vga_color)0x0;
			vga_color ansi_vga_fg = ansi_vga_fg_default;
			vga_color ansi_vga_bg = ansi_vga_bg_default;

			static uint16 ansi_vga_get_row(uint pos);
			static uint16 ansi_vga_get_col(uint pos);
			static vga_color ansi_vga_conv_color(ubyte col);
			static void ansi_vga_init();
			static void ansi_vga_handle(char c);
			static void ansi_vga_handle_newline(char c);
			static void ansi_vga_handle_backspace(char c);
			static void ansi_vga_afterhandle();
			static void ansi_vga_clear();
			static void ansi_vga_set_fg(ubyte color);
			static void ansi_vga_set_bg(ubyte color);
			static void ansi_vga_set_pos(int h, int v);
			static void ansi_vga_reset();
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
			{
				ansi_vga_handle(c);

				#if defined(CFG_ENABLE_SERIAL_DEBUG)
					serial_write(1, c); // Default write to COM1
				#endif
			}
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
			//ansi_vga_handle(c);
			bool rehandle = false; // Whether the character should be rehandled (on a state boundary, for example)
			switch (ansi_escape_state)
			{
			case 1:
				{
					if (util::is_digit(c)) // Starts an escape sequence
					{
						ansi_escape_number[0] = 1 * util::digit_to_num(c).val();
						ansi_escape_state = 2;
					}
					else // Return to normal - TODO: add CSI s and CSI u implementation here
					{
						ansi_escape_state = 0;
						rehandle = true;
					}
				}
				break;

			case 2:
				{
					if (util::is_digit(c))
					{
						ansi_escape_number[0] *= 10;
						ansi_escape_number[0] += util::digit_to_num(c).val();
						ansi_escape_state = 2;
					}
					else if (c == 'J') // Clear screen
					{
						if (ansi_escape_number[0] == 2) // TODO : Add 0 and 1 after this!
							ansi_impl_clear();
						ansi_escape_state = 0;
					}
					else if (c == 'm') // SGR code
					{
						ansi_impl_set_sgr(ansi_escape_number[0]);
						ansi_escape_state = 0;
					}
					else if (c == ';') // Second parameter
					{
						ansi_escape_number[1] = 0;
						ansi_escape_state = 3;
					}
					else // Reset
					{
						ansi_escape_state = 0;
						rehandle = true;
					}
				}
				break;

			case 3:
				{
					if (util::is_digit(c))
					{
						ansi_escape_number[1] = 1 * util::digit_to_num(c).val();
						ansi_escape_state = 4;
					}
					else // Reset
					{
						ansi_escape_state = 0;
						rehandle = true;
					}
				}
				break;

			case 4:
				{
					if (util::is_digit(c))
					{
						ansi_escape_number[1] *= 10;
						ansi_escape_number[1] += util::digit_to_num(c).val();
						ansi_escape_state = 4;
					}
					else if (c == 'f') // Move cursor H/V position
					{
						ansi_impl_set_pos(ansi_escape_number[0], ansi_escape_number[1]);
						ansi_escape_state = 0;
					}
					else
					{
						ansi_escape_state = 0;
						rehandle = true;
					}
				}
				break;

			case 0:
			default: // Erroneous state
				ansi_escape_state = 0;
				break;
			}

			if (rehandle)
				ansi_impl_handle(c);
		}

		static void ansi_impl_clear()
		{
			ansi_vga_clear();
		}

		static void ansi_impl_set_sgr(int code)
		{
			if (code == 0)
				ansi_vga_reset();
			else if (code >= 30 && code <= 37)
				ansi_vga_set_fg(code - 30);
			else if (code >= 90 && code <= 97)
				ansi_vga_set_fg(8 + code - 90);
			else if (code >= 40 && code <= 47)
				ansi_vga_set_bg(code - 40);
			else if (code >= 100 && code <= 107)
				ansi_vga_set_bg(8 + code - 100);
		}

		static void ansi_impl_set_pos(int h, int v)
		{
			ansi_vga_set_pos(h, v);
		}

		// VGA
		#if defined(SYSTEM_ARCH_i686)
			static uint16 ansi_vga_get_row(uint pos) { return pos / ansi_vga_info.cols; }
			static uint16 ansi_vga_get_col(uint pos) { return pos % ansi_vga_info.cols; }

			static vga_color ansi_vga_conv_color(ubyte col)
			{
				const byte coltable[] = {0x0, 0x4, 0x2, 0x6, 0x1, 0x5, 0x3, 0x7, 0x8, 0xC, 0xA, 0xE, 0x9, 0xD, 0xB, 0xF};
				return (vga_color)coltable[col];
			}

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

			static void ansi_vga_handle_newline(char c __attribute__ ((unused)))
			{
				// Skip to the next line
				ansi_vga_pos = (ansi_vga_pos / ansi_vga_info.cols + 1) * ansi_vga_info.cols;
			}

			static void ansi_vga_handle_backspace(char c __attribute__ ((unused)))
			{
				// Decrement cursor position
				ansi_vga_pos --;
				// Place a blank character
				ubyte color = vga_make_color(ansi_vga_fg_default, ansi_vga_bg_default);
				vga_place_entry(' ', color,  ansi_vga_get_col(ansi_vga_pos), ansi_vga_get_row(ansi_vga_pos));
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
				ansi_vga_fg = ansi_vga_conv_color(color);
			}

			static void ansi_vga_set_bg(ubyte color)
			{
				ansi_vga_bg = ansi_vga_conv_color(color);
			}

			static void ansi_vga_set_pos(int h, int v)
			{
				ansi_vga_pos = (v * ansi_vga_info.cols + h) % (ansi_vga_info.cols * ansi_vga_info.rows);
			}

			static void ansi_vga_reset()
			{
				ansi_vga_fg = ansi_vga_fg_default;
				ansi_vga_bg = ansi_vga_bg_default;
			}
		#endif
	}
}
