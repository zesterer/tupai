/*
* 	file : virtualtty.hpp
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

#ifndef TUPAI_VIRTUALTTY_HPP
#define TUPAI_VIRTUALTTY_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	struct ttyentry
	{
		uint32 c;
		byte fg_color;
		byte bg_color;
		uint32 change_stamp = 0;

		bool equals(const ttyentry& other);
	};

	struct virtualtty
	{
		ttyentry* buffer;

		uint16 cols;
		uint16 rows;
		uint32 cursor;
		bool cursor_enabled;
		byte default_fg_color;
		byte default_bg_color;
		byte fg_color;
		byte bg_color;

		uint32 change_counter = 0;

		void (*change_signal_func)() = nullptr;

		void place_entry(uint32 c, uint16 col, uint16 row);
		void write_entry(uint32 c);
		void set_fg_color(byte fg_color);
		void set_bg_color(byte bg_color);
		void scroll(uint16 rows);
		void reset_color();
		void clear();
		void move(uint16 col, uint16 row);
		void enable_cursor(bool enable);
	};

	virtualtty virtualtty_create(uint16 cols, uint16 rows);
}

#endif
