/*
* 	file : buffer.hpp
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

#ifndef TUPAI_GFX_BUFFER_HPP
#define TUPAI_GFX_BUFFER_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/safetype.hpp>
#include <tupai/gfx/color.hpp>

namespace tupai
{
	namespace gfx
	{
		struct buffer
		{
			color* pixels = nullptr;

			uint16 width, height;

			const uint32* data() { return (const uint32*)this->pixels; }
			safeptr<uint32> data_mut() { return safeptr<uint32>((uint32*)this->pixels); }
			color sample_at(uint16 x, uint16 y) { return this->pixels[this->width * y + x]; }
			void blit(buffer& other, uint16 tgt_x = 0, uint16 tgt_y = 0, uint16 w = 0, uint16 h = 0, uint16 src_x = 0, uint16 src_y = 0);
		};

		buffer buffer_create(uint16 width, uint16 height);
	}
}

#endif
