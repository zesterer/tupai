//
// file : virtual.hpp
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

#ifndef TUPAI_MEM_VIRTUAL_HPP
#define TUPAI_MEM_VIRTUAL_HPP

// Tupai
#include <tupai/util/vector.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace mem
	{
		namespace virtual
		{
			struct page_t
			{
				size_t phys_index;
				size_t virt_index;
			}

			struct space_t
			{
				util::vector_t<page_t> _pages;
			};
		}
	}
}

#endif
