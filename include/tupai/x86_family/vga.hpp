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

#ifndef TUPAI_X86_FAMILY_VGA_HPP
#define TUPAI_X86_FAMILY_VGA_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/safetype.hpp>
#include <tupai/virtualtty.hpp>

namespace tupai
{
	namespace x86_family
	{
		bool vga_init();

		safeptr<virtualtty> vga_get_virtualtty();
		void vga_textmode_put_char(char c, uint16 col, uint16 row);
		void vga_textmode_clear();
	}
}

#endif
