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
#include <tupai/util/math.hpp>

namespace tupai
{
	bool ttyentry::equals(const ttyentry& other)
	{
		return other.c == this->c && other.fg_color == this->fg_color && other.bg_color == this->bg_color;
	}

	void virtualtty::place_entry(uint32 c, uint16 col, uint16 row)
	{
		this->change_counter ++;

		if (row >= this->rows)
			return;

		ttyentry nttyentry;
		nttyentry.c = c;
		nttyentry.fg_color = this->fg_color;
		nttyentry.bg_color = this->bg_color;
		nttyentry.change_stamp = this->change_counter;

		this->buffer[row * this->cols + col] = nttyentry;

		//if (this->change_signal_func != nullptr)
		//	this->change_signal_func();
	}

	void virtualtty::write_entry(uint32 c)
	{
		this->change_counter ++;

		// Force a redraw of the old cursor position
		this->buffer[this->cursor].change_stamp = this->change_counter;

		if (util::is_printable(c) || (ubyte)c >= 128)
		{
			this->place_entry(c, this->cursor % this->cols, this->cursor / this->cols);
			this->cursor ++;
		}
		else if (util::is_newline(c))
		{
			this->cursor = (this->cursor / this->cols + 1) * this->cols;
		}
		else if (util::is_backspace(c))
		{
			if (this->cursor > 0)
			{
				this->cursor --;
				this->place_entry(' ', this->cursor % this->cols, this->cursor / this->cols);
			}
		}

		while (this->cursor >= this->cols * this->rows)
		{
			this->scroll(1);
		}

		// Force a redraw of the new cursor position
		this->buffer[this->cursor].change_stamp = this->change_counter;
		this->vga_cursor_phase = 0;

		//if (this->change_signal_func != nullptr)
		//	this->change_signal_func();
	}

	void virtualtty::set_fg_color(byte fg_color)
	{
		this->fg_color = fg_color;
	}

	void virtualtty::set_bg_color(byte bg_color)
	{
		this->bg_color = bg_color;
	}

	void virtualtty::reset_color()
	{
		this->fg_color = this->default_fg_color;
		this->bg_color = this->default_bg_color;
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
		{
			if (!nttyentry.equals(this->buffer[i]))
				this->buffer[i] = nttyentry;
		}

		this->cursor = 0;

		//if (this->change_signal_func != nullptr)
		//	this->change_signal_func();
	}

	void virtualtty::scroll(uint16 rows)
	{
		this->change_counter ++;

		for (uint16 row = 0; row < this->rows; row ++)
		{
			for (uint16 col = 0; col < this->cols; col ++)
			{
				ttyentry tmp;

				if ((row + rows) * cols > 0 && (row + rows) * cols < this->cols * this->rows)
				{
					tmp = this->buffer[(row + rows) * this->cols + col];
				}
				else
				{
					tmp.c = ' ';
					tmp.fg_color = this->default_fg_color;
					tmp.bg_color = this->default_bg_color;
				}

				uint32 old_change_stamp = this->buffer[row * this->cols + col].change_stamp;

				if (tmp.equals(this->buffer[row * this->cols + col])) // We shouldn't force an update if the scroll didn't change the value
					tmp.change_stamp = old_change_stamp;
				else
					tmp.change_stamp = this->change_counter;

				this->buffer[row * this->cols + col] = tmp;
			}
		}

		this->cursor -= this->cols * rows;//util::min(this->cursor, (uint32)this->cols * rows);

		//if (this->change_signal_func != nullptr)
		//	this->change_signal_func();
	}

	void virtualtty::move(uint16 col, uint16 row)
	{
		this->change_counter ++;

		// Force a redraw of the old cursor position
		this->buffer[this->cursor].change_stamp = this->change_counter;

		this->cursor = row * this->cols + col;

		//if (this->change_signal_func != nullptr)
		//	this->change_signal_func();
	}

	void virtualtty::enable_cursor(bool enable)
	{
		this->change_counter ++;

		// Force a redraw of the old cursor position
		this->buffer[this->cursor].change_stamp = this->change_counter;

		this->cursor_enabled = enable;

		//if (this->change_signal_func != nullptr)
		//	this->change_signal_func();
	}

	virtualtty virtualtty_create(uint16 cols, uint16 rows)
	{
		virtualtty ntty;

		ntty.cols = cols;
		ntty.rows = rows;
		ntty.cursor = 0;
		ntty.cursor_enabled = true;
		ntty.default_fg_color = 0xF;
		ntty.default_bg_color = 0x0;
		ntty.fg_color = ntty.default_fg_color;
		ntty.bg_color = ntty.default_bg_color;

		uint32 size = ntty.cols * ntty.rows;
		ntty.buffer = util::alloc<ttyentry>(size).val();

		ntty.clear();

		return ntty;
	}
}
