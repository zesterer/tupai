/*
* 	file : bmp.cpp
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
#include <tupai/type.hpp>
#include <tupai/gfx/bmp.hpp>

#include <libk/stdio.hpp>
#include <tupai/util/conv.hpp>

namespace tupai
{
	namespace gfx
	{
		buffer bmp::to_buffer()
		{
			buffer nbuffer = buffer_create(this->info_header.width, this->info_header.height);

			byte* data = (byte*)((umem)this->self + this->file_header.offbits); // BGR format
			uint32* pixels = nbuffer.data_mut().val();

			libk::printf("OFF = 0x%X\n", this->file_header.offbits);

			for (uint32 i = 0; i < nbuffer.width * nbuffer.height; i ++)
				pixels[i] = (data[i * 4 + 2] << 16) | (data[i * 4 + 1] << 8) | (data[i * 4 + 3] << 24) | 0xFF;

			// The BMP is vertically mirrored. Make it the right way up
			nbuffer.mirror_vertical();

			return nbuffer;
		}

		struct bmp_header
		{
			bmp_file_header file_header;
			bmp_info_header info_header;
		};

		bmp bmp_from(ptr_t header)
		{
			bmp nbmp;

			bmp_header* temp = (bmp_header*)header;
			nbmp.self = (ptr_t)header;
			nbmp.file_header = temp->file_header;
			nbmp.info_header = temp->info_header;

			return nbmp;
		}
	}
}
