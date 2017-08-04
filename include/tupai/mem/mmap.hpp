//
// file : mmap.hpp
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

#ifndef TUPAI_MEM_MMAP_HPP
#define TUPAI_MEM_MMAP_HPP

// Tupai
#include <proc/proc.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace mem
	{
		namespace mmap
		{
			enum class page_flags : uint8_t
			{
				STATIC = 1 << 0,
			}

			struct page_t
			{
				proc::proc_ptr_t owner;
				uint8_t flags;
			};

			void    init();
			page_t& get(void* phys_addr);
			int     alloc(void** phys_addr, proc::proc_ptr_t owner, uint8_t flags);
			int     reserve(void* phys_addr, proc::proc_ptr_t owner, uint8_t flags);
			int     dealloc(void* phys_addr);
		}
	}
}

#endif
