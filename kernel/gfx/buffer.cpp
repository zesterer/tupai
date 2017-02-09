/*
* 	file : buffer.cpp
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
#include <tupai/gfx/buffer.hpp>
#include <tupai/util/mem.hpp>

#include <libk/stdio.hpp>

namespace tupai
{
	namespace gfx
	{
		uint32 tmpbuf[1024][768];

		void buffer::blit(buffer& other, uint16 tgt_x, uint16 tgt_y, uint16 w, uint16 h, uint16 src_x, uint16 src_y)
		{
			// Do nothing for now
		}

		buffer buffer_create(uint16 width, uint16 height)
		{
			buffer nbuffer;

			nbuffer.width = width;
			nbuffer.height = height;

			// TODO : This REALLY needs fixing!
			//nbuffer.pixels = util::alloc<color>(nbuffer.width * nbuffer.height).val();
			nbuffer.pixels = (color*)&tmpbuf;

			libk::printf("PIXELS = 0x%X\n", (umem)nbuffer.pixels - 0xC0000000);

			return nbuffer;
		}
	}
}
