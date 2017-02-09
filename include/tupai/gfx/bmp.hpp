/*
* 	file : bmp.hpp
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

#ifndef TUPAI_GFX_BMPHEADER_HPP
#define TUPAI_GFX_BMPHEADER_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/gfx/buffer.hpp>

namespace tupai
{
	namespace gfx
	{
		struct bmp_file_header
		{
			uint16 type;
			uint32 size;
			uint16 res1;
			uint16 res2;
			uint32 offbits;
		} __attribute__((packed));

		struct bmp_info_header
		{
			uint32 size;
			int32  width;
			int32  height;
			uint16 planes;
			uint16 bitcount;
			uint32 compression;
			uint32 sizeimage;
			int32  xpixelspermeter;
			int32  ypixelspermeter;
			uint32 clrused;
			uint32 clrimportant;
		} __attribute__((packed));

		struct bmp
		{
			ptr_t self;
			bmp_file_header file_header;
			bmp_info_header info_header;

			buffer to_buffer();
		};

		bmp bmp_from(ptr_t header);
	}
}

#endif
