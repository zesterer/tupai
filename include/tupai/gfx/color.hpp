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
			uint32 value; // RGBA

			color(uint32 rgba)
			{
				this->value = rgba;
			}

			color(byte r, byte g, byte b, byte a = 0xFF)
			{
				this->value = (r << 24) | (g << 16) | (b << 8) | a;
			}

			byte r() const { return (this->value >> 24) & 0xFF; }
			byte g() const { return (this->value >> 16) & 0xFF; }
			byte b() const { return (this->value >>  8) & 0xFF; }
			byte a() const { return (this->value >>  0) & 0xFF; }

			color operator*(const color& other)
			{
				return color(
					(this->r() * other.r()) >> 8,
					(this->g() * other.g()) >> 8,
					(this->b() * other.b()) >> 8,
					(this->a() * other.a()) >> 8
				);
			}
		} __attribute((packed));

		static __attribute__ ((unused)) color merge_color(color lo, color hi)
		{
			return color(
				(hi.r() * hi.a() + lo.r() * (255 - hi.a())) >> 8,
				(hi.g() * hi.a() + lo.g() * (255 - hi.a())) >> 8,
				(hi.b() * hi.a() + lo.b() * (255 - hi.a())) >> 8,
				(hi.a()          + lo.a() * (255 - hi.a())) >> 8
			);
		}
	}
}

#endif
