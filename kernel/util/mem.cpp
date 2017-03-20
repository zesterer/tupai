/*
* 	file : mem.cpp
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
#include <tupai/util/mem.hpp>

namespace tupai
{
	namespace util
	{
		umem align_floor(umem n, umem align)
		{
			return n - n % align;
		}

		umem align_ceiling(umem n, umem align)
		{
			return (n + align) - n % align;
		}

		void mem_copy(const byte* src, byte* dest, umem n)
		{
			for (umem i = 0; i < n; i ++)
				dest[i] = src[i];
		}
	}
}
