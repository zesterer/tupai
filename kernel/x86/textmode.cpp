//
// file : textmode.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/x86/textmode.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/mem/mmap.hpp>
#include <tupai/task/task.hpp>
#include <tupai/arch.hpp>
#include <tupai/util/mutex.hpp>

#include <tupai/debug.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		static volatile uint16_t* buffer = (uint16_t*)0xB8000;
		static const int col_max = 80;
		static const int row_max = 25;
		static uint8_t color = 0x0F;

		static int col, row;

		void textmode_init()
		{
			// Reserve buffer
			mem::mmap::reserve_region(
				(void*)buffer,
				col_max * row_max * 16,
				task::get_kernel(),
				(uint8_t)mem::mmap::page_flags::STATIC
			);

			// Add the virtual kernel offset
			buffer = (uint16_t*)((size_t)buffer + arch_get_offset());

			col = 0;
			row = 0;

			textmode_clear();
			textmode_cursor_enable();
			textmode_cursor_move(col, row);
		}

		void textmode_cursor_enable()
		{
			outb(0x3D4, 0x0A);
			outb(0x3D5, 0x00);
		}

		void textmode_cursor_disable()
		{
			outb(0x3D4, 0x0A);
			outb(0x3D5, 0x3F);
		}

		void textmode_cursor_move(int col, int row)
		{
			uint16_t pos = col_max * row + col;
			outb(0x3D4, 0x0F);
			outb(0x3D5, (uint8_t)(pos & 0xFF));
			outb(0x3D4, 0x0E);
			outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
		}

		void textmode_move(int ncol, int nrow)
		{
			col = ncol;
			row = nrow;

			textmode_cursor_move(col, row);
		}

		void textmode_write(char c)
		{
			switch (c)
			{
			case '\n': // Newline
				{
					col = 0;
					row ++;
				}
				break;

			case '\b': // Backspace
				{
					if (col != 0)
						col --;
					else if (row != 0)
					{
						row --;
						col = col_max - 1;
					}
				}
				break;

			case '\r': // Do nothing
				break;

			default: // Printable character
				buffer[col_max * row + col] = (color << 8) | c;
				col ++;
				break;
			}

			if (col >= col_max)
			{
				col = 0;
				row ++;
			}

			while (row >= row_max)
			{
				textmode_scroll(1);
				row --;
			}

			textmode_cursor_move(col, row);
		}

		void textmode_clear()
		{
			for (int row = 0; row < row_max; row ++)
				for (int col = 0; col < col_max; col ++)
					buffer[col_max * row + col] = (color << 8) | ' ';
		}

		void textmode_scroll(int n)
		{
			if (n <= 0)
				return;

			for (int row = n; row < row_max; row ++)
				for (int col = 0; col < col_max; col ++)
					buffer[col_max * (row - n) + col] = buffer[col_max * row + col];

			for (int col = 0; col < col_max; col ++)
				buffer[col_max * (row_max - 1) + col] = (color << 8) | ' ';
		}

		void textmode_apply(util::ansi_cmd_t cmd)
		{
			switch (cmd.type)
			{
			case util::ansi_cmd::PASS:
				textmode_write(cmd.c);
				break;

			case util::ansi_cmd::SGR_PARAMETER:
				textmode_apply_sgr(cmd.sgr_parameter.n);
				break;

			default:
				break;
			}
		}

		void textmode_apply_sgr(int n)
		{
			if (n == 0)
				color = 0x0F;
			else if (n >= 30 && n <= 37)
				color = ((uint8_t)((n - 30) << 0) & 0x0F) | (color & 0xF0);
			else if (n >= 40 && n <= 47)
				color = ((uint8_t)((n - 40) << 4) & 0xF0) | (color & 0x0F);
		}
	}
}
