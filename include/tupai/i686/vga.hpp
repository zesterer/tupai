/*
* 	file : vga.hpp
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

#ifndef TUPAI_I686_VGA_HPP
#define TUPAI_I686_VGA_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	enum class vga_color : ubyte
	{
		BLACK         = 0x00,
		BLUE          = 0x01,
		GREEN         = 0x02,
		CYAN          = 0x03,
		RED           = 0x04,
		MAGENTA       = 0x05,
		BROWN         = 0x06,
		LIGHT_GREY    = 0x07,
		DARK_GREY     = 0x08,
		LIGHT_BLUE    = 0x09,
		LIGHT_GREEN   = 0x0A,
		LIGHT_CYAN    = 0x0B,
		LIGHT_RED     = 0x0C,
		LIGHT_MAGENTA = 0x0D,
		LIGHT_BROWN   = 0X0E,
		WHITE         = 0x0F,
	};

	struct vga_info
	{
		uint16 cols;
		uint16 rows;
		uint16* buffer;
	};

	void     vga_init();
	vga_info vga_get_info();
	ubyte    vga_make_color(vga_color fg, vga_color bg);
	void     vga_place_entry(char c, ubyte color, uint16 col, uint16 row);
	void     vga_place_cursor(uint16 col, uint16 row);
}

#endif
