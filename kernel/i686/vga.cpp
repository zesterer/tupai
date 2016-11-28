/*
* 	file : vga.cpp
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
#include <tupai/i686/vga.hpp>
#include <tupai/i686/port.hpp>

namespace tupai
{
	namespace i686
	{
		uint16* VGA_BUFFER = (uint16*)0xB8000;
		uint16  VGA_COLS = 80;
		uint16  VGA_ROWS = 25;

		void vga_init()
		{
			// Clear the screen
			for (uint16 row = 0; row < VGA_ROWS; row ++)
			{
				for (uint16 col = 0; col < VGA_COLS; col ++)
				{
					vga_place_entry(' ', vga_make_color(vga_color::WHITE, vga_color::BLACK), col, row);
				}
			}
		}

		vga_info vga_get_info()
		{
			vga_info info;
			info.cols = VGA_COLS;
			info.rows = VGA_ROWS;
			return info;
		}

		ubyte vga_make_color(vga_color fg, vga_color bg)
		{
			return (((ubyte)bg & 0x0F) << 4) | ((ubyte)fg & 0x0F);
		}

		void vga_place_entry(char c, ubyte color, uint16 col, uint16 row)
		{
			uint32 index = row * VGA_COLS + col;
			VGA_BUFFER[index] = ((uint16)color << 8) | (uint16)c;
		}

		void vga_place_cursor(uint16 col, uint16 row)
		{
			uint32 index = row * VGA_COLS + col;

			// Cursor low bits
			port_out8(0x3D4, 0x0F);
			port_out8(0x3D5, (ubyte)(index & 0xFF));
			// Cursor high bits
			port_out8(0x3D4, 0x0E);
			port_out8(0x3D5, (ubyte)((index >> 8) & 0xFF));
		}

		void vga_shift_rows(sint16 n, vga_color fg, vga_color bg)
		{
			for (umem row = 0; row < VGA_ROWS; row ++)
			{
				if (row + n < VGA_ROWS)
				{
					// Set row equal to the row n below
					for (umem col = 0; col < VGA_COLS; col ++)
					{
						uint32 src_index = (row + n) * VGA_COLS + col;
						uint32 tgt_index = row * VGA_COLS + col;

						VGA_BUFFER[tgt_index] = VGA_BUFFER[src_index];
					}
				}
				else
				{
					// Wipe the row
					for (umem col = 0; col < VGA_COLS; col ++)
					{
						vga_place_entry(' ', vga_make_color(fg, bg), col, row);
					}
				}
			}
		}
	}
}
