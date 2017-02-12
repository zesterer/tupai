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
			uint32 skip = this->width;

			uint32 width = (w == 0 ? other.width : w);
			uint32 height = (h == 0 ? other.height : h);

			uint32 tgt_line = this->width * tgt_y + tgt_x;
			uint32 src_line = other.width * src_y + src_x;
			for (uint16 line_count = 0; line_count < height; line_count ++)
			{
				for (uint16 col_count = 0; col_count < width; col_count ++)
					this->pixels[tgt_line + col_count] = other.pixels[src_line + col_count];

				tgt_line += skip;
				src_line += other.width;
			}
		}

		void buffer::mirror_horizontal()
		{
			for (uint32 i = 0; i < this->width / 2; i ++)
			{
				for (uint32 j = 0; j < this->height; j ++)
				{
					color tmp = this->pixels[this->width * j + i];
					this->pixels[this->width * j + i] = this->pixels[this->width * j + (this->width - i)];
					this->pixels[this->width * j + (this->width - i)] = tmp;
				}
			}
		}

		void buffer::mirror_vertical()
		{
			for (uint16 i = 0; i < this->width; i ++)
			{
				for (uint16 j = 0; j < this->height / 2; j ++)
				{
					color tmp = this->pixels[this->width * j + i];
					this->pixels[this->width * j + i] = this->pixels[this->width * (this->height - j - 1) + i];
					this->pixels[this->width * (this->height - j - 1) + i] = tmp;
				}
			}
		}

		buffer buffer_create(uint16 width, uint16 height)
		{
			buffer nbuffer;

			nbuffer.width = width;
			nbuffer.height = height;

			// TODO : This REALLY needs fixing!
			if (width != 1024 || height != 768)
				nbuffer.pixels = util::alloc<color>(nbuffer.width * nbuffer.height).val();
			else
				nbuffer.pixels = (color*)&tmpbuf;

			return nbuffer;
		}
	}
}
