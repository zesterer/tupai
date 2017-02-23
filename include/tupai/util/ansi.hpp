/*
* 	file : ansi.hpp
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

#ifndef TUPAI_UTIL_ANSI_HPP
#define TUPAI_UTIL_ANSI_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/tty.hpp>

namespace tupai
{
	namespace util
	{
		struct ansi
		{
			static const byte BLACK         = 0x00;
			static const byte RED           = 0x01;
			static const byte GREEN         = 0x02;
			static const byte YELLOW        = 0x03;
			static const byte BLUE          = 0x04;
			static const byte MAGENTA       = 0x05;
			static const byte CYAN          = 0x06;
			static const byte LIGHT_GREY    = 0x07;
			static const byte DARK_GREY     = 0x08;
			static const byte LIGHT_RED     = 0x09;
			static const byte LIGHT_GREEN   = 0x0A;
			static const byte LIGHT_YELLOW  = 0x0B;
			static const byte LIGHT_BLUE    = 0x0C;
			static const byte LIGHT_MAGENTA = 0x0D;
			static const byte LIGHT_CYAN    = 0X0E;
			static const byte WHITE         = 0x0F;
		};

		void ansi_set_fg_color(ubyte color);
		void ansi_set_bg_color(ubyte color);
		void ansi_place_cursor(uint16 col, uint16 row);
		void ansi_show_cursor();
		void ansi_hide_cursor();
		void ansi_reset();
		void ansi_clear();
	}
}

#endif
