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
#include <tupai/util/ansi.hpp>

#include <tupai/tty.hpp>

namespace tupai
{
	namespace util
	{
		void ansi_set_fg_color(ubyte color) { tty_set_fg_color(color); }
		void ansi_set_bg_color(ubyte color) { tty_set_bg_color(color); }
		void ansi_place_cursor(uint16 col, uint16 row) { tty_place_cursor(col, row); }
		void ansi_show_cursor() { tty_show_cursor(); }
		void ansi_hide_cursor() { tty_hide_cursor(); }
		void ansi_reset() { tty_reset(); }
		void ansi_clear() { tty_clear(); }
	}
}
