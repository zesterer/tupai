//
// file : mem.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_UTIL_MEM_HPP
#define TUPAI_UTIL_MEM_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static __attribute__ ((unused)) size_t align_floor(size_t n, size_t align)
		{
			return n - n % align;
		}

		static __attribute__ ((unused)) size_t align_ceiling(size_t n, size_t align)
		{
			return (n == 0) ? align : (align + align * ((n - 1) / align));
		}

		static __attribute__ ((unused)) void mem_copy(const void* src, void* dest, size_t n)
		{
			for (size_t i = 0; i < n; i ++)
				((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
		}
	}
}

#endif
