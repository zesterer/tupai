/*
* 	file : color.hpp
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

#ifndef TUPAI_GFX_COLOR_HPP
#define TUPAI_GFX_COLOR_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	namespace gfx
	{
		struct color
		{
			uint32 value;

			byte red()   { return (this->value >> 24) & 0xFF; }
			byte green() { return (this->value >> 16) & 0xFF; }
			byte blue()  { return (this->value >>  8) & 0xFF; }
			byte alpha() { return (this->value >>  0) & 0xFF; }
		} __attribute((packed));
	}
}

#endif
