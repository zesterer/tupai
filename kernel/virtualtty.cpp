/*
* 	file : virtualtty.cpp
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
#include <tupai/virtualtty.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/char.hpp>

namespace tupai
{
	void virtualtty::place_entry(uint32 c, uint16 col, uint16 row, byte fg_color, byte bg_color)
	{
		this->change_counter ++;

		if (row >= this->rows)
			return;

		ttyentry nttyentry;
		nttyentry.c = c;
		nttyentry.fg_color = fg_color;
		nttyentry.bg_color = bg_color;
		nttyentry.change_stamp = this->change_counter;

		this->buffer[this->cursor] = nttyentry;

		if (this->change_signal_func != nullptr)
			this->change_signal_func();
	}

	void virtualtty::write_entry(uint32 c, byte fg_color, byte bg_color)
	{
		uint32 col = this->cursor % this->cols;
		uint32 row = this->cursor / this->cols;

		if (util::is_printable(c) || (ubyte)c >= 128)
		{
			this->place_entry(c, col, row, fg_color, bg_color);
			this->cursor += 1;
		}
		else if (util::is_newline(c))
		{
			this->cursor = (row + 1) * this->cols;
		}
		else if (util::is_backspace(c))
		{
			if (this->cursor > 0)
				this->cursor --;
		}
	}

	void virtualtty::clear()
	{
		this->change_counter ++;

		ttyentry nttyentry;
		nttyentry.c = ' ';
		nttyentry.fg_color = this->default_fg_color;
		nttyentry.bg_color = this->default_bg_color;
		nttyentry.change_stamp = this->change_counter;

		for (uint32 i = 0; i < this->cols * this->rows; i ++)
			this->buffer[i] = nttyentry;

		this->cursor = 0;

		if (this->change_signal_func != nullptr)
			this->change_signal_func();
	}

	void virtualtty::move(uint16 col, uint16 row)
	{
		this->change_counter ++;

		this->cursor = row * this->cols + col;

		if (this->change_signal_func != nullptr)
			this->change_signal_func();
	}

	virtualtty virtualtty_create(uint16 cols, uint16 rows)
	{
		virtualtty ntty;

		ntty.cols = cols;
		ntty.rows = rows;
		ntty.cursor = 0;
		ntty.default_fg_color = 0xF;
		ntty.default_bg_color = 0x0;

		uint32 size = ntty.cols * ntty.rows;
		ntty.buffer = util::alloc<ttyentry>(size).val();

		ntty.clear();

		return ntty;
	}
}
