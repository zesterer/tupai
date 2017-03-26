/*
* 	file : psf.hpp
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

#ifndef TUPAI_GFX_PSF_HPP
#define TUPAI_GFX_PSF_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/gfx/buffer.hpp>

#include <tupai/util/vector.hpp>

namespace tupai
{
	namespace gfx
	{
		struct psf
		{
			util::vector<buffer> charbuffs;

			psf()
			{
				for (int i = 0; i < 16; i ++)
					this->charbuffs.push(buffer_create(8, 16, color(255, 255, 255, 255)));
			}

			const buffer& get_charbuff(char c)
			{
				return this->charbuffs[0];
			}
		};

		bmp bmp_from(ptr_t header);
	}
}

#endif
